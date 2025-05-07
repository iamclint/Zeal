#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class Raid
{
public:
	Raid(class ZealService* zeal);
	~Raid();
private:
	void callback_main();
};

