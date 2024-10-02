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
	windows.clear();
	windows.push_back(Zeal::EqGame::Windows->ItemWnd);
	for (int i = 0; i < max_item_displays; i++)
	{
		Zeal::EqUI::ItemDisplayWnd* new_wnd = new Zeal::EqUI::ItemDisplayWnd();
		mem::set((int)new_wnd, 0, sizeof(Zeal::EqUI::ItemDisplayWnd));
		windows.push_back(new_wnd);
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
		for (auto& w : windows)
		{
			if (w->Item.ID == item->ID)
				return w;
		}
	}

	for (auto& w : windows)
	{
		if (!w->IsVisible)
			return w;
	}
	return windows.back();
}
void __fastcall SetItem(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, Zeal::EqStructures::_EQITEMINFO* item, bool show)
{
	ZealService* zeal = ZealService::get_instance();
	wnd = zeal->item_displays->get_available_window(item);
	zeal->hooks->hook_map["SetItem"]->original(SetItem)(wnd, unused, item, show);
	wnd->IconBtn->ZLayer = wnd->ZLayer;
	wnd->Activate();
}
char* build_token_string_PARAM(char* buffer, int stringID, char* string1, char* string2, int u1, int u2, int u3, int u4, int u5, int u6, int u7)
{
	Zeal::EqStructures::_EQITEMINFO* item;

	__asm mov item, esi

	if (strcmp(string1, "Haste") == 0)
	{
		if (strcmp(string2, "(Worn)") == 0)
		{
			int haste_percentage = item->Common.CastingLevel + 1;
			char* haste = new char[24];
			sprintf_s(haste, 24, "Haste: %d%%%%", haste_percentage);
			return ZealService::get_instance()->hooks->hook_map["ModifyHaste"]->original(build_token_string_PARAM)(buffer, stringID, haste, 0, 0, 0, 0, 0, 0, 0, 0);
		}
	}
	return ZealService::get_instance()->hooks->hook_map["ModifyHaste"]->original(build_token_string_PARAM)(buffer, stringID, string1, string2, 0, 0, 0, 0, 0, 0, 0);
}

void __fastcall SetSpell(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, int spell_id, bool show, int unknown)
{
	ZealService* zeal = ZealService::get_instance();
	wnd = zeal->item_displays->get_available_window(0);
	zeal->hooks->hook_map["SetSpell"]->original(SetSpell)(wnd, unused, spell_id, show, unknown);
	wnd->IconBtn->ZLayer = wnd->ZLayer;
	wnd->Activate();
}

void ItemDisplay::CleanUI()
{
		for (auto& w : windows)
		{
			if (w)
				w->IsVisible = false; 
		}
		windows.clear();
}

ItemDisplay::ItemDisplay(ZealService* zeal, IO_ini* ini)
{
	windows.clear();
//	if (Zeal::EqGame::is_in_game()) init_ui(); /*for testing only must be in game before its loaded or you will crash*/
	zeal->hooks->Add("SetItem", 0x423640, SetItem, hook_type_detour);
	zeal->hooks->Add("SetSpell", 0x425957, SetSpell, hook_type_detour);
	zeal->hooks->Add("ModifyHaste", 0x424a95, build_token_string_PARAM, hook_type_replace_call);
	zeal->callbacks->AddGeneric([this]() { init_ui(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	//zeal->callbacks->add_generic([this]() { if (!Zeal::EqGame::is_in_game()) CleanUI(); }, callback_type::MainLoop);

	mem::write<BYTE>(0x4090AB, 0xEB); //for some reason the game when setting spell toggles the item display window unlike with items..this just disables that feature
	mem::write<BYTE>(0x40a4c4, 0xEB); //for some reason the game when setting spell toggles the item display window unlike with items..this just disables that feature
	mem::set(0x421EBF, 0x90, 14); //remove the auto focus of the main item window and handle it ourselves
	mem::set(0x4e81e0, 0x90, 14); //remove the auto focus of the main item window and handle it ourselves
	mem::set(0x4229BE, 0x90, 14); //remove the opening of the window when holding right click (allow zeal to handle it)

}

ItemDisplay::~ItemDisplay()
{
}

