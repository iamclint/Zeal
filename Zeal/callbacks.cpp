#include "callbacks.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
CallBacks::~CallBacks()
{

}

void __fastcall main_loop_hk(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->do_callbacks(callback_fn::MainLoop);
	zeal->hooks->hook_map["main_loop"]->original(main_loop_hk)(t, unused);
}

void __fastcall render_hk(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->do_callbacks(callback_fn::Render);
	zeal->hooks->hook_map["Render"]->original(render_hk)(t, unused);
}

void _fastcall charselect_hk(int t, int u)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->do_callbacks(callback_fn::CharacterSelect);
	zeal->hooks->hook_map["DoCharacterSelection"]->original(charselect_hk)(t, u);
}

void CallBacks::eml()
{
	do_callbacks(callback_fn::EndMainLoop);
}

void CallBacks::do_callbacks(callback_fn fn)
{
	for (auto& f : callback_functions[fn])
			f();
}

void CallBacks::add_callback(std::function<void()> callback_function, callback_fn fn)
{
	callback_functions[fn].push_back(callback_function);
}
void CallBacks::add_worldmessage_callback(std::function<bool(UINT, char*, UINT)> callback_function)
{
	worldmessage_functions.push_back(callback_function);
}
void CallBacks::add_sendmessage_callback(std::function<bool(UINT, char*, UINT)> callback_function)
{
	sendmessage_functions.push_back(callback_function);
}
void __fastcall enterzone_hk(int t, int unused, int hwnd)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->do_callbacks(callback_fn::Zone);
	zeal->hooks->hook_map["EnterZone"]->original(enterzone_hk)(t, unused, hwnd);
}
void __fastcall initgameui_hk(int t, int u)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->hooks->hook_map["InitGameUI"]->original(initgameui_hk)(t, u);
	zeal->callbacks->do_callbacks(callback_fn::InitUI);
}
void __stdcall clean_up_ui()
{
	ZealService* zeal = ZealService::get_instance();
	zeal->callbacks->do_callbacks(callback_fn::CleanUI);
	zeal->hooks->hook_map["CleanUpUI"]->original(clean_up_ui)();
}

bool CallBacks::do_worldmessage_callbacks(UINT opcode, char* buffer, UINT len)
{
	for (auto& fn : worldmessage_functions)
	{
		if (fn(opcode, buffer, len))
			return true;
	}
	return false;
}

bool CallBacks::do_sendmessage_callbacks(UINT opcode, char* buffer, UINT len)
{
	for (auto& fn : sendmessage_functions)
	{
		if (fn(opcode, buffer, len))
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

	if (zeal->callbacks->do_worldmessage_callbacks(opcode, buffer, len))
		return 1;
	return zeal->hooks->hook_map["HandleWorldMessage"]->original(handleworldmessage_hk)(connection, unused, unk, opcode, buffer, len);
}
void send_message_hk(int* connection, UINT opcode, int* buffer, UINT size, int unknown)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->hooks->hook_map["SendMessage"]->original(send_message_hk)(connection, opcode, buffer, size, unknown);
}


CallBacks::CallBacks(ZealService* zeal)
{
	zeal->hooks->Add("main_loop", 0x5473c3, main_loop_hk, hook_type_detour);
	zeal->hooks->Add("Render", 0x4AA8BC, render_hk, hook_type_detour);
	zeal->hooks->Add("EnterZone", 0x53D2C4, enterzone_hk, hook_type_detour);
	zeal->hooks->Add("CleanUpUI", 0x4A6EBC, clean_up_ui, hook_type_detour);
	zeal->hooks->Add("DoCharacterSelection", 0x53b9cf, charselect_hk, hook_type_detour);
	zeal->hooks->Add("InitGameUI", 0x4a60b5, initgameui_hk, hook_type_detour);
	zeal->hooks->Add("HandleWorldMessage", 0x4e829f, handleworldmessage_hk, hook_type_detour);
	zeal->hooks->Add("SendMessage", 0x54e51a, send_message_hk, hook_type_detour);
}
