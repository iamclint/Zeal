#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class chat
{
public:
	bool timestamps = false;
	bool bluecon = false;
	bool zealinput = false;
	void set_input_color(Zeal::EqUI::ARGBCOLOR col);
	chat(class ZealService* pHookWrapper, class IO_ini* ini);
	~chat();
private:
	void LoadSettings(class IO_ini* ini);
};
