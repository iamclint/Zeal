#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>

enum key_category //this is bitwise so you can do multiple categorys by doing Movement | Target for example
{
	Movement = 1,
	Commands = 2,
	Spell = 4,
	Target = 8,
	Camera = 16,
	Chat = 32,
	UI = 64,
	Macros = 128
};

class Binds
{
public:
	Binds(class ZealService* zeal);
	~Binds();
	char* KeyMapNames[256] = { 0 };
	int ptr_binds = 0;
	std::unordered_map<int, std::function<void(int state)>> KeyMapFunctions;
	std::unordered_map<int, std::vector<std::function<bool(int state)>>> ReplacementFunctions;
	std::pair<int, int> last_targets;
	void read_ini();
	void add_binds();
	void add_bind(int index, const char* name, const char* short_name, int category, std::function<void(int state)> callback);
	void replace_cmd(int cmd, std::function<bool(int state)> callback);
	void main_loop();
	void on_zone();
	bool execute_cmd(UINT opcode, bool state);
};