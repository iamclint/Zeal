#include "ui_options.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

static int __fastcall CheckboxClick_hook(Zeal::EqUI::BasicWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	ui_options* ui = ZealService::get_instance()->ui->options.get();
	int rval = ZealService::get_instance()->hooks->hook_map["CheckboxClick"]->original(CheckboxClick_hook)(pWnd, unused, pt, flag);
	if (ui->checkbox_callbacks.count(pWnd) > 0)
		ui->checkbox_callbacks[pWnd](pWnd);
	return rval;
}
static void __fastcall SetSliderValue_hook(Zeal::EqUI::SliderWnd* pWnd, int unused, int value)
{
	ui_options* ui = ZealService::get_instance()->ui->options.get();
	ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(pWnd, unused, value);
	
	if (value < 0)
		value = 0;
	if (value > 100)
		value = 100;
	if (ui->slider_callbacks.count(pWnd) > 0)
		ui->slider_callbacks[pWnd](pWnd, value);
}
static void __fastcall SetComboValue_hook(Zeal::EqUI::BasicWnd* pWnd, int unused, int value)
{
	ui_options* ui = ZealService::get_instance()->ui->options.get();
	ZealService::get_instance()->hooks->hook_map["SetComboValue"]->original(SetComboValue_hook)(pWnd, unused, value);
	if (ui->combo_callbacks.count(pWnd) > 0)
		ui->combo_callbacks[pWnd](pWnd, value);
	else if(ui->combo_callbacks.count(pWnd->ParentWnd) > 0)
		ui->combo_callbacks[pWnd->ParentWnd](pWnd->ParentWnd, value);
}

void ui_options::AddCheckboxCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback)
{
	if (Zeal::EqGame::Windows->Options)
	{
		Zeal::EqUI::BasicWnd* btn = Zeal::EqGame::Windows->Options->GetChildItem(name);
		if (btn)
		{
			checkbox_callbacks[btn] = callback;
			checkbox_names[name] = btn;
		}
		else
		{
			Zeal::EqGame::print_chat("Checkbox %s not found", name.c_str());
		}
	}
}

void ui_options::AddSliderCallback(std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback)
{
	if (Zeal::EqGame::Windows->Options)
	{
		Zeal::EqUI::SliderWnd* btn = (Zeal::EqUI::SliderWnd*)Zeal::EqGame::Windows->Options->GetChildItem(name);
		if (btn)
		{
			slider_callbacks[btn] = callback;
			slider_names[name] = btn;
			btn->max_val = 100;
		}
		else
		{
			Zeal::EqGame::print_chat("Slider %s not found", name.c_str());
		}
	}
}

void ui_options::AddComboCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback)
{
	if (Zeal::EqGame::Windows->Options)
	{
		Zeal::EqUI::BasicWnd* btn = (Zeal::EqUI::BasicWnd*)Zeal::EqGame::Windows->Options->GetChildItem(name);
		if (btn)
		{
			combo_callbacks[btn] = callback;
			combo_names[name] = btn;
		}
		else
		{
			Zeal::EqGame::print_chat("Combo %s not found", name.c_str());
		}
	}
}

void ui_options::AddLabel(std::string name)
{
	if (Zeal::EqGame::Windows->Options)
	{
		Zeal::EqUI::SliderWnd* btn = (Zeal::EqUI::SliderWnd*)Zeal::EqGame::Windows->Options->GetChildItem(name);
		if (btn)
		{
			label_names[name] = btn;
		}
		else
		{
			Zeal::EqGame::print_chat("Label %s not found", name.c_str());
		}
	}
}

void ui_options::SetSliderValue(std::string name, int value)
{
	if (slider_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(slider_names[name], 0, value);
	}
}
void ui_options::SetSliderValue(std::string name, float value)
{
	if (slider_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(slider_names[name], 0, static_cast<int>(value));
	}
}
void ui_options::SetChecked(std::string name, bool checked)
{
	if (checkbox_names.count(name) > 0)
		checkbox_names[name]->Checked = checked;
}

void ui_options::SetLabelValue(std::string name, const char* format, ...)
{
	va_list argptr;
	char buffer[512];
	va_start(argptr, format);
	vsnprintf(buffer, 511, format, argptr);
	va_end(argptr);
	if (label_names.count(name) > 0)
	{
		Zeal::EqGame::EqGameInternal::CXStr_PrintString(&label_names[name]->Text, buffer);
	}
}

void ui_options::SetComboValue(std::string name, int value)
{
	if (combo_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetComboValue"]->original(SetComboValue_hook)(combo_names[name]->FirstChildWnd, 0, value);
	}
}

void ui_options::InitUI()
{
	/*add callbacks when the buttons are pressed in the options window*/
	AddCheckboxCallback("Zeal_HideCorpse", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->looting_hook->set_hide_looted(wnd->Checked); });
	AddCheckboxCallback("Zeal_Cam", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->camera_mods->set_smoothing(wnd->Checked); });
	AddCheckboxCallback("Zeal_BlueCon", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_bluecon(wnd->Checked); });
	AddCheckboxCallback("Zeal_Timestamp", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_timestamp(wnd->Checked); });
	AddCheckboxCallback("Zeal_Input", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_input(wnd->Checked); }); 
	AddCheckboxCallback("Zeal_Escape", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "Escape", wnd->Checked); });
	
	AddCheckboxCallback("Zeal_ShowHelm", [](Zeal::EqUI::BasicWnd* wnd) { Zeal::EqGame::print_chat("Show helm toggle"); });
	AddSliderCallback("Zeal_PanDelaySlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->set_pan_delay(value*4); 
		SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
	});
	AddSliderCallback("Zeal_FirstPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x);
	});
	AddSliderCallback("Zeal_FirstPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y);
	});
	AddSliderCallback("Zeal_ThirdPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
	});
	AddSliderCallback("Zeal_ThirdPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
	});
	AddSliderCallback("Zeal_FoVSlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		int val = 45.0f + (static_cast<float>(value) / 100.0f) * 45.0f;
		ZealService::get_instance()->camera_mods->set_fov(val);

		// Update the label with the remapped value
		SetLabelValue("Zeal_FoVValueLabel", "%i", val);
	});
	AddLabel("Zeal_PanDelayValueLabel");
	AddLabel("Zeal_FirstPersonLabel_X");
	AddLabel("Zeal_FirstPersonLabel_Y");
	AddLabel("Zeal_ThirdPersonLabel_X");
	AddLabel("Zeal_ThirdPersonLabel_Y");
	AddLabel("Zeal_FoVValueLabel");

	//AddComboCallback("Zeal_HideCorpseCombobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { Zeal::EqGame::print_chat("Combo set to %i", value); });
	/*set the current states*/
	UpdateOptions();
}


void ui_options::CleanUI()
{
	combo_names.clear();
	combo_callbacks.clear();
	checkbox_names.clear();
	checkbox_callbacks.clear();
	slider_names.clear();
	slider_callbacks.clear();
	label_names.clear();
}


void ui_options::UpdateOptions()
{
	SetComboValue("Zeal_HideCorpseCombobox", 2);
	SetSliderValue("Zeal_PanDelaySlider", ZealService::get_instance()->camera_mods->pan_delay > 0.f ? ZealService::get_instance()->camera_mods->pan_delay / 4 : 0.f);
	SetSliderValue("Zeal_ThirdPersonSlider_Y", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd * 50 : 0.f);
	SetSliderValue("Zeal_ThirdPersonSlider_X", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd * 50 : 0.f);
	SetSliderValue("Zeal_FirstPersonSlider_Y", ZealService::get_instance()->camera_mods->user_sensitivity_y > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_y * 50 : 0.f);
	SetSliderValue("Zeal_FirstPersonSlider_X", ZealService::get_instance()->camera_mods->user_sensitivity_x > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_x * 50 : 0.f);
	SetSliderValue("Zeal_FoVSlider", static_cast<int>((ZealService::get_instance()->camera_mods->fov - 45.0f) / 45.0f * 100.0f));
	SetLabelValue("Zeal_FoVValueLabel", "%i", ZealService::get_instance()->camera_mods->fov);
	SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x);
	SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y);
	SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
	SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
	SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
	SetChecked("Zeal_HideCorpse", ZealService::get_instance()->looting_hook->hide_looted);
	SetChecked("Zeal_Cam", ZealService::get_instance()->camera_mods->enabled);
	SetChecked("Zeal_BlueCon", ZealService::get_instance()->chat_hook->bluecon);
	SetChecked("Zeal_Timestamp", ZealService::get_instance()->chat_hook->timestamps);
	SetChecked("Zeal_Input", ZealService::get_instance()->chat_hook->zealinput);
	SetChecked("Zeal_Escape", ZealService::get_instance()->ini->getValue<bool>("Zeal", "Escape"));
}



ui_options::ui_options(ZealService* zeal, IO_ini* ini)
{
	zeal->hooks->Add("CheckboxClick", 0x5c3480, CheckboxClick_hook, hook_type_detour); 
	zeal->hooks->Add("SetSliderValue", 0x5a6c70, SetSliderValue_hook, hook_type_detour);
	zeal->hooks->Add("SetComboValue", 0x579af0, SetComboValue_hook, hook_type_detour);
	zeal->callbacks->add_generic([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->add_generic([this]() { InitUI(); }, callback_type::InitUI);
	//if (Zeal::EqGame::is_in_game()) InitUI();
}
ui_options::~ui_options()
{
}

