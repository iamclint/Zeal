#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>
enum class callback_fn
{
	MainLoop,
	Zone,
	CleanUI,
	Render,
	CharacterSelect,
	InitUI,
	EndMainLoop,
	WorldMessage
};
class CallBacks
{
public:
	void do_callbacks(callback_fn fn);
	void add_callback(std::function<void()> callback_function, callback_fn fn = callback_fn::MainLoop);
	void add_worldmessage_callback(std::function<bool(UINT, char*, UINT)> callback_function);
	bool do_worldmessage_callbacks(UINT opcode, char* buffer, UINT len);
	CallBacks(class ZealService* zeal);
	~CallBacks();
	void eml();
private:
	std::unordered_map<callback_fn, std::vector<std::function<void()>>> callback_functions;
	std::vector<std::function<bool(UINT, char*, UINT)>> worldmessage_functions;
};

