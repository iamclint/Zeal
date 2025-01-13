#include "ui_options.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
#include <algorithm>
#include "ZealSettings.h"

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
int GetSensitivityForSlider(ZealSetting<float>& value)
{
	if (value.get() > 0)
		return static_cast<int>(value.get() * 100.f);
	else
		return 0;
}

int __fastcall WndNotification(Zeal::EqUI::BasicWnd* wnd, int unused, Zeal::EqUI::BasicWnd* sender, int message, int data)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	if (ui && Zeal::EqGame::Windows && sender == (Zeal::EqUI::BasicWnd*)Zeal::EqGame::Windows->ColorPicker)
	{
		if (message == 0x1E && ui->clicked_button)
		{
			ui->clicked_button->TextColor.ARGB = data;
			ui->options->SaveColors();
		}
	}
	return reinterpret_cast<int (__thiscall*)(Zeal::EqUI::BasicWnd * wnd, Zeal::EqUI::BasicWnd * sender, int message, int data)>(0x56e920)(wnd, sender, message, data);
}

void ui_options::SaveColors() const
{
	IO_ini* ini = ZealService::get_instance()->ini.get();
	for (auto& [index, btn] : color_buttons)
	{
		ini->setValue("ZealColors", "Color" + std::to_string(index), std::to_string(btn->TextColor.ARGB));
	}
}

DWORD ui_options::GetColor(int index) const
{
	auto it = color_buttons.find(index);
	return (it == color_buttons.end()) ? 0xFFFFFFFF : it->second->TextColor.ARGB;
}

void ui_options::LoadColors()
{
	IO_ini* ini = ZealService::get_instance()->ini.get();

	if (!ini->exists("ZealColors", "Color0")) //Adds default Nameplate Color to Button1  for new users
	{
		if (color_buttons.count(0))
			color_buttons[0]->TextColor.ARGB = 0xFFFF8000; //AFK - Orange
	}
	if (!ini->exists("ZealColors", "Color1")) //Adds default Nameplate Color to Button2 for new users
	{
		if (color_buttons.count(1))
			color_buttons[1]->TextColor.ARGB = 0xFFCFFF00; //LFG - Yellow
	}
	if (!ini->exists("ZealColors", "Color2")) //Adds default Nameplate Color to Button3 for new users
	{
		if (color_buttons.count(2))
			color_buttons[2]->TextColor.ARGB = 0xFFFF0000; //LinkDead - Red
	}
	if (!ini->exists("ZealColors", "Color3")) //Adds default Nameplate Color to Button4 for new users
	{
		if (color_buttons.count(3))
			color_buttons[3]->TextColor.ARGB = 0xFFFF8080; //Guild Member - White Red
	}
	if (!ini->exists("ZealColors", "Color4")) //Adds default Nameplate Color to Button5 for new users
	{
		if (color_buttons.count(4))
			color_buttons[4]->TextColor.ARGB = 0xFFFFFFFF; //Raid Member - White Light Purple
	}
	if (!ini->exists("ZealColors", "Color5")) //Adds default Nameplate Color to Button6 for new users
	{
		if (color_buttons.count(5))
			color_buttons[5]->TextColor.ARGB = 0xFF00FF32; //Group Member - Light Green
	}
	if (!ini->exists("ZealColors", "Color6")) //Adds default Nameplate Color to Button7 for new users
	{
		if (color_buttons.count(6))
			color_buttons[6]->TextColor.ARGB = 0xFFFF0000; //PVP - Red
	}
	if (!ini->exists("ZealColors", "Color7")) //Adds default Nameplate Color to Button8 for new users
	{
		if (color_buttons.count(7))
			color_buttons[7]->TextColor.ARGB = 0xFF85489C; //Roleplay - Purple
	}
	if (!ini->exists("ZealColors", "Color8")) //Adds default Nameplate Color to Button9 for new users
	{
		if (color_buttons.count(8))
			color_buttons[8]->TextColor.ARGB = 0xFFFFFF80; //OtherGuild Member - White Yellow
	}
	if (!ini->exists("ZealColors", "Color9")) //Adds default Nameplate Color to Button10 for new users
	{
		if (color_buttons.count(9))
			color_buttons[9]->TextColor.ARGB = 0xFF3D6BDC; //DefaultAdventurer - Default Blue
	}
	if (!ini->exists("ZealColors", "Color10")) //Adds default Nameplate Color to Button11 for new users
	{
		if (color_buttons.count(10))
			color_buttons[10]->TextColor.ARGB = 0xFF000000; //Npc Corpse - Black
	}
	if (!ini->exists("ZealColors", "Color11")) //Adds default Nameplate Color to Button12 for new users
	{
		if (color_buttons.count(11))
			color_buttons[11]->TextColor.ARGB = 0xFFFFFFFF; //Players Corpse - White Light Purple
	}
	if (!ini->exists("ZealColors", "Color12")) //Adds default Nameplate Color to Button13 for new users
	{
		if (color_buttons.count(12))
			color_buttons[12]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0x0, 0xf0, 0x0); //CON_GREEN
	}
	if (!ini->exists("ZealColors", "Color13")) //Adds default Nameplate Color to Button14 for new users
	{
		if (color_buttons.count(13))
			color_buttons[13]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0x0, 0xf0, 0xf0);//CON_LIGHTBLUE
	}
	if (!ini->exists("ZealColors", "Color14")) //Adds default Nameplate Color to Button15 for new users
	{
		if (color_buttons.count(14))
		{
			if (ZealService::get_instance()->ini->getValue<bool>("Zeal", "Bluecon"))
				color_buttons[14]->TextColor.ARGB = Zeal::EqGame::get_user_color(70); //BlueCon - Keeps original BlueCon if set from old Options, Color #70
			else
				color_buttons[14]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0x0, 0x40, 0xf0); //Default DarkBlue is lighter than CON_BLUE as Nameplate Color Button15 for new users
		}
	}
	if (!ini->exists("ZealColors", "Color15")) //Adds default Nameplate Color to Button16 for new users
	{
		if (color_buttons.count(15))
			color_buttons[15]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0xf0);//CON_WHITE
	}
	if (!ini->exists("ZealColors", "Color16")) //Adds default Nameplate Color to Button17 for new users
	{
		if (color_buttons.count(16))
			color_buttons[16]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0x0); //CON_YELLOW
	}
	if (!ini->exists("ZealColors", "Color17")) //Adds default Nameplate Color to Button18 for new users
	{
		if (color_buttons.count(17))
			color_buttons[17]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xf0, 0x0, 0x0); //CON_RED
	}
	if (!ini->exists("ZealColors", "Color18")) //Adds default Nameplate Color to Button18 for new users
	{
		if (color_buttons.count(18))
			color_buttons[18]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xff, 0x80, 0xff); //Target Pink Default
	}
	if (!ini->exists("ZealColors", "Color19")) //My Pet Damage
	{
		if (color_buttons.count(19))
			color_buttons[19]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0xf0); //Default White
	}
	if (!ini->exists("ZealColors", "Color20")) //Other Pet Damage
	{
		if (color_buttons.count(20))
			color_buttons[20]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0xf0); //Default White
	}
	if (!ini->exists("ZealColors", "Color21")) //My Pet Say
	{
		if (color_buttons.count(19))
			color_buttons[21]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0xf0); //Default White
	}
	if (!ini->exists("ZealColors", "Color22")) //Other Pet Say
	{
		if (color_buttons.count(20))
			color_buttons[22]->TextColor.ARGB = D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0xf0); //Default White
	}

	for (auto& [index, btn] : color_buttons)
	{
		if (ini->exists("ZealColors", "Color" + std::to_string(index)))
			btn->TextColor.ARGB = ini->getValue<DWORD>("ZealColors", "Color" + std::to_string(index));
	}
}

void ui_options::InitUI()
{
	if (wnd)
		Zeal::EqGame::print_chat("Warning: Init out of sync for ui_options");

	static const char* xml_file = "./uifiles/zeal/EQUI_ZealOptions.xml";
	if (!wnd && ui && std::filesystem::exists(xml_file))
		wnd = ui->CreateSidlScreenWnd("ZealOptions");
	
	if (!wnd)
	{
		Zeal::EqGame::print_chat("Error: Failed to load %s", xml_file);
		return;
	}

	wnd->IsVisible = false;  // Redundant but ensure there is a transition to trigger sync.
	wnd->vtbl->WndNotification = WndNotification;
	InitGeneral();
	InitCamera();
	InitMap();
	InitTargetRing();
	InitColors();
	InitNameplate();
	InitFloatingDamage();

	// The option states are synchronized when the wnd is made visible.
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
		return;

	for (int i = 0; i < 100; i++)
	{
		Zeal::EqUI::BasicWnd* btn = ui->AddButtonCallback(wnd, "Zeal_Color" + std::to_string(i), [](Zeal::EqUI::BasicWnd* wnd) { Zeal::EqGame::Windows->ColorPicker->Activate(wnd, wnd->TextColor.ARGB); }, false);
		if (btn)
			color_buttons[i] = btn;
	}
	LoadColors();
}
void ui_options::InitGeneral()
{
	if (!wnd)
		return;
	/*add callbacks when the buttons are pressed in the options window*/
	ui->AddCheckboxCallback(wnd, "Zeal_HideCorpse",				[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->looting_hook->set_hide_looted(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_Cam",					[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->camera_mods->set_smoothing(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_BlueCon",				[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->UseBlueCon.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_Input",					[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->UseZealInput.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_Escape",					[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "Escape", wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_RaidEscapeLock",			[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "EscapeRaidLock", wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_ShowHelm",				[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->helm->ShowHelmEnabled.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_AltContainerTooltips",	[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->tooltips->set_alt_all_containers(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_SpellbookAutoStand",		[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->movement->set_spellbook_autostand(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_RightClickToEquip",      [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->equip_item_hook->Enabled.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_ClassicClasses",		    [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->chat_hook->UseClassicClassNames.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_TellWindows",			[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->tells->SetEnabled(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_TellWindowsHist",		[](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->tells->SetHist(wnd->Checked); });
	ui->AddComboCallback(wnd,	 "Zeal_Timestamps_Combobox",	[](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->chat_hook->TimeStampsStyle.set(value); });
	ui->AddSliderCallback(wnd,	 "Zeal_HoverTimeout_Slider",	[this](Zeal::EqUI::SliderWnd* wnd, int value) {	int val = value * 5; ZealService::get_instance()->tooltips->set_timer(val); ui->SetLabelValue("Zeal_HoverTimeout_Value", "%i ms", val); });
	ui->AddLabel(wnd, "Zeal_HoverTimeout_Value");
	ui->AddLabel(wnd, "Zeal_VersionValue");
}
void ui_options::InitFloatingDamage()
{
	if (!wnd)
		return;
	ui->AddCheckboxCallback(wnd, "Zeal_FloatingDamage", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->floating_damage->enabled.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_FloatingSpells", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->floating_damage->spells.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_FloatingSpellIcons", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->floating_damage->spell_icons.set(wnd->Checked); });

	ui->AddComboCallback(wnd, "Zeal_FloatingFont_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) {
		std::string font_name("");
		if (value >= 0) {
			Zeal::EqUI::CXSTR selected_name;
			wnd->CmbListWnd->GetItemText(&selected_name, value, 0);
			if (selected_name.Data) {
				font_name = std::string(selected_name.Data->Text);
				selected_name.FreeRep();
			}
		}
		ZealService::get_instance()->floating_damage->set_font(font_name);
		});
}
void ui_options::InitCamera()
{
	if (!wnd)
		return;
	ui->AddCheckboxCallback(wnd, "Zeal_Cam_TurnLocked", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->camera_mods->cam_lock.set(wnd->Checked); } );
	ui->AddCheckboxCallback(wnd, "Zeal_UseOldSens", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->camera_mods->use_old_sens.set(wnd->Checked); });
	ui->AddSliderCallback(wnd, "Zeal_PanDelaySlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->pan_delay.set(value * 4);
		ui->SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay.get());
		});
	ui->AddSliderCallback(wnd, "Zeal_FirstPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x.set(GetSensitivityFromSlider(value));
		ui->SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x.get());
		});
	ui->AddSliderCallback(wnd, "Zeal_FirstPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y.set(GetSensitivityFromSlider(value));
		ui->SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y.get());
		});
	ui->AddSliderCallback(wnd, "Zeal_ThirdPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd.set(GetSensitivityFromSlider(value));
		ui->SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd.get());
		});
	ui->AddSliderCallback(wnd, "Zeal_ThirdPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd.set(GetSensitivityFromSlider(value));
		ui->SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd.get());
		});
	ui->AddSliderCallback(wnd, "Zeal_FoVSlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = 45.0f + (static_cast<float>(value) / 100.0f) * 45.0f;
		ZealService::get_instance()->camera_mods->fov.set(val);
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
		return;
	ui->AddCheckboxCallback(wnd, "Zeal_Map", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_enabled(wnd->Checked, true); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapAutoFadeEnable", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_default_to_zlevel_autofade(wnd->Checked, true); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapInteractiveEnable", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_interactive_enable(wnd->Checked, true); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapExternalWindow", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_external_enable(wnd->Checked, true); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapShowRaid", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_show_raid(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_MapShowGrid", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->zone_map->set_show_grid(wnd->Checked); });
	ui->AddComboCallback(wnd, "Zeal_MapShowGroup_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_show_group_mode(value); });
	ui->AddComboCallback(wnd, "Zeal_MapBackground_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_background(value); });
	ui->AddComboCallback(wnd, "Zeal_MapAlignment_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_alignment(value); });
	ui->AddComboCallback(wnd, "Zeal_MapLabels_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_labels_mode(value); });
	ui->AddComboCallback(wnd, "Zeal_MapDataMode_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_map_data_mode(value); });
	ui->AddComboCallback(wnd, "Zeal_MapZoomDefault_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { ZealService::get_instance()->zone_map->set_map_zoom_default_index(value); });

	ui->AddComboCallback(wnd, "Zeal_MapFont_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) {
		std::string font_name("");
		if (value > 0) {  // Note: Assuming first value is the default, which "" selects anyways.
			Zeal::EqUI::CXSTR selected_name;
			wnd->CmbListWnd->GetItemText(&selected_name, value, 0);
			if (selected_name.Data) {
				font_name = std::string(selected_name.Data->Text);
				selected_name.FreeRep();
			}
		}
    	ZealService::get_instance()->zone_map->set_font(font_name);
		});

	ui->AddSliderCallback(wnd, "Zeal_MapZoom_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_zoom(value * 15 + 100);  // Note scale and zoom offset.
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
	ui->AddSliderCallback(wnd, "Zeal_MapFadedZLevelAlpha_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_faded_zlevel_alpha(value);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapNamesLength_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_name_length(value * ZoneMap::kMaxNameLength / 100);
		});
	ui->AddSliderCallback(wnd, "Zeal_MapGridPitch_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->zone_map->set_grid_pitch(value * ZoneMap::kMaxGridPitch / 100);
		});
	ui->AddLabel(wnd, "Zeal_MapZoom_Value");
	ui->AddLabel(wnd, "Zeal_MapPositionSize_Value");
	ui->AddLabel(wnd, "Zeal_MapMarkerSize_Value");
	ui->AddLabel(wnd, "Zeal_MapBackgroundAlpha_Value");
	ui->AddLabel(wnd, "Zeal_MapFadedZLevelAlpha_Value");
	ui->AddLabel(wnd, "Zeal_MapNamesLength_Value");
	ui->AddLabel(wnd, "Zeal_MapGridPitch_Value");
}
void ui_options::InitTargetRing()
{
	if (!wnd)
		return;
	ui->AddLabel(wnd, "Zeal_TargetRingFill_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingSize_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingRotation_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingSegments_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingFlash_Value");
	ui->AddLabel(wnd, "Zeal_TargetRingTransparency_Value");

	ui->AddCheckboxCallback(wnd, "Zeal_TargetRing", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->target_ring->enabled.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_TargetRingAttackIndicator", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->target_ring->attack_indicator.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_TargetRingForward", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->target_ring->rotate_match_heading.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_TargetRingCone", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->target_ring->use_cone.set(wnd->Checked); });
	
	ui->AddComboCallback(wnd, "Zeal_TargetRingTexture_Combobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) {
		if (value >= 0)
		{
			Zeal::EqUI::CXSTR texture_name;
			wnd->CmbListWnd->GetItemText(&texture_name, value, 0);
			if (texture_name.Data)
			{
				ZealService::get_instance()->target_ring->texture_name.set(texture_name.Data->Text);
				texture_name.FreeRep();
			}
		}
		else
		{
			ZealService::get_instance()->target_ring->texture_name.set("None");
		}
	});
	
	ui->AddSliderCallback(wnd, "Zeal_TargetRingFlash_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = ScaleSliderToFloat(value, 0, 5, wnd);
		ZealService::get_instance()->target_ring->flash_speed.set(val);
		ui->SetLabelValue("Zeal_TargetRingFlash_Value", "%.2f", val);
	});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingFill_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = ScaleSliderToFloat(value, 0, 1, wnd);
		ZealService::get_instance()->target_ring->inner_percent.set(val);
		ui->SetLabelValue("Zeal_TargetRingFill_Value", "%.2f", val);
	});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingSize_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		//float val = static_cast<float>(value) / 5.0f;
		float val = ScaleSliderToFloat(value, 0, 20, wnd);
		ZealService::get_instance()->target_ring->outer_size.set(val);
		ui->SetLabelValue("Zeal_TargetRingSize_Value", "%.2f", val);
		});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingRotation_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		//float val = ((static_cast<float>(value)-50.f)/100.f)*2;
		float val = ScaleSliderToFloat(value, -1, 1, wnd);
		ZealService::get_instance()->target_ring->rotation_speed.set(val);
		ui->SetLabelValue("Zeal_TargetRingRotation_Value", "%.2f", val);
		});
	ui->AddSliderCallback(wnd, "Zeal_TargetRingSegments_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->target_ring->num_segments.set(value);
		ui->SetLabelValue("Zeal_TargetRingSegments_Value", "%i", value);
	}, 128);
	ui->AddSliderCallback(wnd, "Zeal_TargetRingTransparency_Slider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = ScaleSliderToFloat(value, 0, 1, wnd);
		ZealService::get_instance()->target_ring->transparency.set(val);
		ui->SetLabelValue("Zeal_TargetRingTransparency_Value", "%.2f", val);
	}, 128);


}

void ui_options::InitNameplate()
{
	if (!wnd)
		return;
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateColors", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_colors.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateConColors", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_con_colors.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateHideSelf", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_hide_self.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateX", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_x.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateHideRaidPets", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_hide_raid_pets.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateCharSelect", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_char_select.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateInlineGuild", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_inline_guild.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateTargetColor", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_target_color.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateTargetMarker", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_target_marker.set(wnd->Checked); });
	ui->AddCheckboxCallback(wnd, "Zeal_NameplateTargetHealth", [](Zeal::EqUI::BasicWnd* wnd) {ZealService::get_instance()->nameplate->setting_target_health.set(wnd->Checked); });
}

void ui_options::UpdateOptions()
{
	if (!wnd)
		return;

	UpdateOptionsCamera();
	UpdateOptionsGeneral();
	UpdateOptionsTargetRing();
	UpdateOptionsNameplate();
	UpdateOptionsMap();
	UpdateOptionsFloatingDamage();
}

void ui_options::UpdateOptionsGeneral()
{
	if (!wnd)
		return;

	ui->SetComboValue("Zeal_Timestamps_Combobox", ZealService::get_instance()->chat_hook->TimeStampsStyle.get());
	ui->SetSliderValue("Zeal_HoverTimeout_Slider", ZealService::get_instance()->tooltips->hover_timeout > 0 ? ZealService::get_instance()->tooltips->hover_timeout / 5 : 0);
	ui->SetLabelValue("Zeal_HoverTimeout_Value", "%d ms", ZealService::get_instance()->tooltips->hover_timeout);
	ui->SetChecked("Zeal_HideCorpse", ZealService::get_instance()->looting_hook->hide_looted);
	ui->SetChecked("Zeal_TellWindows", ZealService::get_instance()->tells->enabled);
	ui->SetChecked("Zeal_TellWindowsHist", ZealService::get_instance()->tells->hist_enabled);
	ui->SetChecked("Zeal_ClassicClasses", ZealService::get_instance()->chat_hook->UseClassicClassNames.get());
	ui->SetLabelValue("Zeal_VersionValue", "%s (%s)", ZEAL_VERSION, ZEAL_BUILD_VERSION);
	ui->SetChecked("Zeal_BlueCon", ZealService::get_instance()->chat_hook->UseBlueCon.get());
	ui->SetChecked("Zeal_Timestamp", ZealService::get_instance()->chat_hook->TimeStampsStyle.get());
	ui->SetChecked("Zeal_Input", ZealService::get_instance()->chat_hook->UseZealInput.get());
	ui->SetChecked("Zeal_Escape", ZealService::get_instance()->ini->getValue<bool>("Zeal", "Escape"));
	ui->SetChecked("Zeal_ShowHelm", ZealService::get_instance()->helm->ShowHelmEnabled.get());
	ui->SetChecked("Zeal_AltContainerTooltips", ZealService::get_instance()->tooltips->all_containers);
	ui->SetChecked("Zeal_SpellbookAutoStand", ZealService::get_instance()->movement->spellbook_autostand);
	ui->SetChecked("Zeal_RightClickToEquip", ZealService::get_instance()->equip_item_hook->Enabled.get());
}
void ui_options::UpdateOptionsCamera()
{
	if (!wnd)
		return;

	ui->SetChecked("Zeal_Cam_TurnLocked", ZealService::get_instance()->camera_mods->cam_lock.get());
	ui->SetSliderValue("Zeal_PanDelaySlider", ZealService::get_instance()->camera_mods->pan_delay.get() > 0.f ? ZealService::get_instance()->camera_mods->pan_delay.get() / 4 : 0.f);
	ui->SetSliderValue("Zeal_ThirdPersonSlider_Y", GetSensitivityForSlider(ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd));
	ui->SetSliderValue("Zeal_ThirdPersonSlider_X", GetSensitivityForSlider(ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd));
	ui->SetSliderValue("Zeal_FirstPersonSlider_Y", GetSensitivityForSlider(ZealService::get_instance()->camera_mods->user_sensitivity_y));
	ui->SetSliderValue("Zeal_FirstPersonSlider_X", GetSensitivityForSlider(ZealService::get_instance()->camera_mods->user_sensitivity_x));
	ui->SetSliderValue("Zeal_FoVSlider", static_cast<int>((ZealService::get_instance()->camera_mods->fov.get() - 45.0f) / 45.0f * 100.0f));
	ui->SetLabelValue("Zeal_FoVValueLabel", "%.0f", ZealService::get_instance()->camera_mods->fov.get());
	ui->SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x.get());
	ui->SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y.get());
	ui->SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd.get());
	ui->SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd.get());
	ui->SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay.get());
	ui->SetChecked("Zeal_Cam", ZealService::get_instance()->camera_mods->enabled.get());
	ui->SetChecked("Zeal_UseOldSens", ZealService::get_instance()->camera_mods->use_old_sens.get());
}
void ui_options::UpdateOptionsTargetRing()
{
	if (!wnd)
		return;

	ui->SetChecked("Zeal_TargetRing", ZealService::get_instance()->target_ring->enabled.get());
	ui->SetChecked("Zeal_TargetRingAttackIndicator", ZealService::get_instance()->target_ring->attack_indicator.get());
	ui->SetChecked("Zeal_TargetRingForward", ZealService::get_instance()->target_ring->rotate_match_heading.get());
	ui->SetChecked("Zeal_TargetRingCone", ZealService::get_instance()->target_ring->use_cone.get());
	ui->SetSliderValue("Zeal_TargetRingFill_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->inner_percent.get(), 0, 1, ui->GetSlider("Zeal_TargetRingFill_Slider")));
	ui->SetSliderValue("Zeal_TargetRingSize_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->outer_size.get(), 0, 20, ui->GetSlider("Zeal_TargetRingSize_Slider")));
	ui->SetSliderValue("Zeal_TargetRingRotation_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->rotation_speed.get(), -1, 1, ui->GetSlider("Zeal_TargetRingSize_Slider")));
	ui->SetSliderValue("Zeal_TargetRingFlash_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->flash_speed.get(), 1, 5, ui->GetSlider("Zeal_TargetRingFlash_Slider")));
	ui->SetSliderValue("Zeal_TargetRingSegments_Slider", static_cast<int>(ZealService::get_instance()->target_ring->num_segments.get()));
	ui->SetSliderValue("Zeal_TargetRingTransparency_Slider", ScaleFloatToSlider(ZealService::get_instance()->target_ring->transparency.get(), 0, 1, ui->GetSlider("Zeal_TargetRingTransparency_Slider")));
	ui->SetLabelValue("Zeal_TargetRingFlash_Value", "%.2f", ZealService::get_instance()->target_ring->flash_speed.get());
	ui->SetLabelValue("Zeal_TargetRingFill_Value", "%.2f", ZealService::get_instance()->target_ring->inner_percent.get());
	ui->SetLabelValue("Zeal_TargetRingSegments_Value", "%i", ZealService::get_instance()->target_ring->num_segments.get());
	ui->SetLabelValue("Zeal_TargetRingRotation_Value", "%.2f", ZealService::get_instance()->target_ring->rotation_speed.get());
	ui->SetLabelValue("Zeal_TargetRingSize_Value", "%.2f", ZealService::get_instance()->target_ring->outer_size.get());
	ui->SetLabelValue("Zeal_TargetRingTransparency_Value", "%.2f", ZealService::get_instance()->target_ring->transparency.get());
	ui->SetComboValue("Zeal_TargetRingTexture_Combobox",
		FindComboIndex("Zeal_TargetRingTexture_Combobox", ZealService::get_instance()->target_ring->texture_name.get()));
}

void ui_options::UpdateOptionsNameplate()
{
	if (!wnd)
		return;

	ui->SetChecked("Zeal_NameplateColors", ZealService::get_instance()->nameplate->setting_colors.get());
	ui->SetChecked("Zeal_NameplateConColors", ZealService::get_instance()->nameplate->setting_con_colors.get());
	ui->SetChecked("Zeal_NameplateHideSelf", ZealService::get_instance()->nameplate->setting_hide_self.get());
	ui->SetChecked("Zeal_NameplateX", ZealService::get_instance()->nameplate->setting_x.get());
	ui->SetChecked("Zeal_NameplateHideRaidPets", ZealService::get_instance()->nameplate->setting_hide_raid_pets.get());
	ui->SetChecked("Zeal_NameplateCharSelect", ZealService::get_instance()->nameplate->setting_char_select.get());
	ui->SetChecked("Zeal_NameplateInlineGuild", ZealService::get_instance()->nameplate->setting_inline_guild.get());
	ui->SetChecked("Zeal_NameplateTargetColor", ZealService::get_instance()->nameplate->setting_target_color.get());
	ui->SetChecked("Zeal_NameplateTargetMarker", ZealService::get_instance()->nameplate->setting_target_marker.get());
	ui->SetChecked("Zeal_NameplateTargetHealth", ZealService::get_instance()->nameplate->setting_target_health.get());
}

void ui_options::UpdateOptionsFloatingDamage()
{
	ui->SetChecked("Zeal_FloatingDamage", ZealService::get_instance()->floating_damage->enabled.get());
	ui->SetChecked("Zeal_FloatingSpells", ZealService::get_instance()->floating_damage->spells.get());
	ui->SetChecked("Zeal_FloatingSpellIcons", ZealService::get_instance()->floating_damage->spell_icons.get());

	std::string current_font = ZealService::get_instance()->floating_damage->get_font();
	if (current_font.empty())
		current_font = FloatingDamage::kUseClientFontString;
	ui->SetComboValue("Zeal_FloatingFont_Combobox",
		FindComboIndex("Zeal_FloatingFont_Combobox", current_font));
}

void ui_options::UpdateOptionsMap()
{
	if (!wnd)
		return;
	
	ui->SetChecked("Zeal_Map", ZealService::get_instance()->zone_map->is_enabled());
	ui->SetChecked("Zeal_MapAutoFadeEnable", ZealService::get_instance()->zone_map->is_default_zlevel_autofade());
	ui->SetChecked("Zeal_MapInteractiveEnable", ZealService::get_instance()->zone_map->is_interactive_enabled());
	ui->SetChecked("Zeal_MapExternalWindow", ZealService::get_instance()->zone_map->is_external_enabled());
	ui->SetChecked("Zeal_MapShowRaid", ZealService::get_instance()->zone_map->is_show_raid_enabled());
	ui->SetChecked("Zeal_MapShowGrid", ZealService::get_instance()->zone_map->is_show_grid_enabled());
	ui->SetComboValue("Zeal_MapShowGroup_Combobox", ZealService::get_instance()->zone_map->get_show_group_mode());
	ui->SetComboValue("Zeal_MapBackground_Combobox", ZealService::get_instance()->zone_map->get_background());
	ui->SetComboValue("Zeal_MapAlignment_Combobox", ZealService::get_instance()->zone_map->get_alignment());
	ui->SetComboValue("Zeal_MapLabels_Combobox", ZealService::get_instance()->zone_map->get_labels_mode());
	ui->SetComboValue("Zeal_MapDataMode_Combobox", ZealService::get_instance()->zone_map->get_map_data_mode());
	ui->SetComboValue("Zeal_MapFont_Combobox",
		FindComboIndex("Zeal_MapFont_Combobox", ZealService::get_instance()->zone_map->get_font()));
	ui->SetComboValue("Zeal_MapZoomDefault_Combobox", ZealService::get_instance()->zone_map->get_map_zoom_default_index());
	ui->SetSliderValue("Zeal_MapZoom_Slider", (ZealService::get_instance()->zone_map->get_zoom() - 100)/15);  // 100 to 1600%
	ui->SetSliderValue("Zeal_MapPositionSize_Slider", ZealService::get_instance()->zone_map->get_position_size());
	ui->SetSliderValue("Zeal_MapMarkerSize_Slider", ZealService::get_instance()->zone_map->get_marker_size());
	ui->SetSliderValue("Zeal_MapBackgroundAlpha_Slider", ZealService::get_instance()->zone_map->get_background_alpha());
	ui->SetSliderValue("Zeal_MapFadedZLevelAlpha_Slider", ZealService::get_instance()->zone_map->get_faded_zlevel_alpha());
	ui->SetSliderValue("Zeal_MapNamesLength_Slider", ZealService::get_instance()->zone_map->get_name_length() * 100 / ZoneMap::kMaxNameLength);
	ui->SetSliderValue("Zeal_MapGridPitch_Slider", ZealService::get_instance()->zone_map->get_grid_pitch() * 100 / ZoneMap::kMaxGridPitch);
	ui->SetLabelValue("Zeal_MapZoom_Value", "%i%%", ZealService::get_instance()->zone_map->get_zoom());
	ui->SetLabelValue("Zeal_MapPositionSize_Value", "%i%%", ZealService::get_instance()->zone_map->get_position_size());
	ui->SetLabelValue("Zeal_MapMarkerSize_Value", "%i%%", ZealService::get_instance()->zone_map->get_marker_size());
	ui->SetLabelValue("Zeal_MapBackgroundAlpha_Value", "%i%%", ZealService::get_instance()->zone_map->get_background_alpha());
	ui->SetLabelValue("Zeal_MapFadedZLevelAlpha_Value", "%i%%", ZealService::get_instance()->zone_map->get_faded_zlevel_alpha());
	ui->SetLabelValue("Zeal_MapNamesLength_Value", "%i", ZealService::get_instance()->zone_map->get_name_length());
	ui->SetLabelValue("Zeal_MapGridPitch_Value", "%i", ZealService::get_instance()->zone_map->get_grid_pitch());
}

void ui_options::RenderUI()
{
	if (!wnd || !Zeal::EqGame::Windows || !Zeal::EqGame::Windows->Options)
		return;

	if (wnd->IsVisible != Zeal::EqGame::Windows->Options->IsVisible) {
		if (Zeal::EqGame::Windows->Options->IsVisible) {
			UpdateDynamicUI();
			UpdateOptions();
		}
		wnd->show(Zeal::EqGame::Windows->Options->IsVisible, false);
	}
}

int ui_options::FindComboIndex(std::string combobox, std::string text_value) {
	if (!wnd)
		return -1;

	Zeal::EqUI::ComboWnd* cmb = (Zeal::EqUI::ComboWnd*)wnd->GetChildItem(combobox.c_str());
	if (!cmb)
		return -1;

	int value = 0;
	for (int value = 0; value < 20; ++value) {
		Zeal::EqUI::CXSTR selected_name;
		cmb->CmbListWnd->GetItemText(&selected_name, value, 0);  // Assumption: value > rows is safe.
		if (!selected_name.Data)
			return -1;  // End of list.

		std::string value_label = std::string(selected_name.Data->Text);
		selected_name.FreeRep();
		if (Zeal::String::compare_insensitive(value_label, text_value))
			return value;
	}
	return -1;
}

void ui_options::UpdateDynamicUI() {
	if (!wnd)
		return;

	Zeal::EqUI::ComboWnd* cmb = (Zeal::EqUI::ComboWnd*)wnd->GetChildItem("Zeal_TargetRingTexture_Combobox");
	if (cmb) {
		std::vector<std::string> textures = ZealService::get_instance()->target_ring->get_available_textures();
		cmb->DeleteAll();
		ZealService::get_instance()->ui->AddListItems(cmb, textures);

		std::string current_texture = ZealService::get_instance()->target_ring->texture_name.get();
		cmb->SetChoice(max(0, FindComboIndex("Zeal_TargetRingTexture_Combobox", current_texture)));
	}

	cmb = (Zeal::EqUI::ComboWnd*)wnd->GetChildItem("Zeal_MapFont_Combobox");
	if (cmb) {
		std::vector<std::string> fonts = ZealService::get_instance()->zone_map->get_available_fonts();
		cmb->DeleteAll();
		ZealService::get_instance()->ui->AddListItems(cmb, fonts);

		std::string current_font = ZealService::get_instance()->zone_map->get_font();
		cmb->SetChoice(max(0, FindComboIndex("Zeal_MapFont_Combobox", current_font)));
	}

	cmb = (Zeal::EqUI::ComboWnd*)wnd->GetChildItem("Zeal_FloatingFont_Combobox");
	if (cmb) {
		std::vector<std::string> fonts = ZealService::get_instance()->floating_damage->get_available_fonts();
		cmb->DeleteAll();
		ZealService::get_instance()->ui->AddListItems(cmb, fonts);

		std::string current_font = ZealService::get_instance()->floating_damage->get_font();
		if (current_font.empty())
			current_font = FloatingDamage::kUseClientFontString;
		cmb->SetChoice(max(0, FindComboIndex("Zeal_FloatingFont_Combobox", current_font)));
	}
}

void ui_options::CleanDynamicUI() {
	if (!wnd)
		return;

	std::vector<std::string> box_list = { "Zeal_TargetRingTexture_Combobox", "Zeal_MapFont_Combobox",
											"Zeal_FloatingFont_Combobox"};
	for (const auto& box_name : box_list) {
		Zeal::EqUI::ComboWnd* cmb = (Zeal::EqUI::ComboWnd*)wnd->GetChildItem(box_name.c_str());
		if (cmb)
		{
			cmb->CmbListWnd->SelectedIndex = -1;
			cmb->DeleteAll();
		}
	}
}

void ui_options::CleanUI()
{
	if (!wnd)
		return;

	// We assume that deactivate_ui() was called by the framework already (so not needed here).
	color_buttons.clear();
	CleanDynamicUI();  // The destructor below may handle these children, but just in case clean up.
	ZealService::get_instance()->ui->DestroySidlScreenWnd(wnd);
	wnd = nullptr;
}

void ui_options::Deactivate()
{
	if (!wnd)
		return;

	wnd->show(0, false);
}

ui_options::ui_options(ZealService* zeal, IO_ini* ini, ui_manager* mgr) : ui(mgr)
{
	wnd = nullptr;
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { RenderUI(); }, callback_type::RenderUI);
	zeal->callbacks->AddGeneric([this]() { Deactivate(); }, callback_type::DeactivateUI);
	ui->AddXmlInclude("EQUI_ZealOptions.xml");
}
ui_options::~ui_options()
{
}

