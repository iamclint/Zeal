#include "item_display.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>


void __fastcall Deconstruct(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, BYTE reason)
{
	Zeal::EqGame::get_wnd_manager()->Unknown0x8 -= 1;
}

void ItemDisplay::init_ui()
{
	display_windows.clear();
	display_windows.push_back(Zeal::EqGame::Windows->ItemWnd);
	for (int i = 0; i < max_item_displays; i++)
	{
		Zeal::EqUI::ItemDisplayWnd* new_wnd = new Zeal::EqUI::ItemDisplayWnd();
		display_windows.push_back(new_wnd);
		reinterpret_cast<Zeal::EqUI::ItemDisplayWnd* (__thiscall*)(const Zeal::EqUI::ItemDisplayWnd*, int unk)>(0x423331)(new_wnd, 0);
		new_wnd->SetupCustomVTable();
		new_wnd->vtbl->Deconstructor = Deconstruct;
		new_wnd->Location.Top += 20 * (i + 1);
		new_wnd->Location.Left += 20 * (i + 1);
		new_wnd->Location.Bottom += 20 * (i + 1);
		new_wnd->Location.Right += 20 * (i + 1);
	}
}

Zeal::EqUI::ItemDisplayWnd* ItemDisplay::get_available_window(Zeal::EqStructures::_EQITEMINFO* item)
{

	if (item)
	{
		/*check if the item is already being displayed*/
		for (auto& w : display_windows)
		{
			if (w->Item.Id == item->Id)
			{
				w->Activate();
				return w;
			}
		}
	}

	for (auto& w : display_windows)
	{
		if (!w->IsVisible)
		{
			w->Activate();
			return w;
		}
	}
	return display_windows.back();
}
void __fastcall SetItem(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, Zeal::EqStructures::_EQITEMINFO* item, bool show)
{
	ZealService* zeal = ZealService::get_instance();
	wnd = zeal->item_displays->get_available_window(item);
	zeal->hooks->hook_map["SetItem"]->original(SetItem)(wnd, unused, item, show);
}
void __fastcall SetSpell(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, int spell_id, bool show, int unknown)
{
	ZealService* zeal = ZealService::get_instance();
	wnd = zeal->item_displays->get_available_window(0);
	zeal->hooks->hook_map["SetSpell"]->original(SetSpell)(wnd, unused, spell_id, show, unknown);
}

ItemDisplay::ItemDisplay(ZealService* zeal, IO_ini* ini)
{
	//if (Zeal::EqGame::is_in_game()) init_ui(); /*for testing only must be in game before its loaded or you will crash*/
	zeal->hooks->Add("SetItem", 0x423640, SetItem, hook_type_detour);
	zeal->hooks->Add("SetSpell", 0x425957, SetSpell, hook_type_detour);
	zeal->main_loop_hook->add_callback([this]() { init_ui(); }, callback_fn::InitUI);
	//zeal->main_loop_hook->add_callback([this]() { clean_ui(); }, callback_fn::CleanUI);
	zeal->binds_hook->replace_bind(0xC8, [this](int state) { 
		for (auto rit = display_windows.rbegin(); rit != display_windows.rend(); ++rit) {
			Zeal::EqUI::ItemDisplayWnd* wnd = *rit;
			if (wnd->IsVisible)
			{
				wnd->IsVisible = false;
				return true;
			}
		}
		return false; 
	});
	mem::write<BYTE>(0x4090AB, 0xEB); //for some reason the game when setting spell toggles the item display window unlike with items..this just disables that feature
	mem::set(0x421EBF, 0x90, 14); //remove the auto focus of the main item window and handle it ourselves
	//0x798984 --render distance
	//0x798918 --fog maybe
	//0x5e780c --render distance multiplier? reused for multiple things would have to remap it
}
ItemDisplay::~ItemDisplay()
{
}

