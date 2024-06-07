#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"

class TargetRing
{
public:
	void callback_render();
	void set_enabled(bool enable);
	bool enabled;
	TargetRing(class ZealService* zeal, class IO_ini* ini);
	~TargetRing();
private:
	LONG color = 0xFF00FFFF;
};


