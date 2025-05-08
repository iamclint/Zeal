#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class ui_raid
{
public:
	ui_raid(class ZealService* zeal, class UIManager* mgr);
	~ui_raid();
private:
	void InitUI();
	void CleanUI();
	void LoadSettings(class IO_ini* ini);
	UIManager* ui;
};

