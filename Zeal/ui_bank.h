#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_bank
{
public:
	void change();
	ui_bank(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_bank();
private:
	void InitUI();
	ui_manager* ui;
};