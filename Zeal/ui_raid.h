#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class ui_raid
{
public:
	ui_raid(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_raid();
private:
	void InitUI();
	void CleanUI();
	void LoadSettings(class IO_ini* ini);
	ui_manager* ui;
};

