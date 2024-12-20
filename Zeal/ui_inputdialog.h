#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
using InputDialogCallback = std::function<void(std::string input)>;
class ui_inputdialog
{
public:
	bool show(const std::string& title, const std::string& message, const std::string& button1, const std::string button2, InputDialogCallback button1_callback, InputDialogCallback button2_callback, bool show_input_field = true);
	void hide();
	bool isVisible();
	std::pair<InputDialogCallback, InputDialogCallback> button_callbacks;
	Zeal::EqUI::BasicWnd* button1;
	Zeal::EqUI::BasicWnd* button2;
	Zeal::EqUI::BasicWnd* label;
	Zeal::EqUI::EditWnd* input;
	
	ui_inputdialog(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_inputdialog();
private:

	Zeal::EqUI::EQWND* wnd = nullptr;
	void InitUI();
	void Deactivate();
	void CleanUI();
	ui_manager* ui;
};

