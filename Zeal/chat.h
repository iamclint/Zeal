#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class chat
{
public:
	bool timestamps=false;
	chat(class ZealService* pHookWrapper, class IO_ini* ini);
	~chat();
private:
	void LoadSettings(class IO_ini* ini);
};
