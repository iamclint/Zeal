#include "raid.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"

void raid::callback_main()
{

}

raid::~raid()
{

}

void __fastcall SetLootTypeResponse(void* t, int unused, int p1)
{
	ZealService* zeal = ZealService::get_instance();
	int new_loot_type = *(int*)(p1 + 0x84);
	if (new_loot_type == 4)
	{
		Zeal::EqGame::print_chat("The loot type is now - free for all");
		return;
	}
	else
	{
		zeal->hooks->hook_map["SetLootTypeResponse"]->original(SetLootTypeResponse)(t, unused, p1);
	}
}

raid::raid(ZealService* zeal)
{
	mem::write<byte>(0x49E182, 4); // allow for 4 types in setloottype
	mem::write<byte>(0x42FAB3, 4); // allow for 4 types being set from the options window
	zeal->hooks->Add("SetLootTypeResponse", 0x49dbc1, SetLootTypeResponse, hook_type_detour); //add extra prints for new loot types
	zeal->callbacks->add_callback([this]() { callback_main(); });
}
