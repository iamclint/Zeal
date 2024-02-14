#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class looting
{
public:
	looting(class ZealService* zeal);
	~looting();

private:
	hook* hook;
};
