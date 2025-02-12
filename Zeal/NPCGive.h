#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include "ZealSettings.h"

class NPCGive
{
public:
	NPCGive(class ZealService* pHookWrapper, class IO_ini* ini);
	~NPCGive();
	void HandleItemPickup();  // For internal callback use only.

	ZealSetting<bool> setting_enable_give = { false, "SingleClick", "EnableGive", false };

private:
	bool wait_cursor_item=false;
	void tick();
};

