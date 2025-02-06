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
	{
		MessageBoxA(nullptr, "Show", "ZoneSelect", 0);
		wnd->show(1, 1);
	}
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
	Zeal::EqUI::ComboWnd* cmb = (Zeal::EqUI::ComboWnd*)wnd->GetChildItem("Zeal_ZoneSelect_Combobox");
	if (cmb) {

		Zeal::EqStructures::EQWorldData*  tmp = (Zeal::EqStructures::EQWorldData*)(*(DWORD*)0x7F9494);
		std::vector<std::string> zones;
		for (int i = 0;i<77;i++)
			if (tmp->Zones[i])
				zones.push_back(tmp->Zones[i]->name_long);
		cmb->DeleteAll();
		if (zones.size() > kMaxComboBoxItems)
			zones.resize(kMaxComboBoxItems);
		ZealService::get_instance()->ui->AddListItems(cmb, zones);
	}

	ui->AddComboCallback(wnd, "Zeal_ZoneSelect_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) {
		if (value >= 0)
		{
			Zeal::EqUI::CXSTR texture_name;
			wnd->CmbListWnd->GetItemText(&texture_name, value, 0);
			if (texture_name.Data)
			{
				MessageBoxA(nullptr, texture_name.CastToCharPtr(), "Zone Select", 0);
				//ZealService::get_instance()->target_ring->texture_name.set(std::string(texture_name));
				texture_name.FreeRep();
			}
		}
	});
	//button1 = wnd->GetChildItem("ZealDialogButton1");
	//button2 = wnd->GetChildItem("ZealDialogButton2");
	//label = wnd->GetChildItem("ZealDialogMessage");
	//input = (Zeal::EqUI::EditWnd*)wnd->GetChildItem("ZealDialogInput");
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