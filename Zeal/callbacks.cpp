#include "callbacks.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
CallbackManager::~CallbackManager()
{

}

void __fastcall main_loop_hk(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->invoke_generic(callback_type::MainLoop);
	zeal->callbacks->invoke_delayed();
	zeal->hooks->hook_map["MainLoop"]->original(main_loop_hk)(t, unused);
}

void __fastcall render_hk(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->hooks->hook_map["Render"]->original(render_hk)(t, unused);
	zeal->callbacks->invoke_generic(callback_type::Render);
}

void render_ui(int x)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->invoke_generic(callback_type::RenderUI);
	zeal->hooks->hook_map["RenderUI"]->original(render_ui)(x);
}

void _fastcall charselect_hk(int t, int u)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->invoke_generic(callback_type::CharacterSelect);
	zeal->hooks->hook_map["DoCharacterSelection"]->original(charselect_hk)(t, u);
}

void CallbackManager::eml()
{
	invoke_generic(callback_type::EndMainLoop);
}

void CallbackManager::invoke_generic(callback_type fn)
{
	for (auto& f : generic_functions[fn])
			f();
}

void CallbackManager::AddDelayed(std::function<void()> callback_function, int ms)
{
	delayed_functions.push_back({ GetTickCount64() + ms, callback_function });
}

void CallbackManager::AddGeneric(std::function<void()> callback_function, callback_type fn)
{
	generic_functions[fn].push_back(callback_function);
}
void CallbackManager::AddPacket(std::function<bool(UINT, char*, UINT)> callback_function, callback_type type)
{
	packet_functions[type].push_back(callback_function);
}
void CallbackManager::AddCommand(std::function<bool(UINT, BOOL)> callback_function, callback_type type)
{
	cmd_functions[type].push_back(callback_function);
}

void __fastcall enterzone_hk(int t, int unused, int hwnd)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->hooks->hook_map["EnterZone"]->original(enterzone_hk)(t, unused, hwnd);
	zeal->callbacks->invoke_generic(callback_type::Zone);
}
void __fastcall initgameui_hk(int t, int u)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->hooks->hook_map["InitGameUI"]->original(initgameui_hk)(t, u);
	zeal->callbacks->invoke_generic(callback_type::InitUI);
}
void __stdcall clean_up_ui()
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->invoke_generic(callback_type::CleanUI);
	zeal->hooks->hook_map["CleanUpUI"]->original(clean_up_ui)();
}

void CallbackManager::invoke_delayed()
{
	ULONGLONG current_time = GetTickCount64();
	for (auto& [end_time, fn] : delayed_functions)
	{
		if (current_time >= end_time)
			fn();
	}
	delayed_functions.erase(
		std::remove_if(
			delayed_functions.begin(),
			delayed_functions.end(),
			[current_time](const std::pair<ULONGLONG, std::function<void()>>& item) {
				return current_time>item.first;
			}
		),
		delayed_functions.end()
	);
}

bool CallbackManager::invoke_packet(callback_type cb_type, UINT opcode, char* buffer, UINT len)
{
	for (auto& fn : packet_functions[cb_type])
	{
		if (fn(opcode, buffer, len))
			return true;
	}
	return false;
}

bool CallbackManager::invoke_command(callback_type cb_type, UINT opcode, bool state)
{
	for (auto& fn : cmd_functions[cb_type])
	{
		if (fn(opcode, state))
			return true;
	}
	return false;
}

char __fastcall handleworldmessage_hk(int* connection, int unused, UINT unk, UINT opcode, char* buffer, UINT len)
{
	ZealService* zeal = ZealService::get_instance();

	//static std::vector<UINT> ignored_opcodes = { 16543, 16629, 16530, 16425, 16562, 16526, 16694, 8253, 16735, 16727, 16735, 16458 };
	//if (!std::count(ignored_opcodes.begin(), ignored_opcodes.end(), opcode))
	//	Zeal::EqGame::print_chat("opcode: 0x%x len: %i", opcode, len);

	//if (!Zeal::EqGame::get_self() && opcode == 0x4107) //a fix for a crash reported by Ecliptor at 0x004E2803
	//	return 1;

	if (zeal->callbacks->invoke_packet(callback_type::WorldMessage,opcode, buffer, len))
		return 1;

	return zeal->hooks->hook_map["HandleWorldMessage"]->original(handleworldmessage_hk)(connection, unused, unk, opcode, buffer, len);
}
void send_message_hk(int* connection, UINT opcode, char* buffer, UINT len, int unknown)
{
	ZealService* zeal = ZealService::get_instance();
	//Zeal::EqGame::print_chat("Opcode %i   len: %i", opcode, len);
	if (zeal->callbacks->invoke_packet(callback_type::SendMessage_, opcode, buffer, len))
		return;

	zeal->hooks->hook_map["SendMessage"]->original(send_message_hk)(connection, opcode, buffer, len, unknown);
}

void executecmd_hk(UINT cmd, bool isdown, int unk2)
{
	ZealService* zeal = ZealService::get_instance();
	//Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Cmd: %i", cmd);
	if (cmd == 0xd2)
		zeal->callbacks->invoke_generic(callback_type::EndMainLoop);
	if (zeal->callbacks->invoke_command(callback_type::ExecuteCmd, cmd, isdown))
		return;

	zeal->hooks->hook_map["ExecuteCmd"]->original(executecmd_hk)(cmd, isdown, unk2);
}

void msg_new_text(char* msg)
{
	ZealService* zeal = ZealService::get_instance();
	if (!Zeal::EqGame::get_self())
	{
	//	Zeal::EqGame::print_chat("self was null during new text");
		return;
	}
	zeal->hooks->hook_map["MsgNewText"]->original(msg_new_text)(msg);
}

int __fastcall AddDeferred(int t, int u)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->invoke_generic(callback_type::AddDeferred);
	return ZealService::get_instance()->hooks->hook_map["AddDeferred"]->original(AddDeferred)(t, u);
}

CallbackManager::CallbackManager(ZealService* zeal)
{
	zeal->hooks->Add("AddDeferred", 0x59E000, AddDeferred, hook_type_detour); //render in this hook so damage is displayed behind ui
	zeal->hooks->Add("MsgNewText", 0x4e25a1, msg_new_text, hook_type_detour);
	zeal->hooks->Add("ExecuteCmd", 0x54050c, executecmd_hk, hook_type_detour);
	zeal->hooks->Add("MainLoop", 0x5473c3, main_loop_hk, hook_type_detour);
	zeal->hooks->Add("Render", 0x4AA8BC, render_hk, hook_type_detour);
	HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");
	if (eqfx)
		zeal->hooks->Add("RenderUI", (DWORD)eqfx+0x6b7f0, render_ui, hook_type_detour);
	
	zeal->hooks->Add("EnterZone", 0x53D2C4, enterzone_hk, hook_type_detour);
	zeal->hooks->Add("CleanUpUI", 0x4A6EBC, clean_up_ui, hook_type_detour);
	zeal->hooks->Add("DoCharacterSelection", 0x53b9cf, charselect_hk, hook_type_detour);
	zeal->hooks->Add("InitGameUI", 0x4a60b5, initgameui_hk, hook_type_detour);
	zeal->hooks->Add("HandleWorldMessage", 0x4e829f, handleworldmessage_hk, hook_type_detour);
	zeal->hooks->Add("SendMessage", 0x54e51a, send_message_hk, hook_type_detour);
}
