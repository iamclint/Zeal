#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class ui_guild
{
public:
	Zeal::EqUI::BasicWnd* guild=nullptr;
	Zeal::EqUI::ListWnd* members=nullptr;
	ui_guild(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_guild();
private:
	void InitUI();
	void CleanUI();
	void LoadSettings(class IO_ini* ini);
	ui_manager* ui;
};

