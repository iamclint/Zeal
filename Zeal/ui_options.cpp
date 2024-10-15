#include "ui_options.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
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
//Zeal::EqUI::EQWND* __fastcall deconstructor(Zeal::EqUI::EQWND* wnd, int u, byte p1)
//{
//	wnd->IsVisible = false;
//	//reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*)>(0x56e680)(wnd);
//	//if (p1)
//	//{
//	//	reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, Zeal::EqUI::BasicWnd*)>(0x5c63b3)(wnd, wnd);
//	//}
//	wnd->MouseHoverTimer = 0;
//	wnd->ValidCXWnd = 0;
//	wnd->Unknown0008 = 0;
//	wnd->vtbl->Deconstructor = 0;
//	MessageBoxA(0, "UHH deconstruct??", "Zeal options", 1);
//	return 0;
//}
//most of the options window xml was put together by nillipus

void PrintUIError()
{
	Zeal::EqGame::print_chat("Warning: The zeal ui files are not in place, cannot load zeal options!");
}

int __fastcall WndNotification(Zeal::EqUI::BasicWnd* wnd, int unused, Zeal::EqUI::BasicWnd* sender, int message, int data)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	if (Zeal::EqGame::Windows && sender == (Zeal::EqUI::BasicWnd*)Zeal::EqGame::Windows->ColorPicker)
	{
		if (message == 0x1E && ui->clicked_button)
		{
			ui->clicked_button->TextColor.ARGB = data;
			ui->options->SaveColors();
		}
	}
	return reinterpret_cast<int (__thiscall*)(Zeal::EqUI::BasicWnd * wnd, Zeal::EqUI::BasicWnd * sender, int message, int data)>(0x56e920)(wnd, sender, message, data);
}

void ui_options::SaveColors()
{
	IO_ini* ini = ZealService::get_instance()->ini.get();
	for (auto& [index, btn] : color_buttons)
	{
		ini->setValue("ZealColors", "Color" + std::to_string(index), std::to_string(btn->TextColor.ARGB));
	}
}

DWORD ui_options::GetColor(int index)
{
	auto it = color_buttons.find(index);
	return (it == color_buttons.end()) ? 0xFFFFFFFF : it->second->TextColor.ARGB;
}

void ui_options::LoadColors()
{
	IO_ini* ini = ZealService::get_instance()->ini.get();
	for (auto& [index, btn] : color_buttons)
	{
		if (ini->exists("ZealColors", "Color" + std::to_string(index)))
			btn->TextColor.ARGB = ini->getValue<DWORD>("ZealColors", "Color" + std::to_string(index));
		if (!ini->exists("ZealColors", "Color" + std::to_string(index))) //Adds default Nameplate colors for new users
		{
			color_buttons[0]->TextColor.ARGB = 0xFFFF8000; //AFK - Orange
			color_buttons[1]->TextColor.ARGB = 0xFFCFFF00; //LFG - Yellow
			color_buttons[2]->TextColor.ARGB = 0xFFFF0000; //LinkDead - Red
			color_buttons[3]->TextColor.ARGB = 0xFFFF8080; //Guild Member - White Red
			color_buttons[4]->TextColor.ARGB = 0xFFFFFFFF; //Raid Member - White Light Purple
			color_buttons[5]->TextColor.ARGB = 0xFF00FF32; //Group Member - Light Green
			color_buttons[6]->TextColor.ARGB = 0xFFFF0000; //PVP - Red
			color_buttons[7]->TextColor.ARGB = 0xFF85489C; //Roleplay - Purple
			color_buttons[8]->TextColor.ARGB = 0xFFFFFF80; //OtherGuild Member - White Yellow
			color_buttons[9]->TextColor.ARGB = 0xFF3D6BDC; //Not in Guild Member - Default Blue
			color_buttons[10]->TextColor.ARGB = 0xFF000000; //Npc Corpse - Black
			color_buttons[11]->TextColor.ARGB = 0xFFFFFFFF; //Players Corpse - White Light Purple
			if (ZealService::get_instance()->ini->getValue<bool>("Zeal", "Bluecon"))
				color_buttons[12]->TextColor.ARGB = Zeal::EqGame::get_user_color(70); //BlueCon - Keeps original BlueCon if set from old Options menu
			if (!ZealService::get_instance()->ini->getValue<bool>("Zeal", "Bluecon"))
				color_buttons[12]->TextColor.ARGB = 0xFF0040FF; //BlueCon - Default DarkBlue is ligher than CON_BLUE for new users
		}
	}
}

void ui_options::InitUI()
{
	if (!wnd)
	{
		if (std::filesystem::exists("./uifiles/zeal/EQUI_ZealOptions.xml"))
		{
		//	ui->LoadSidl("uifiles\\zeal\\", "uifiles\\default\\", "EQUI_ZealOptions.xml");
			wnd = ui->CreateSidlScreenWnd("ZealOptions");
			wnd->vtbl->WndNotification = WndNotification;
		}
		else
		{
			PrintUIError();
			return;
		}
	}

	InitGeneral();
	InitCamera();
	InitMap();
	InitTargetRing();
	InitColors();

	isReady = true;
	/*set the current states*/
	UpdateOptions();
}

int ScaleFloatToSlider(float value, float fmin, float fmax, Zeal::EqUI::SliderWnd* wnd)
{
	if (!wnd)
		return 0;
	// Clamp the float value between fmin and fmax
	value = std::clamp(value, fmin, fmax);

	// Normalize the float value to the range [0, 1]
	float normalized = (value - fmin) / (fmax - fmin);

	// Map the normalized float to the integer range [imin, imax]
	return static_cast<int>(0 + normalized * (wnd->max_val));
}
float ScaleSliderToFloat(int ivalue, float fmin, float fmax, Zeal::EqUI::SliderWnd* wnd)
{
	if (!wnd)
		return 0;
	// Clamp the integer value between imin and imax
	ivalue = std::clamp(ivalue, 0, wnd->max_val);

	// Normalize the integer value to the range [0, 1]
	float normalized = static_cast<float>(ivalue) / (wnd->max_val);

	// Map the normalized value to the float range [fmin, fmax]
	return fmin + normalized * (fmax - fmin);
}
void ui_options::InitColors()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}

	for (int i = 0; i < 100; i++)
	{
		Zeal::EqUI::BasicWnd* btn = ui->AddButtonCallback(wnd, "Zeal_Color" + std::to_string(i), [](Zeal::EqUI::BasicWnd* wnd) { Zeal::EqGame::Windows->ColorPicker->Activate(wnd, wnd->TextColor.ARGB); });
		if (btn)
			color_buttons[i] = btn;
	}
	LoadColors();
}
void ui_options::InitGeneral()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}

	/*add callbacks when the buttons are pressed in the options window*/
	ui->AddCheckboxCallback(wnd, "Zeal_HideCorpse", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->looting_hook->set_hide_looted(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_Cam", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->camera_mods->set_smoothing(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_BlueCon", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_bluecon(wnd->Checked); });
	//ui->AddCheckboxCallback(wnd, "Zeal_Timestamp", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_timestamp(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_Input", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_input(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_Escape", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "Escape", wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_RaidEscapeLock", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "EscapeRaidLock", wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_ShowHelm", [](Zeal::EqUI::BasicWnd* wnd) { Zeal::EqGame::print_chat("Show helm toggle"); });
	ui->AddCheckboxCallback(wnd, "Zeal_AltContainerTooltips", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->tooltips->set_alt_all_containers(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_SpellbookAutoStand", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->movement->set_spellbook_autostand(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_FloatingDamage", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->floating_damage->set_enabled(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_ClassicClasses", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->chat_hook->set_classes(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateColors", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->colors_set_enabled(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateConColors", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->con_colors_set_enabled(wnd->Checked); });
	
	ui->AddCheckboxCallback(wnd, "Zeal_TellWindows", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->tells->SetEnabled(wnd->Checked); });
	ui->AddComboCallback(wnd, "Zeal_Timestamps_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->chat_hook->set_timestamp(value); });
	ui->AddSliderCallback(wnd, "Zeal_HoverTimeout_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		int val = value * 5;
		ZealService::get_instance()->tooltips->set_timer(val);
		ui->SetLabelValue("Zeal_HoverTimeout_Value", "%i ms", val);
		});

	ui->AddLabel(wnd, "Zeal_HoverTimeout_Value");
	ui->AddLabel(wnd, "Zeal_VersionValue");
}
void ui_options::InitCamera()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}
	ui->AddCheckboxCallback(wnd, "Zeal_UseOldSens", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->camera_mods->set_old_sens(wnd->Checked); });
	ui->AddSliderCallback(wnd, "Zeal_PanDelaySlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->set_pan_delay(value * 4);
		ui->SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
		});
	ui->AddSliderCallback(wnd, "Zeal_FirstPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x);
		});
	ui->AddSliderCallback(wnd, "Zeal_FirstPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y);
		});
	ui->AddSliderCallback(wnd, "Zeal_ThirdPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
		});
	ui->AddSliderCallback(wnd, "Zeal_ThirdPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd = GetSensitivityFromSlider(value);
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
		});
	ui->AddSliderCallback(wnd, "Zeal_FoVSlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = 45.0f + (static_cast<float>(value) / 100.0f) * 45.0f;
		ZealService::get_instance()->camera_mods->set_fov(val);
		ui->SetLabelValue("Zeal_FoVValueLabel", "%.0f", val);
		});

	ui->AddLabel(wnd, "Zeal_PanDelayValueLabel");
	ui->AddLabel(wnd, "Zeal_FirstPersonLabel_X");
	ui->AddLabel(wnd, "Zeal_FirstPersonLabel_Y");
	ui->AddLabel(wnd, "Zeal_ThirdPersonLabel_X");
	ui->AddLabel(wnd, "Zeal_ThirdPersonLabel_Y");
	ui->AddLabel(wnd, "Zeal_FoVValueLabel");
}
void ui_options::InitMap()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}
	ui->AddCheckboxCallback(wnd, "Zeal_Map", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_enabled(wnd->Checked, true); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapExternalWindow", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_external_enable(wnd->Checked, true); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapShowGroup", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_show_group(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapShowRaid", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_show_raid(wnd->Checked); });
	ui->AddComboCallback(wnd, "Zeal_MapBackground_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_background(value); });
	ui->AddComboCallback(wnd, "Zeal_MapAlignment_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_alignment(value); });
	ui->AddComboCallback(wnd, "Zeal_MapLabels_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_labels_mode(value); });

	ui->AddSliderCallback(wnd, "Zeal_MapZoom_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_zoom(value * 15 + 100);  // Note scale and zoom offset.
		});
	ui->AddSliderCallback(wnd, "Zeal_MapLeft_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_left(value);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapWidth_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_width(value);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapTop_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_top(value);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapHeight_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_map_height(value);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapPositionSize_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_position_size(value);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapMarkerSize_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_marker_size(value);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapBackgroundAlpha_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_background_alpha(value);
		});
	ui->AddLabel(wnd, "Zeal_MapZoom_Value");
	ui->AddLabel(wnd, "Zeal_MapLeft_Value");
	ui->AddLabel(wnd, "Zeal_MapWidth_Value");
	ui->AddLabel(wnd, "Zeal_MapTop_Value");
	ui->AddLabel(wnd, "Zeal_MapHeight_Value");
	ui->AddLabel(wnd, "Zeal_MapPositionSize_Value");
	ui->AddLabel(wnd, "Zeal_MapMarkerSize_Value");
	ui->AddLabel(wnd, "Zeal_MapBackgroundAlpha_Value");
}
void ui_options::InitTargetRing()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}
	ui->AddLabel(wnd, "Zeal_TargetRingFill_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingSize_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingRotation_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingSegments_Value");

	ui->AddCheckboxCallback(wnd, "Zeal_TargetRing", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->target_ring->set_enabled(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_TargetRingAttackIndicator", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->target_ring->set_indicator(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_TargetRingForward", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->target_ring->set_rotation_match(wnd->Checked); });

	ui->AddComboCallback(wnd, "Zeal_TargetRingTexture_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) {
		if (value > 0)
		{
			Zeal::EqUI::CXSTR texture_name;
			wnd->CmbListWnd->GetItemText(&texture_name, value, 0);
			if (texture_name.Data)
			{
				ZealService::get_instance()->target_ring->set_texture(texture_name.Data->Text);
				texture_name.FreeRep();
			}
		}
		else
		{
			ZealService::get_instance()->target_ring->set_texture("");
		}
	});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingFill_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = ScaleSliderToFloat(value, 0, 1, wnd);
		ZealService::get_instance()->target_ring->set_pct(val);
		ui->SetLabelValue("Zeal_TargetRingFill_Value", "%.2f", val);
	});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingSize_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		//float val = static_cast<float>(value) / 5.0f;
		float val = ScaleSliderToFloat(value, 0, 20, wnd);
		ZealService::get_instance()->target_ring->set_size(val);
		ui->SetLabelValue("Zeal_TargetRingSize_Value", "%.2f", val);
		});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingRotation_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		//float val = ((static_cast<float>(value)-50.f)/100.f)*2;
		float val = ScaleSliderToFloat(value, -1, 1, wnd);
		ZealService::get_instance()->target_ring->set_rotation_speed(val);
		ui->SetLabelValue("Zeal_TargetRingRotation_Value", "%.2f", val);
		});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingSegments_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->target_ring->set_segments(value);
		ui->SetLabelValue("Zeal_TargetRingSegments_Value", "%i", value);
	}, 128);

}

void ui_options::UpdateOptions()
{
	if (!isReady)
		return;
	if (!wnd)
	{
		PrintUIError();
		return;
	}

	UpdateOptionsCamera();
	UpdateOptionsGeneral();
	UpdateOptionsTargetRing();
	UpdateOptionsMap();

}

void ui_options::UpdateOptionsGeneral()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}
	ui->SetComboValue("Zeal_Timestamps_Combobox", ZealService::get_instance()->chat_hook->TimeStampsStyle);
	ui->SetSliderValue("Zeal_HoverTimeout_Slider", ZealService::get_instance()->tooltips->hover_timeout > 0 ? ZealService::get_instance()->tooltips->hover_timeout / 5 : 0);
	ui->SetLabelValue("Zeal_HoverTimeout_Value", "%d ms", ZealService::get_instance()->tooltips->hover_timeout);
	ui->SetChecked("Zeal_HideCorpse", ZealService::get_instance()->looting_hook->hide_looted);
	ui->SetChecked("Zeal_TellWindows", ZealService::get_instance()->tells->enabled);
	ui->SetChecked("Zeal_ClassicClasses", ZealService::get_instance()->chat_hook->UseClassicClassNames);
	ui->SetLabelValue("Zeal_VersionValue", "%s (%s)", ZEAL_VERSION, ZEAL_BUILD_VERSION);
	ui->SetChecked("Zeal_BlueCon", ZealService::get_instance()->chat_hook->UseBlueCon);
	ui->SetChecked("Zeal_Timestamp", ZealService::get_instance()->chat_hook->TimeStampsStyle);
	ui->SetChecked("Zeal_Input", ZealService::get_instance()->chat_hook->UseZealInput);
	ui->SetChecked("Zeal_Escape", ZealService::get_instance()->ini->getValue<bool>("Zeal", "Escape"));
	ui->SetChecked("Zeal_AltContainerTooltips", ZealService::get_instance()->tooltips->all_containers);
	ui->SetChecked("Zeal_SpellbookAutoStand", ZealService::get_instance()->movement->spellbook_autostand);
	ui->SetChecked("Zeal_FloatingDamage", ZealService::get_instance()->floating_damage->enabled);
	ui->SetChecked("Zeal_NameplateColors", ZealService::get_instance()->nameplate->nameplateColors);
	ui->SetChecked("Zeal_NameplateConColors", ZealService::get_instance()->nameplate->nameplateconColors);
}
void ui_options::UpdateOptionsCamera()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}
	ui->SetSliderValue("Zeal_PanDelaySlider", ZealService::get_instance()->camera_mods->pan_delay > 0.f ? ZealService::get_instance()->camera_mods->pan_delay / 4 : 0.f);
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
	ui->SetChecked("Zeal_Cam", ZealService::get_instance()->camera_mods->enabled);
	ui->SetChecked("Zeal_UseOldSens", ZealService::get_instance()->camera_mods->use_old_sens);
}
void ui_options::UpdateOptionsTargetRing()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}
	ui->SetChecked("Zeal_TargetRing", ZealService::get_instance()->target_ring->get_enabled());
	ui->SetChecked("Zeal_TargetRingAttackIndicator", ZealService::get_instance()->target_ring->get_indicator());
	ui->SetChecked("Zeal_TargetRingForward", ZealService::get_instance()->target_ring->get_rotation_match());
	ui->SetSliderValue("Zeal_TargetRingFill_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->get_pct(), 0, 1, ui->GetSlider("Zeal_TargetRingFill_Slider")));
	ui->SetSliderValue("Zeal_TargetRingSize_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->get_size(), 0, 20, ui->GetSlider("Zeal_TargetRingSize_Slider")));
	ui->SetSliderValue("Zeal_TargetRingRotation_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->get_rotation_speed(), -1, 1, ui->GetSlider("Zeal_TargetRingSize_Slider")));
	ui->SetSliderValue("Zeal_TargetRingSegments_Slider", static_cast<int>(ZealService::get_instance()->target_ring->get_segments()));
	ui->SetLabelValue("Zeal_TargetRingFill_Value", "%.2f", ZealService::get_instance()->target_ring->get_pct());
	ui->SetLabelValue("Zeal_TargetRingSegments_Value", "%i", ZealService::get_instance()->target_ring->get_segments());
	ui->SetLabelValue("Zeal_TargetRingRotation_Value", "%.2f", ZealService::get_instance()->target_ring->get_rotation_speed());
	ui->SetLabelValue("Zeal_TargetRingSize_Value", "%.2f", ZealService::get_instance()->target_ring->get_size());
}
void ui_options::UpdateOptionsMap()
{
	if (!wnd)
	{
		PrintUIError();
		return;
	}
	ui->SetChecked("Zeal_Map", ZealService::get_instance()->zone_map->is_enabled());
	ui->SetChecked("Zeal_MapExternalWindow", ZealService::get_instance()->zone_map->is_external_enabled());
	ui->SetChecked("Zeal_MapShowGroup", ZealService::get_instance()->zone_map->is_show_group_enabled());
	ui->SetChecked("Zeal_MapShowRaid", ZealService::get_instance()->zone_map->is_show_raid_enabled());
	ui->SetComboValue("Zeal_MapBackground_Combobox", ZealService::get_instance()->zone_map->get_background());
	ui->SetComboValue("Zeal_MapAlignment_Combobox", ZealService::get_instance()->zone_map->get_alignment());
	ui->SetComboValue("Zeal_MapLabels_Combobox", ZealService::get_instance()->zone_map->get_labels_mode());
	ui->SetSliderValue("Zeal_MapZoom_Slider", (ZealService::get_instance()->zone_map->get_zoom() - 100)/15);  // 100 to 1600%
	ui->SetSliderValue("Zeal_MapLeft_Slider", ZealService::get_instance()->zone_map->get_map_left());
	ui->SetSliderValue("Zeal_MapWidth_Slider", ZealService::get_instance()->zone_map->get_map_width());
	ui->SetSliderValue("Zeal_MapTop_Slider", ZealService::get_instance()->zone_map->get_map_top());
	ui->SetSliderValue("Zeal_MapHeight_Slider", ZealService::get_instance()->zone_map->get_map_height());
	ui->SetSliderValue("Zeal_MapPositionSize_Slider", ZealService::get_instance()->zone_map->get_position_size());
	ui->SetSliderValue("Zeal_MapMarkerSize_Slider", ZealService::get_instance()->zone_map->get_marker_size());
	ui->SetSliderValue("Zeal_MapBackgroundAlpha_Slider", ZealService::get_instance()->zone_map->get_background_alpha());
	ui->SetLabelValue("Zeal_MapZoom_Value", "%i%%", ZealService::get_instance()->zone_map->get_zoom());
	ui->SetLabelValue("Zeal_MapLeft_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_left());
	ui->SetLabelValue("Zeal_MapWidth_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_width());
	ui->SetLabelValue("Zeal_MapTop_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_top());
	ui->SetLabelValue("Zeal_MapHeight_Value", "%i%%", ZealService::get_instance()->zone_map->get_map_height());
	ui->SetLabelValue("Zeal_MapPositionSize_Value", "%i%%", ZealService::get_instance()->zone_map->get_position_size());
	ui->SetLabelValue("Zeal_MapMarkerSize_Value", "%i%%", ZealService::get_instance()->zone_map->get_marker_size());
	ui->SetLabelValue("Zeal_MapBackgroundAlpha_Value", "%i%%", ZealService::get_instance()->zone_map->get_background_alpha());
}

void ui_options::RenderUI()
{
	if (wnd && Zeal::EqGame::Windows && Zeal::EqGame::Windows->Options)
	{
		bool was_visible = wnd->IsVisible;
		wnd->IsVisible = Zeal::EqGame::Windows->Options->IsVisible;
		if (!was_visible && wnd->IsVisible)
		{
			UpdateOptions();
			ZealService::get_instance()->target_ring->options_opened();
		}
	}
}


void ui_options::CleanUI()
{
	isReady = false;
	if (wnd)
	{
		wnd->show(0, 0);
		Zeal::EqUI::ComboWnd* cmb = (Zeal::EqUI::ComboWnd*)wnd->GetChildItem("Zeal_TargetRingTexture_Combobox");
		if (cmb)
		{
			cmb->CmbListWnd->SelectedIndex = -1;
			cmb->DeleteAll();
		}
		wnd->Deconstruct();
		wnd = nullptr;
	}
}
void ui_options::Deactivate()
{
	if (wnd)
	{
		wnd->show(0, 0);
		Zeal::EqUI::ComboWnd* cmb =  (Zeal::EqUI::ComboWnd*)wnd->GetChildItem("Zeal_TargetRingTexture_Combobox");
		if (cmb)
		{
			cmb->CmbListWnd->SelectedIndex = -1;
			cmb->DeleteAll();
		}
	}
}

ui_options::ui_options(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	wnd = nullptr;
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { RenderUI(); }, callback_type::RenderUI);
	zeal->callbacks->AddGeneric([this]() { Deactivate(); }, callback_type::DeactivateUI);
	//if (Zeal::EqGame::is_in_game()) InitUI();
}
ui_options::~ui_options()
{
}

