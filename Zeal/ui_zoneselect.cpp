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
}
void ui_zoneselect::Deactivate()
{
	if (wnd)
	{
		wnd->show(0, false);
	}
}
void ui_zoneselect::Hide()
{
	if (wnd)
		wnd->show(0, 0);
}
void ui_zoneselect::Show()
{
	if (wnd)
		wnd->show(1, 1);
}
void ui_zoneselect::InitUI()
{
	static constexpr int kMaxComboBoxItems = 50;
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
	Zeal::EqUI::ListWnd* lst = (Zeal::EqUI::ListWnd*)wnd->GetChildItem("Zeal_ZoneSelect_ListBox");
	ZealService::get_instance()->ui->AddButtonCallback(wnd, "Zeal_ZoneSelect_Apply", [this, lst](Zeal::EqUI::BasicWnd* btn) { 
		
		std::string str_zone_id = lst->GetItemText(lst->SelectedIndex, 0);
		int zone_id = 0;
		Zeal::String::tryParse(str_zone_id, &zone_id);
		ZealService::get_instance()->charselect->ZoneIndex.set(zone_id);
		ZealService::get_instance()->ui->inputDialog->show("Character Select Zone",
			"The change in zone will take effect the next time you enter character select", "OK", "", nullptr, nullptr, false);
	});
	if (lst) {

		Zeal::EqStructures::EQWorldData*  world = (Zeal::EqStructures::EQWorldData*)(*(DWORD*)0x7F9494);
		std::vector<std::vector<std::string>> zone_list;
		for (int i = 0; i < 225; i++)
		{
			if (world->Zones[i])
			{
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

		lst->DeleteAll();
		ZealService::get_instance()->ui->AddListItems(lst, zone_list);
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
	zeal->callbacks->AddGeneric([this]() { InitUI();  Show(); }, callback_type::InitCharSelectUI);

	ui->AddXmlInclude("EQUI_ZoneSelect.xml");
}