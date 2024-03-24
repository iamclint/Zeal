#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class Melody
{
public:
	void start();
	void end();
	Melody(class ZealService* pHookWrapper, class IO_ini* ini);
	~Melody();
private:
	void tick();
	void stop_cast();
	bool active = false;
	int current_index = 0;
	std::vector<int> songs;
};

