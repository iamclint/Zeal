#pragma once
#include "hook_wrapper.h"
#include "EqUI.h"
#include "ZealSettings.h"
#include <stdint.h>

class Tick
{
public:
	DWORD GetTimeUntilTick();
	DWORD GetTickGauge(struct Zeal::EqUI::CXSTR* str);
	Tick(class ZealService* zeal);
	~Tick();

	ZealSetting<bool> ReverseDirection = { false, "ServerTick", "ReverseDirection", false };

private:
};

