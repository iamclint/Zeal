#include "main_loop.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
MainLoop::~MainLoop()
{

}

void __fastcall main_loop_hk(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->main_loop_hook->do_callbacks(callback_fn::MainLoop);
	zeal->hooks->hook_map["main_loop"]->original(main_loop_hk)(t, unused);
}


void __fastcall render_hk(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->main_loop_hook->do_callbacks(callback_fn::Render);
	zeal->hooks->hook_map["render"]->original(main_loop_hk)(t, unused);
}

void _fastcall charselect_hk(int t, int u)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->main_loop_hook->do_callbacks(callback_fn::CharacterSelect);
	zeal->hooks->hook_map["DoCharacterSelection"]->original(charselect_hk)(t, u);
}

void MainLoop::do_callbacks(callback_fn fn)
{
	for (auto& f : callback_functions[fn])
			f();
}

void MainLoop::add_callback(std::function<void()> callback_function, callback_fn fn)
{
	callback_functions[fn].push_back(callback_function);
}

void __fastcall EnterZone(int t, int unused, int hwnd)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->main_loop_hook->do_callbacks(callback_fn::Zone);
	zeal->hooks->hook_map["EnterZone"]->original(EnterZone)(t, unused, hwnd);
}

void __stdcall clean_up_ui()
{
	ZealService* zeal = ZealService::get_instance();
	zeal->main_loop_hook->do_callbacks(callback_fn::CleanUI);
	zeal->hooks->hook_map["CleanUpUI"]->original(clean_up_ui)();
}


MainLoop::MainLoop(ZealService* zeal)
{
	zeal->hooks->Add("main_loop", 0x5473c3, main_loop_hk, hook_type_detour);
	zeal->hooks->Add("render", 0x4aa8bc, render_hk, hook_type_detour);
	zeal->hooks->Add("EnterZone", 0x53D2C4, EnterZone, hook_type_detour);
	zeal->hooks->Add("CleanUpUI", 0x4A6EBC, clean_up_ui, hook_type_detour);
	zeal->hooks->Add("DoCharacterSelection", 0x53b9cf, charselect_hk, hook_type_detour);
}
