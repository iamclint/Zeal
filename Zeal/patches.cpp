#include "patches.h"
#include "Zeal.h"


void __fastcall GetZoneInfoFromNetwork(int* t, int unused, char* p1)
{
	int* backup_this = t;

	ZealService::get_instance()->hooks->hook_map["GetZoneInfoFromNetwork"]->original(GetZoneInfoFromNetwork)(t, unused, p1);
	int retry_count = 0;
	while (!t)
	{
		retry_count++;
		Sleep(100);
		t = backup_this;
		ZealService::get_instance()->hooks->hook_map["GetZoneInfoFromNetwork"]->original(GetZoneInfoFromNetwork)(t, unused, p1);
		if (retry_count >= 15 && !t)
		{
			MessageBoxA(NULL, "Zeal attempted to retry GetZoneInfoFromNetwork but has failed", "Crash", 0);
			break;
		}
	}
}

patches::patches()
{
	const char sit_stand_patch[] = { (char)0xEB, (char)0x1A };
	mem::write(0x42d14d, sit_stand_patch); //fix pet sit shortcut crash (makes default return of function the sit/stand button not sure why its passing in 0)


	//disable client sided mana ticking
	mem::set(0x4C3F93, 0x90, 7);
	mem::set(0x4C7642, 0x90, 7);
	mem::write<BYTE>(0x4A14CF, 0xEB); //don't print Your XML files are not compatible with current EverQuest files, certain windows may not perform correctly.  Use "/loadskin Default 1" to load the EverQuest default skin.

	ZealService::get_instance()->hooks->Add("GetZoneInfoFromNetwork", 0x53D026, GetZoneInfoFromNetwork, hook_type_detour);
}