#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_bank
{
public:
	void change();
	void InitUI();
	ui_bank(class ZealService* zeal, class IO_ini* ini);
	~ui_bank();
private:
};