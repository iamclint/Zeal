#pragma once
#include "hook_wrapper.h"
#include "memory.h"
class AutoStand
{
public:
	void handle_movement_binds(int cmd, bool key_down);
	void handle_spellcast_binds(int cmd);
	AutoStand(class ZealService* zeal);
	~AutoStand();
};

