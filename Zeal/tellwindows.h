#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
class TellWindows
{
public:
	bool HandleTell(std::string& cmd_data);
	TellWindows(class ZealService* zeal, class IO_ini* ini);
	~TellWindows();
	void SetEnabled(bool val);
	bool enabled = false;
private:
	void CleanUI();
	void LoadUI();
};

