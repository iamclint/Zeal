#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqStructures.h"

class CycleTarget
{
public:
	CycleTarget(class ZealService* zeal);
	~CycleTarget();
	Zeal::EqStructures::Entity* get_next_ent(float dist, BYTE type);
	Zeal::EqStructures::Entity* get_nearest_ent(float dist, BYTE type);

private:
	//hook* hook;
};
