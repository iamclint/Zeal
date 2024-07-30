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

void ui_manager::AddCheckboxCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback)
{
	if (wnd)
	{
		Zeal::EqUI::BasicWnd* btn = wnd->GetChildItem(name);
		if (btn)
		{
			checkbox_callbacks[btn] = callback;
			checkbox_names[name] = btn;
		}
	}
}

void ui_manager::AddSliderCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback)
{
	if (wnd)
	{
		Zeal::EqUI::SliderWnd* btn = (Zeal::EqUI::SliderWnd*)wnd->GetChildItem(name);
		if (btn)
		{
			slider_callbacks[btn] = callback;
			slider_names[name] = btn;
			btn->max_val = 100;
		}
	}
}

void ui_manager::AddComboCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback)
{
	if (wnd)
	{
		Zeal::EqUI::BasicWnd* btn = (Zeal::EqUI::BasicWnd*)wnd->GetChildItem(name);
		if (btn)
		{
			combo_callbacks[btn] = callback;
			combo_names[name] = btn;
		}
	}
}

void ui_manager::AddLabel(Zeal::EqUI::BasicWnd* wnd, std::string name)
{
	if (wnd)
	{
		Zeal::EqUI::BasicWnd* btn = wnd->GetChildItem(name);
		if (btn)
		{
			label_names[name] = btn;
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
void ui_manager::AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::vector<std::string>>data)
{

	for (int row = 0; auto& current_row : data)
	{
		
		int x = wnd->AddString("");
		for (int col = 0; auto& current_col : current_row)
		{
			wnd->SetItemText(current_col, row, col);
			col++;
		}
		wnd->SetItemData(row);
		row++;
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


void ui_manager::CleanUI()
{
	Zeal::EqGame::print_debug("Clean UI UIMANAGER");
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
//Zeal::EqUI::BasicWnd* ui_manager::GetChild(Zeal::EqUI::BasicWnd* parent, std::string name) 
//{
//	return WindowChildren[parent][name];
//}
//
//static Zeal::EqUI::BasicWnd* __fastcall CreateXWndFromTemplate_hook(int sidlmgr, int unused, Zeal::EqUI::BasicWnd* parent, Zeal::EqUI::ControlTemplate* control_template)
//{
//	Zeal::EqUI::BasicWnd* rval = ZealService::get_instance()->hooks->hook_map["CreateXWndFromTemplate"]->original(CreateXWndFromTemplate_hook)(sidlmgr, unused, parent, control_template);
//	ui_manager* ui = ZealService::get_instance()->ui.get();
//	if (control_template && control_template->Item)
//		ui->WindowChildren[parent][control_template->Item->Text] = rval;
//	return rval;
//}

ui_manager::ui_manager(ZealService* zeal, IO_ini* ini)
{
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { init_ui(); }, callback_type::InitUI);

	bank = std::make_shared<ui_bank>(zeal, ini, this);
	options = std::make_shared<ui_options>(zeal, ini, this);
	loot = std::make_shared<ui_loot>(zeal, ini, this);
	//guild = std::make_shared<ui_guild>(zeal, ini, this);
	raid = std::make_shared<ui_raid>(zeal, ini, this);
	hotbutton = std::make_shared<ui_hotbutton>(zeal, ini, this);

//	zeal->hooks->Add("CreateXWndFromTemplate", 0x59bc40, CreateXWndFromTemplate_hook, hook_type_detour);
	zeal->hooks->Add("CheckboxClick", 0x5c3480, CheckboxClick_hook, hook_type_detour);
	zeal->hooks->Add("SetSliderValue", 0x5a6c70, SetSliderValue_hook, hook_type_detour);
	zeal->hooks->Add("SetComboValue", 0x579af0, SetComboValue_hook, hook_type_detour);
}

