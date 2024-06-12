#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class chat
{
public:
	int timestamps = 0;
	bool bluecon = false;
	bool zealinput = false;
	bool uniquenames = false;
	void set_input_color(Zeal::EqUI::ARGBCOLOR col);
	void set_bluecon(bool val);
	void set_timestamp(int val);
	void set_input(bool val);
	chat(class ZealService* pHookWrapper, class IO_ini* ini);
	~chat();
private:
	void LoadSettings(class IO_ini* ini);
};
