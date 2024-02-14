#pragma once
#include "memory.h"
#include <chrono>

class MouseZoom
{
public:
	std::chrono::steady_clock::time_point prevTime; 
	MouseZoom(class ZealService* pHookWrapper);
	~MouseZoom();
};
	

