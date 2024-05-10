#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class tooltip
{
public:
	void set_timer(int hover_timeout);
	void set_alt_all_containers(bool enabled);
	bool all_containers = false;
	int hover_timeout = 500;
	tooltip(class ZealService* pHookWrapper, class IO_ini* ini);
	~tooltip();
private:
	void LoadSettings(class IO_ini* ini);
};
