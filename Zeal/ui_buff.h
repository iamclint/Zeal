#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include "ZealSettings.h"

struct TickTime {
	int hours;
	int minutes;
	int seconds;
};


class ui_buff
{
public:
	ui_buff(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_buff();
	ZealSetting<bool> BuffTimers = { true, "Zeal", "Bufftimers", false };
private:

	void InitUI();
	void Deactivate();
	void CleanUI();
	ui_manager* ui;
};

