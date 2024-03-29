#include "UIOptions.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

static int __fastcall CheckboxClick_hook(Zeal::EqUI::BasicWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	UIOptions* ui = ZealService::get_instance()->ui.get();
	int rval = ZealService::get_instance()->hooks->hook_map["CheckboxClick"]->original(CheckboxClick_hook)(pWnd, unused, pt, flag);
	if (ui->checkbox_callbacks.count(pWnd) > 0)
		ui->checkbox_callbacks[pWnd](pWnd);
	return rval;
}
static void __fastcall SetSliderValue_hook(Zeal::EqUI::SliderWnd* pWnd, int unused, int value)
{
	UIOptions* ui = ZealService::get_instance()->ui.get();
	ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(pWnd, unused, value);
	if (ui->slider_callbacks.count(pWnd) > 0)
		ui->slider_callbacks[pWnd](pWnd, value);
}


void UIOptions::AddCheckboxCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback)
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

void UIOptions::AddSliderCallback(std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback)
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

void UIOptions::AddComboCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback)
{
	if (Zeal::EqGame::Windows->Options)
	{
		Zeal::EqUI::SliderWnd* btn = (Zeal::EqUI::SliderWnd*)Zeal::EqGame::Windows->Options->GetChildItem(name);
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

void UIOptions::AddLabel(std::string name)
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

void UIOptions::SetSliderValue(std::string name, int value)
{
	if (slider_names.count(name) > 0)
	{
		SetSliderValue_hook(slider_names[name], 0, value);
	}
}
void UIOptions::SetChecked(std::string name, bool checked)
{
	if (checkbox_names.count(name) > 0)
		checkbox_names[name]->Checked = checked;
}

void UIOptions::InitUI()
{
	/*add callbacks when the buttons are pressed in the options window*/
	AddCheckboxCallback("Zeal_HideCorpse", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->looting_hook->set_hide_looted(wnd->Checked); });
	AddCheckboxCallback("Zeal_Cam", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->camera_mods->set_smoothing(wnd->Checked); });
	AddCheckboxCallback("Zeal_BlueCon", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_bluecon(wnd->Checked); });
	AddCheckboxCallback("Zeal_Timestamp", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_timestamp(wnd->Checked); });
	AddCheckboxCallback("Zeal_Input", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_input(wnd->Checked); }); 

	AddSliderCallback("Zeal_PanDelaySlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		if (value < 0)
			value = 0;
		ZealService::get_instance()->camera_mods->set_pan_delay(value*4); 
		Zeal::EqUI::BasicWnd* lbl = label_names["Zeal_PanDelayValueLabel"];
		if (lbl)
			Zeal::EqGame::CXStr_PrintString(&lbl->Text, "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
	});
	AddLabel("Zeal_PanDelayValueLabel");


	/*set the current states*/
	SetSliderValue("Zeal_PanDelaySlider", ZealService::get_instance()->camera_mods->pan_delay>0?ZealService::get_instance()->camera_mods->pan_delay / 4:0);
	SetChecked("Zeal_HideCorpse", ZealService::get_instance()->looting_hook->hide_looted);
	SetChecked("Zeal_Cam", ZealService::get_instance()->camera_mods->enabled);
	SetChecked("Zeal_BlueCon", ZealService::get_instance()->chat_hook->bluecon);
	SetChecked("Zeal_Timestamp", ZealService::get_instance()->chat_hook->timestamps);
	SetChecked("Zeal_Input", ZealService::get_instance()->chat_hook->zealinput);


}


UIOptions::UIOptions(ZealService* zeal, IO_ini* ini)
{
	zeal->hooks->Add("CheckboxClick", 0x5c3480, CheckboxClick_hook, hook_type_detour); 
	zeal->hooks->Add("SetSliderValue", 0x5a6c70, SetSliderValue_hook, hook_type_detour);
	zeal->main_loop_hook->add_callback([this]() { InitUI(); }, callback_fn::InitUI);
	if (Zeal::EqGame::is_in_game()) InitUI();
}
UIOptions::~UIOptions()
{
}

