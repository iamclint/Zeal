#include "ui_zoneselect.h"
#include "Zeal.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "string_util.h"
void ui_zoneselect::CleanUI()
{
	if (wnd)
	{
		// Should already be deactivated by the time the framework calls this.
		ui->DestroySidlScreenWnd(wnd);
		wnd = nullptr;
	}
	if (btn_wnd)
	{
		ui->DestroySidlScreenWnd(btn_wnd);
		btn_wnd = nullptr;
	}

}
void ui_zoneselect::Deactivate()
{
	if (wnd)
	{
		wnd->show(0, false);
	}

	if (btn_wnd)
		btn_wnd->show(0, false);
}
void ui_zoneselect::Hide()
{
	if (wnd)
	{
		wnd->show(0, 0);
	}
}
void ui_zoneselect::Show()
{
	if (wnd)
	{
		wnd->show(1, 1);
	}
}
void ui_zoneselect::HideButton()
{
	if (btn_wnd)
		btn_wnd->show(0, false);
}
void ui_zoneselect::ShowButton()
{
	if (btn_wnd)
		btn_wnd->show(1, true);  /// Show and put on top.
}
void ui_zoneselect::InitUI()
{
	static constexpr int kMaxComboBoxItems = 50;
	if (!Zeal::EqGame::Windows->CharacterSelect)
	{
		MessageBoxA(NULL, "Character Select Not Loaded", "UI Error", 1);
		return;
	}

	// The ZoneSelect is optional in the XML files so support the case where it is not found
	// by adding a simple button wnd.
	if (!Zeal::EqGame::Windows->CharacterSelect->GetChildItem("Zeal_ZoneSelect"))
	{
		if (!btn_wnd)
		{
			static const char* xml_file = "./uifiles/zeal/EQUI_ZealButtonWnd.xml";
			if (ui && std::filesystem::exists(xml_file))
				btn_wnd = ui->CreateSidlScreenWnd("ZealButtonWnd");
		}

		if (!btn_wnd)  // Bail out since failed to create a button to select zones.
			return;

		Zeal::EqUI::BasicWnd* btn = btn_wnd->GetChildItem("Zeal_Button", true);
		if (btn)
			btn->Text.Set("Zone Select");

		// ShowButton() is deferred to SelectCharacter() to support returning from Explore.
	}

	// Initialize Zone select window.
	if (wnd)
		Zeal::EqGame::print_chat("Warning: Init out of sync for ui_zoneselect");

	static const char* xml_file = "./uifiles/zeal/EQUI_ZoneSelect.xml";
	if (!wnd && ui && std::filesystem::exists(xml_file))
		wnd = ui->CreateSidlScreenWnd("ZealZoneSelect");

	if (!wnd)
	{
		Zeal::EqGame::print_chat("Error: Failed to load %s", xml_file);
		return;
	}

	ui->AddButtonCallback(Zeal::EqGame::Windows->CharacterSelect, "Explore_Button", [this](Zeal::EqUI::BasicWnd* btnwnd) {
		Hide();  // Close the window when entering explore mode.
		HideButton();  // Also hide the zone select toggle button.
		});

	Zeal::EqUI::EQWND* btn_parent = btn_wnd ? btn_wnd : Zeal::EqGame::Windows->CharacterSelect;
	const char* label = btn_wnd ? "Zeal_Button" : "Zeal_ZoneSelect";
	ui->AddButtonCallback(btn_parent, label, [this](Zeal::EqUI::BasicWnd* btnwnd) {
		if (wnd && wnd->IsVisible)
			Hide();  // Makes the button act as a toggle to hide the window.
		else
			Show();  // Opens the window for selecting a zone.
		});

	Zeal::EqUI::ListWnd* lst = (Zeal::EqUI::ListWnd*)wnd->GetChildItem("Zeal_ZoneSelect_ListBox");
	ui->AddButtonCallback(wnd, "Zeal_ZoneSelect_Apply", [this, lst](Zeal::EqUI::BasicWnd* btn) { 

		std::string str_zone_id = lst->GetItemText(lst->SelectedIndex, 0);
		int zone_id = 0;
		Zeal::String::tryParse(str_zone_id, &zone_id);
		if (zone_id <= 0)
			ui->inputDialog->show("Character Select Zone", "Please select a valid zone", "OK", "", nullptr, nullptr, false);
		else {
			ZealService::get_instance()->charselect->ZoneIndex.set(zone_id);
			ui->inputDialog->show("Character Select Zone",
				"The change in zone will take effect the next time you enter character select", "OK", "", nullptr, nullptr, false);
			Hide();
		}
	});
	if (lst) {

		Zeal::EqStructures::EQWorldData*  world = (Zeal::EqStructures::EQWorldData*)(*(DWORD*)0x7F9494);
		std::deque<std::vector<std::string>> zone_list;
		for (int i = 0; i < 225; i++)
		{
			if (world->Zones[i])
		{
				if (!std::filesystem::exists(std::string(world->Zones[i]->name_short) + ".s3d"))
					continue;
				zones[world->Zones[i]->name_long] = world->Zones[i]->zone_id;
				if (world->Zones[i]->zone_id == ZealService::get_instance()->charselect->ZoneIndex.get())
			{
					lst->SelectedIndex = i;
					Zeal::EqUI::BasicWnd* lbl = (Zeal::EqUI::BasicWnd*)wnd->GetChildItem("Zeal_ZoneSelect_CurrentZone");
					if (lbl)
						lbl->Text.Set(world->Zones[i]->name_long);
				}
				zone_list.push_back({ std::to_string(world->Zones[i]->zone_id), world->Zones[i]->name_short, world->Zones[i]->name_long });
			}
		}

		std::sort(zone_list.begin(), zone_list.end(), [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
			return a[2] < b[2]; // Compare second elements
		});
		zone_list.push_front({ "185","CharSelect", "Character Select" });
		std::vector<std::vector<std::string>> vec_zone_list(zone_list.begin(), zone_list.end());
		lst->DeleteAll();
		ZealService::get_instance()->ui->AddListItems(lst, vec_zone_list);
	}
}
ui_zoneselect::~ui_zoneselect()
{

}
ui_zoneselect::ui_zoneselect(ZealService* zeal, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	//zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { Deactivate(); }, callback_type::DeactivateUI);
	zeal->callbacks->AddGeneric([this]() { InitUI(); Hide(); }, callback_type::InitCharSelectUI);

	ui->AddXmlInclude("EQUI_ZoneSelect.xml");
	ui->AddXmlInclude("EQUI_ZealButtonWnd.xml");
}