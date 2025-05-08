#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include "ZealSettings.h"

class NPCGive
{
public:
	NPCGive(class ZealService* pHookWrapper);
	~NPCGive();
	void HandleItemPickup();  // For internal callback use only.

	ZealSetting<bool> setting_enable_give = { false, "SingleClick", "EnableGive", false };

private:
	bool wait_cursor_item=false;
	int bag_index = 0;  // /singleclick bag <#> with 1-8 valid values. 0 disables.
	void tick();
};

