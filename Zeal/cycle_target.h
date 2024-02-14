#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class CycleTarget
{
public:
	CycleTarget(class ZealService* zeal);
	~CycleTarget();

private:
	hook* hook;
};
