#include "ui_options.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

float GetSensitivityFromSlider(int value)
{
	return value / 100.f;
}
int GetSensitivityForSlider(float* value)
{
	if (value && *value > 0)
		return static_cast<int>(*value * 100.f);
	else
		return 0;
}

//most of the options window xml was put together by nillipus
void ui_options::InitUI()
{
	/*add callbacks when the buttons are pressed in the options window*/
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_HideCorpse", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->looting_hook->set_hide_looted(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_Cam", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->camera_mods->set_smoothing(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_BlueCon", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_bluecon(wnd->Checked); });
	//ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_Timestamp", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_timestamp(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_Input", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_input(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_Escape", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "Escape", wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_RaidEscapeLock", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "EscapeRaidLock", wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_ShowHelm", [](Zeal::EqUI::BasicWnd* wnd) { Zeal::EqGame::print_chat("Show helm toggle"); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_AltContainerTooltips", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->tooltips->set_alt_all_containers(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_SpellbookAutoStand", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->movement->set_spellbook_autostand(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_FloatingDamage", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->floating_damage->set_enabled(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_UseOldSens", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->camera_mods->set_old_sens(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_TargetRing", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->target_ring->set_enabled(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_ClassicClasses", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->chat_hook->set_classes(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_TellWindows", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->tells->SetEnabled(wnd->Checked); });
	ui->AddCheckboxCallback(Zeal::EqGame::Windows->Options, "Zeal_Map", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_enabled(wnd->Checked, true); });
	ui->AddComboCallback(Zeal::EqGame::Windows->Options, "Zeal_Timestamps_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->chat_hook->set_timestamp(value); });
	ui->AddComboCallback(Zeal::EqGame::Windows->Options, "Zeal_MapBackground_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_background(value); });
	ui->AddComboCallback(Zeal::EqGame::Windows->Options, "Zeal_MapAlignment_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_alignment(value); });
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_PanDelaySlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->set_pan_delay(value*4); 
		ui->SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_FirstPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_FirstPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_ThirdPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_ThirdPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_FoVSlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = 45.0f + (static_cast<float>(value) / 100.0f) * 45.0f;
		ZealService::get_instance()->camera_mods->set_fov(val);
		ui->SetLabelValue("Zeal_FoVValueLabel", "%.0f", val);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_HoverTimeout_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		int val = value * 5;
		ZealService::get_instance()->tooltips->set_timer(val);
		ui->SetLabelValue("Zeal_HoverTimeout_Value", "%i ms", val);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_TargetRingFill_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = static_cast<float>(value) / 100.0f;
		ZealService::get_instance()->target_ring->set_pct(val);
		ui->SetLabelValue("Zeal_TargetRingFill_Value", "%.2f", val);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_MapZoom_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_zoom(value + 100);  // Note zoom offset.
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_MapLeft_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_left(value);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_MapRight_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_right(value);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_MapTop_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_top(value);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_MapBottom_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_bottom(value);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_MapPositionSize_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_position_size(value);
	});
	ui->AddSliderCallback(Zeal::EqGame::Windows->Options, "Zeal_MapMarkerSize_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_marker_size(value);
	});

	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_PanDelayValueLabel");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_FirstPersonLabel_X");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_FirstPersonLabel_Y");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_ThirdPersonLabel_X");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_ThirdPersonLabel_Y");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_FoVValueLabel");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_HoverTimeout_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_VersionValue");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_TargetRingFill_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_MapZoom_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_MapLeft_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_MapRight_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_MapTop_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_MapBottom_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_MapPositionSize_Value");
	ui->AddLabel(Zeal::EqGame::Windows->Options, "Zeal_MapMarkerSize_Value");

	/*set the current states*/
	UpdateOptions();
}


void ui_options::CleanUI()
{
	Zeal::EqGame::print_debug("Clean UI Options");
}


void ui_options::UpdateOptions()
{
	ui->SetComboValue("Zeal_Timestamps_Combobox", ZealService::get_instance()->chat_hook->TimeStampsStyle);
	ui->SetComboValue("Zeal_HideCorpseCombobox", 2);
	ui->SetSliderValue("Zeal_PanDelaySlider", ZealService::get_instance()->camera_mods->pan_delay > 0.f ? ZealService::get_instance()->camera_mods->pan_delay / 4 : 0.f);
	ui->SetSliderValue("Zeal_HoverTimeout_Slider", ZealService::get_instance()->tooltips->hover_timeout > 0 ? ZealService::get_instance()->tooltips->hover_timeout / 5 : 0);
	ui->SetSliderValue("Zeal_ThirdPersonSlider_Y", GetSensitivityForSlider(&ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd));
	ui->SetSliderValue("Zeal_ThirdPersonSlider_X", GetSensitivityForSlider(&ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd));
	ui->SetSliderValue("Zeal_FirstPersonSlider_Y", GetSensitivityForSlider(&ZealService::get_instance()->camera_mods->user_sensitivity_y));
	ui->SetSliderValue("Zeal_FirstPersonSlider_X", GetSensitivityForSlider(&ZealService::get_instance()->camera_mods->user_sensitivity_x));
	ui->SetSliderValue("Zeal_FoVSlider", static_cast<int>((ZealService::get_instance()->camera_mods->fov - 45.0f) / 45.0f * 100.0f));
	ui->SetLabelValue("Zeal_FoVValueLabel", "%.0f", ZealService::get_instance()->camera_mods->fov);
	ui->SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x);
	ui->SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y);
	ui->SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
	ui->SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
	ui->SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
	ui->SetLabelValue("Zeal_HoverTimeout_Value", "%d ms", ZealService::get_instance()->tooltips->hover_timeout);
	ui->SetChecked("Zeal_HideCorpse", ZealService::get_instance()->looting_hook->hide_looted);
	ui->SetChecked("Zeal_Cam", ZealService::get_instance()->camera_mods->enabled);
	ui->SetChecked("Zeal_BlueCon", ZealService::get_instance()->chat_hook->UseBlueCon);
	ui->SetChecked("Zeal_Timestamp", ZealService::get_instance()->chat_hook->TimeStampsStyle);
	ui->SetChecked("Zeal_Input", ZealService::get_instance()->chat_hook->UseZealInput);
	ui->SetChecked("Zeal_Escape", ZealService::get_instance()->ini->getValue<bool>("Zeal", "Escape"));
	ui->SetChecked("Zeal_AltContainerTooltips", ZealService::get_instance()->tooltips->all_containers);
	ui->SetChecked("Zeal_SpellbookAutoStand", ZealService::get_instance()->movement->spellbook_autostand);
	ui->SetChecked("Zeal_FloatingDamage", ZealService::get_instance()->floating_damage->enabled);
	ui->SetChecked("Zeal_UseOldSens", ZealService::get_instance()->camera_mods->use_old_sens);
	ui->SetChecked("Zeal_TargetRing", ZealService::get_instance()->target_ring->enabled);
	ui->SetChecked("Zeal_ClassicClasses", ZealService::get_instance()->chat_hook->UseClassicClassNames);
	ui->SetChecked("Zeal_TellWindows", ZealService::get_instance()->tells->enabled);


	ui->SetLabelValue("Zeal_VersionValue", "%s (%s)", ZEAL_VERSION, ZEAL_BUILD_VERSION);
	ui->SetSliderValue("Zeal_TargetRingFill_Slider", static_cast<int>(ZealService::get_instance()->target_ring->get_ring_pct() * 100.0f));
	ui->SetLabelValue("Zeal_TargetRingFill_Value", "%.2f", ZealService::get_instance()->target_ring->get_ring_pct());

	UpdateOptionsMapOnly();

}

void ui_options::UpdateOptionsMapOnly()
{
	ui->SetChecked("Zeal_Map", ZealService::get_instance()->zone_map->is_enabled());
	ui->SetComboValue("Zeal_MapBackground_Combobox", ZealService::get_instance()->zone_map->get_background());
	ui->SetComboValue("Zeal_MapAlignment_Combobox", ZealService::get_instance()->zone_map->get_alignment());
	ui->SetSliderValue("Zeal_MapZoom_Slider", ZealService::get_instance()->zone_map->get_zoom() - 100);
	ui->SetSliderValue("Zeal_MapLeft_Slider", ZealService::get_instance()->zone_map->get_map_left());
	ui->SetSliderValue("Zeal_MapRight_Slider", ZealService::get_instance()->zone_map->get_map_right());
	ui->SetSliderValue("Zeal_MapTop_Slider", ZealService::get_instance()->zone_map->get_map_top());
	ui->SetSliderValue("Zeal_MapBottom_Slider", ZealService::get_instance()->zone_map->get_map_bottom());
	ui->SetSliderValue("Zeal_MapPositionSize_Slider", ZealService::get_instance()->zone_map->get_position_size());
	ui->SetSliderValue("Zeal_MapMarkerSize_Slider", ZealService::get_instance()->zone_map->get_marker_size());
	ui->SetLabelValue("Zeal_MapZoom_Value", "%i%%", ZealService::get_instance()->zone_map->get_zoom());
	ui->SetLabelValue("Zeal_MapLeft_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_left());
	ui->SetLabelValue("Zeal_MapRight_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_right());
	ui->SetLabelValue("Zeal_MapTop_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_top());
	ui->SetLabelValue("Zeal_MapBottom_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_bottom());
	ui->SetLabelValue("Zeal_MapPositionSize_Value", "%i%%", ZealService::get_instance()->zone_map->get_position_size());
	ui->SetLabelValue("Zeal_MapMarkerSize_Value", "%i%%", ZealService::get_instance()->zone_map->get_marker_size());
}


ui_options::ui_options(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	//if (Zeal::EqGame::is_in_game()) InitUI();
}
ui_options::~ui_options()
{
}

