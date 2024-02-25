#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class raid
{
public:
	raid(class ZealService* zeal);
	~raid();
private:
	void callback_main();
};

