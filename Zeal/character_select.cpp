#include "character_select.h"
#include "Zeal.h"
#include <random>

DWORD GetRandomZone()
{
	// Seed with a random device for better randomness
	static std::random_device rd;
	static std::mt19937 generator(rd());  // Mersenne Twister engine
	static std::uniform_int_distribution<DWORD> distribution(1, 77);

	return distribution(generator);
}

void __fastcall StartWorldDisplay(DWORD t, DWORD unk, DWORD zone_index, DWORD uhh)
{
	if (zone_index == 0xB9) //loading (character select)
		zone_index = GetRandomZone();
	ZealService::get_instance()->hooks->hook_map["StartWorldDisplay"]->original(StartWorldDisplay)(t, unk, zone_index, uhh);
	reinterpret_cast<void(__stdcall*)(void)>(0x4B459C)(); //moveplayerlocalsafecoords
}



CharacterSelect::CharacterSelect(ZealService* zeal)
{
	zeal->hooks->Add("StartWorldDisplay", 0x4A849E, StartWorldDisplay, hook_type_detour);
}
CharacterSelect::~CharacterSelect()
{

}