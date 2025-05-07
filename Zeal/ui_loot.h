#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_loot
{
public:
	ui_loot(class ZealService* zeal, class UIManager* mgr);
	~ui_loot();
private:
	void InitUI();
	UIManager* ui;
};
