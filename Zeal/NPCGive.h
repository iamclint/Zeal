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
	void HandleItemInCursor(); // For internal callback use only.
	void HandleItemPickup(int from_slot);  // For internal callback use only.
	void ClearItem();  // Reset the waiting for item flag.

	ZealSetting<bool> setting_enable_give = { false, "SingleClick", "EnableGive", false };

private:
	Zeal::EqStructures::EQITEMINFOBASE* wait_cursor_item = nullptr;  // Auto-move item on cursor.
	int bag_index = 0;  // /singleclick bag <#> with 1-8 valid values. 0 disables.
	void tick();
};

