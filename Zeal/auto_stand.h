#pragma once
#include "hook_wrapper.h"
#include "memory.h"
class AutoStand
{
public:
	void handle_binds(int cmd, bool key_down);
	AutoStand(class ZealService* zeal);
	~AutoStand();
};

