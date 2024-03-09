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

void MainLoop::do_callbacks(callback_fn fn)
{
	if (fn == callback_fn::MainLoop)
	{
		for (auto& f : callback_functions)
			f();
	}
	else if (fn == callback_fn::Zone)
	{
		for (auto& f : callback_functions_zone)
			f();
	}
}

void MainLoop::add_callback(std::function<void()> callback_function, callback_fn fn)
{
	if (fn == callback_fn::MainLoop)
	{
		callback_functions.push_back(callback_function);
	}
	else if (fn == callback_fn::Zone)
	{
		callback_functions_zone.push_back(callback_function);
	}
}

void __fastcall EnterZone(int t, int unused, int hwnd)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->main_loop_hook->do_callbacks(callback_fn::Zone);
	zeal->hooks->hook_map["EnterZone"]->original(EnterZone)(t, unused, hwnd);
}


MainLoop::MainLoop(ZealService* zeal)
{
	//zeal->hooks->Add("main_loop", Zeal::EqGame::EqGameInternal::fn_main_loop, main_loop_hk, hook_type_detour);
	zeal->hooks->Add("main_loop", 0x4aa8bc, main_loop_hk, hook_type_detour);
	zeal->hooks->Add("EnterZone", 0x53D2C4, EnterZone, hook_type_detour);
}