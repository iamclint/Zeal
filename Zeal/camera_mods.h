#pragma once
#include "memory.h"
#include <chrono>

class CameraMods
{
public:
	std::chrono::steady_clock::time_point prevTime; 
	float sensitivity_x = 0.7f;
	float sensitivity_y = 0.4f;
	int eq_ptr = 0;
	bool smoothing;
	float height;
	void set_smoothing(bool val);
	void callback_main();
	CameraMods(class ZealService* pHookWrapper);
	~CameraMods();
};
	

