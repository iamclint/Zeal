#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class NPCGive
{
public:
	NPCGive(class ZealService* pHookWrapper, class IO_ini* ini);
	~NPCGive();
	bool enabled = true;
	void HandleItemPickup();
private:
	bool wait_cursor_item=false;
	void tick();
};

