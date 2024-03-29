#include "UIOptions.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

static int __fastcall CheckboxClick(Zeal::EqUI::BasicWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	UIOptions* ui = ZealService::get_instance()->ui.get();
	int rval = ZealService::get_instance()->hooks->hook_map["CheckboxClick"]->original(CheckboxClick)(pWnd, unused, pt, flag);
	if (ui->checkbox_callbacks.count(pWnd) > 0)
		ui->checkbox_callbacks[pWnd](pWnd);
	return rval;
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

void UIOptions::AddSliderCallback(std::string name, std::function<void(Zeal::EqUI::SliderWnd*)> callback)
{
	if (Zeal::EqGame::Windows->Options)
	{
		Zeal::EqUI::SliderWnd* btn = (Zeal::EqUI::SliderWnd*)Zeal::EqGame::Windows->Options->GetChildItem(name);
		if (btn)
		{
			slider_callbacks[btn] = callback;
			slider_names[name] = btn;
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

	AddSliderCallback("Zeal_PanDelaySlider", [](Zeal::EqUI::SliderWnd* wnd) { ZealService::get_instance()->chat_hook->set_input(wnd->Checked); });
	AddLabel("Zeal_PanDelayValueLabel");

	/*set the current states*/
	SetChecked("Zeal_HideCorpse", ZealService::get_instance()->looting_hook->hide_looted);
	SetChecked("Zeal_Cam", ZealService::get_instance()->camera_mods->enabled);
	SetChecked("Zeal_BlueCon", ZealService::get_instance()->chat_hook->bluecon);
	SetChecked("Zeal_Timestamp", ZealService::get_instance()->chat_hook->timestamps);
	SetChecked("Zeal_Input", ZealService::get_instance()->chat_hook->zealinput);


}


UIOptions::UIOptions(ZealService* zeal, IO_ini* ini)
{
	//InitUI(); /*for testing only must be in game before its loaded or you will crash*/
	zeal->hooks->Add("CheckboxClick", 0x5c3480, CheckboxClick, hook_type_detour); //add extra prints for new loot types
	zeal->main_loop_hook->add_callback([this]() { InitUI(); }, callback_fn::InitUI);
}
UIOptions::~UIOptions()
{
}

