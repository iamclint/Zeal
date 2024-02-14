#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>
class MainLoop
{
public:
	void do_callbacks();
	void add_callback(std::function<void()> callback_function);
	MainLoop(class ZealService* zeal);
	~MainLoop();
private:
	std::vector<std::function<void()>> callback_functions;
};

