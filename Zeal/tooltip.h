#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class tooltip
{
public:
	void set_timer(int time);
	tooltip(class ZealService* pHookWrapper, class IO_ini* ini);
	~tooltip();
private:
	int time = 500;
	void LoadSettings(class IO_ini* ini);
};
