#include "main_loop.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
MainLoop::~MainLoop()
{

}

void __fastcall main_loop_hk(int t, int unk, HWND wnd)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->main_loop_hook->do_callbacks();
	zeal->hooks->hook_map["main_loop"]->original(main_loop_hk)(t, unk, wnd);
}

void MainLoop::do_callbacks()
{
	for (auto& f : callback_functions)
		f();
}
void MainLoop::add_callback(std::function<void()> callback_function)
{
	callback_functions.push_back(callback_function);
}

MainLoop::MainLoop(ZealService* zeal)
{
	zeal->hooks->Add("main_loop", Zeal::EqGame::EqGameInternal::fn_main_loop, main_loop_hk, hook_type_detour, 9);
}