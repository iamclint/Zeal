#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
class Physics
{
public:
	Physics(class ZealService* zeal, class IO_ini* ini);
	~Physics();
	bool process();
	bool movement();
private:
	bool did_physics = false;
	ULONGLONG last_physic_calc = 0;
};

