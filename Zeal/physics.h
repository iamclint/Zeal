#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
class Physics
{
public:
	Physics(class ZealService* zeal, class IO_ini* ini);
	~Physics();
	bool can_move(short spawn_id);
private:
	bool did_physics = false;
	ULONGLONG last_physic_calc = 0;
	std::unordered_map<short, UINT> move_timers;
};

