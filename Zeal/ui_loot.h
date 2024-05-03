#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_loot
{
public:
	ui_loot(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_loot();
private:
	void InitUI();
	ui_manager* ui;
};
