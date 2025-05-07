#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "ZealSettings.h"

class Tooltip
{
public:
	void set_timer(int hover_timeout);
	void set_alt_all_containers(bool enabled);
	ZealSetting<bool> all_containers = { false, "Zeal", "alt_all_containers", false };
	ZealSetting<int> hover_timeout = { 500, "Zeal", "TooltipTime", false };
	Tooltip(class ZealService* pHookWrapper);
	~Tooltip();
private:
	void LoadSettings(class IO_ini* ini);
};
