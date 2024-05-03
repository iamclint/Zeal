#include "ui_manager.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
static int __fastcall CheckboxClick_hook(Zeal::EqUI::BasicWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	int rval = ZealService::get_instance()->hooks->hook_map["CheckboxClick"]->original(CheckboxClick_hook)(pWnd, unused, pt, flag);
	if (ui->checkbox_callbacks.count(pWnd) > 0)
		ui->checkbox_callbacks[pWnd](pWnd);
	return rval;
}
static void __fastcall SetSliderValue_hook(Zeal::EqUI::SliderWnd* pWnd, int unused, int value)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
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
	ui_manager* ui = ZealService::get_instance()->ui.get();
	ZealService::get_instance()->hooks->hook_map["SetComboValue"]->original(SetComboValue_hook)(pWnd, unused, value);
	if (ui->combo_callbacks.count(pWnd) > 0)
		ui->combo_callbacks[pWnd](pWnd, value);
	else if (ui->combo_callbacks.count(pWnd->ParentWnd) > 0)
		ui->combo_callbacks[pWnd->ParentWnd](pWnd->ParentWnd, value);
}

void ui_manager::AddCheckboxCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback)
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

void ui_manager::AddSliderCallback(std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback)
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

void ui_manager::AddComboCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback)
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

void ui_manager::AddLabel(std::string name)
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

void ui_manager::SetSliderValue(std::string name, int value)
{
	if (slider_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(slider_names[name], 0, value);
	}
}
void ui_manager::SetSliderValue(std::string name, float value)
{
	if (slider_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(slider_names[name], 0, static_cast<int>(value));
	}
}
void ui_manager::SetChecked(std::string name, bool checked)
{
	if (checkbox_names.count(name) > 0)
		checkbox_names[name]->Checked = checked;
}

void ui_manager::SetLabelValue(std::string name, const char* format, ...)
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

void ui_manager::SetComboValue(std::string name, int value)
{
	if (combo_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetComboValue"]->original(SetComboValue_hook)(combo_names[name]->FirstChildWnd, 0, value);
	}
}


void ui_manager::clean_ui()
{
	combo_names.clear();
	combo_callbacks.clear();
	checkbox_names.clear();
	checkbox_callbacks.clear();
	slider_names.clear();
	slider_callbacks.clear();
	label_names.clear();
}

void ui_manager::init_ui()
{

}

ui_manager::ui_manager(ZealService* zeal, IO_ini* ini)
{
	zeal->callbacks->add_generic([this]() { clean_ui(); }, callback_type::CleanUI);
	zeal->callbacks->add_generic([this]() { init_ui(); }, callback_type::InitUI);

	bank = std::make_shared<ui_bank>(zeal, ini, this);
	options = std::make_shared<ui_options>(zeal, ini, this);
	loot = std::make_shared<ui_loot>(zeal, ini, this);
	guild = std::make_shared<ui_guild>(zeal, ini, this);
	raid = std::make_shared<ui_raid>(zeal, ini, this);

	zeal->hooks->Add("CheckboxClick", 0x5c3480, CheckboxClick_hook, hook_type_detour);
	zeal->hooks->Add("SetSliderValue", 0x5a6c70, SetSliderValue_hook, hook_type_detour);
	zeal->hooks->Add("SetComboValue", 0x579af0, SetComboValue_hook, hook_type_detour);
}

