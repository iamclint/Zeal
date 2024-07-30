#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>
enum class callback_type
{
	MainLoop,
	Zone,
	CleanUI,
	Render,
	CharacterSelect,
	InitUI,
	EndMainLoop,
	WorldMessage,
	SendMessage_,
	ExecuteCmd,
	Delayed,
	RenderUI,
	EndScene,
	AddDeferred
};
class CallbackManager
{
public:
	void AddGeneric(std::function<void()> callback_function, callback_type fn = callback_type::MainLoop);
	void AddPacket(std::function<bool(UINT, char*, UINT)> callback_function, callback_type fn = callback_type::WorldMessage);
	void AddCommand(std::function<bool(UINT, BOOL)> callback_function, callback_type fn = callback_type::ExecuteCmd);
	void AddDelayed(std::function<void()> callback_function, int ms);
	void invoke_generic(callback_type fn);
	bool invoke_packet(callback_type fn, UINT opcode, char* buffer, UINT len);
	bool invoke_command(callback_type fn, UINT opcode, bool state);
	void invoke_delayed();
	CallbackManager(class ZealService* zeal);
	~CallbackManager();
	void eml();
private:
	std::vector<std::pair<ULONGLONG, std::function<void()>>> delayed_functions;
	std::unordered_map<callback_type, std::vector<std::function<void()>>> generic_functions;
	std::unordered_map<callback_type, std::vector<std::function<bool(UINT, char*, UINT)>>> packet_functions;
	std::unordered_map<callback_type, std::vector<std::function<bool(UINT, BOOL)>>> cmd_functions;
};

