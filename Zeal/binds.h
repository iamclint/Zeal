#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>

enum key_category
{
	Movement = 1,
	Combat = 2,
	Target = 8
};

enum strafe_direction
{
	None,
	Left,
	Right
};

class Binds
{
public:
	Binds(class ZealService* zeal);
	~Binds();
	char* KeyMapNames[256] = { 0 };
	int ptr_binds = 0;
	std::unordered_map<int, std::function<void(int state)>> KeyMapFunctions;
	void read_ini();
	void add_binds();
	void add_bind(int index, const char* name, const char* short_name, int category, std::function<void(int state)> callback);
private:
	hook* hook;
};


