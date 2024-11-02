#include "ui_inputdialog.h"
#include "Zeal.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
#include <algorithm>

void ui_inputdialog::PrintUIError()
{
	Zeal::EqGame::print_chat("Warning: The zeal ui files are not in place EQUI_ZealInputDialog.xml not found!");
}

bool ui_inputdialog::isVisible()
{
	if (!wnd)
		return false;
	return wnd->IsVisible;
}
void ui_inputdialog::hide()
{
	wnd->show(0, 0);
	wnd->IsVisible = false;
}
bool ui_inputdialog::show(const std::string& title, const std::string& message, const std::string& button1_name, const std::string button2_name, InputDialogCallback button1_callback, InputDialogCallback button2_callback)
{
	if (!wnd || wnd->IsVisible)
		return false;
	button_callbacks = { button1_callback, button2_callback };
	if (button1)
		button1->Text = button1_name;
	if (button2)
		button2->Text = button2_name;
	if (label)
		label->Text = message;
	if (wnd)
		wnd->Text = title;
	wnd->show(0, 1);
	wnd->IsVisible = true;
	if (input)
	{
		wnd->BringToFront();
		input->SetText("");
		//Zeal::EqGame::print_chat("0x%x", Zeal::EqGame::Windows->ChatManager->ChatWindows[0]->edit->vtbl->OnSetFocus);
		input->SetFocus();
	}
	return true;
}

int __fastcall IDWndNotification(Zeal::EqUI::BasicWnd* wnd, int unused, Zeal::EqUI::BasicWnd* sender, int message, int data)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	if (sender == ui->inputDialog->button1 || message == 0x6) //message 6 = enter key
	{
		if (ui->inputDialog->button_callbacks.first)
			ui->inputDialog->button_callbacks.first(ui->inputDialog->input->Text.CastToCharPtr());
		ui->inputDialog->hide();
		return 0;
	}
	if (sender == ui->inputDialog->button2)
	{
		if (ui->inputDialog->button_callbacks.second)
			ui->inputDialog->button_callbacks.second(ui->inputDialog->input->Text.CastToCharPtr());
		ui->inputDialog->hide();
		return 0;
	}
	return reinterpret_cast<int(__thiscall*)(Zeal::EqUI::BasicWnd * wnd, Zeal::EqUI::BasicWnd * sender, int message, int data)>(0x56e920)(wnd, sender, message, data);
}

void ui_inputdialog::CleanUI()
{
	if (wnd)
	{
		wnd->show(0, 0);
		wnd->Deconstruct();
		wnd = nullptr;
	}
}
void ui_inputdialog::Deactivate()
{
	if (wnd)
	{
		wnd->show(0, 0);
	}
}

void ui_inputdialog::InitUI()
{
	if (!wnd)
	{
		if (std::filesystem::exists("./uifiles/zeal/EQUI_ZealInputDialog.xml"))
		{
			wnd = ui->CreateSidlScreenWnd("ZealInputDialog");
			wnd->vtbl->WndNotification = IDWndNotification;
			button1 = wnd->GetChildItem("ZealDialogButton1");
			button2 = wnd->GetChildItem("ZealDialogButton2");
			label = wnd->GetChildItem("ZealDialogMessage");
			input = (Zeal::EqUI::EditWnd*)wnd->GetChildItem("ZealDialogInput");
		}
		else
		{
			PrintUIError();
			return;
		}
	}
}

ui_inputdialog::ui_inputdialog(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->commands_hook->Add("/testdialog", {}, "test",
		[this, mgr](std::vector<std::string>& args) {
			if (wnd)
			{
				show("Spell Sets", "Enter a name for this spellset", "Save", "Cancel", [](std::string input_data) { Zeal::EqGame::print_chat(input_data); }, nullptr);
			}
			return true;
		});
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { Deactivate(); }, callback_type::DeactivateUI);

	ui->AddXmlInclude("EQUI_ZealInputDialog.xml");
}
ui_inputdialog::~ui_inputdialog()
{


}
