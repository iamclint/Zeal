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

// Base client is tagging other players that die as Type = 2 (NPCCorpse) while tagging a self death
// as Type = 3. Upon camping and rejoining, other player corpses are tagged as Type = 3, so this
// looks like a client bug to patch (impacts corpse nameplates and targeting).
static void __fastcall ProcessDeath(uint32_t passthruECX, uint32_t unusedEDX,
	Zeal::Packets::Death_Struct* death_struct)
{
	auto* ent = ZealService::get_instance()->entity_manager->Get(death_struct->spawn_id);
	bool player_death = (ent != nullptr && ent->Type == Zeal::EqEnums::Player);
	ZealService::get_instance()->hooks->hook_map["ProcessDeath"]->original(ProcessDeath)(passthruECX, unusedEDX, death_struct);
	if (player_death && ent->Type == Zeal::EqEnums::NPCCorpse)
		ent->Type = Zeal::EqEnums::PlayerCorpse;
}

// There is a client startup crash issue where it looks like the CBreathWnd::OnProcessFrame or
// EQ_3DView::DisplaySpells is calling CanIBreathe with a EQCHARINFO that has a null SpawnInfo entry.
// The other path calling CanIBreathe protects against this.
static int32_t __fastcall CanIBreathe(Zeal::EqStructures::EQCHARINFO* self_char_info, uint32_t unusedEDX)
{
	if (!self_char_info)
		return 1;  // Not expected to happen, so just default to true, can breathe.

	// Patch the crashing case (null SpawnInfo) here.
	if (!self_char_info->SpawnInfo) {
		self_char_info->IsSwimmingUnderwater = 0;  // Match the updating behavior of CanIBreathe with an assumption.
		return 1;  // And just respond that yes can breathe (for now).
	}

	return ZealService::get_instance()->hooks->hook_map["CanIBreathe"]->original(CanIBreathe)(self_char_info, unusedEDX);
}

void patches::fonts() //this was a test function and I later found out these aren't even used
{
	//const char* driverName = "DISPLAY";

	//// Create the information context (IC) for the display device
	//HDC hdc = CreateICA(driverName, NULL, NULL, NULL);

	//// Check if the device context was created successfully
	//if (hdc == NULL) return;

	//HFONT fonts[6] = {};
	//fonts[0] = reinterpret_cast<HFONT(__stdcall*)(int height, int weight, LPCSTR)>(0x55AE55)(10, 100, "Arial");
	//fonts[1] = reinterpret_cast<HFONT(__stdcall*)(int height, int weight, LPCSTR)>(0x55AE55)(12, 100, "Arial");
	//fonts[2] = reinterpret_cast<HFONT(__stdcall*)(int height, int weight, LPCSTR)>(0x55AE55)(14, 100, "Arial");
	//fonts[3] = reinterpret_cast<HFONT(__stdcall*)(int height, int weight, LPCSTR)>(0x55AE55)(20, 100, "Arial");
	//fonts[4] = reinterpret_cast<HFONT(__stdcall*)(int height, int weight, LPCSTR)>(0x55AE55)(24, 100, "Arial");
	//fonts[5] = reinterpret_cast<HFONT(__stdcall*)(int height, int weight, LPCSTR)>(0x55AE55)(32, 100, "Arial");

	//int** game_fonts = reinterpret_cast<int**>(0x809444);
	//game_fonts[0] = reinterpret_cast<int*(__stdcall*)(HDC context, HGDIOBJ font, int size)>(0x55aed3)(hdc, fonts[0], 10);
	//game_fonts[1] = reinterpret_cast<int* (__stdcall*)(HDC context, HGDIOBJ font, int size)>(0x55aed3)(hdc, fonts[1], 12);
	//game_fonts[2] = reinterpret_cast<int* (__stdcall*)(HDC context, HGDIOBJ font, int size)>(0x55aed3)(hdc, fonts[2], 14);
	//game_fonts[3] = reinterpret_cast<int* (__stdcall*)(HDC context, HGDIOBJ font, int size)>(0x55aed3)(hdc, fonts[3], 20);
	//game_fonts[4] = reinterpret_cast<int* (__stdcall*)(HDC context, HGDIOBJ font, int size)>(0x55aed3)(hdc, fonts[4], 24);
	//game_fonts[5] = reinterpret_cast<int* (__stdcall*)(HDC context, HGDIOBJ font, int size)>(0x55aed3)(hdc, fonts[5], 32);
	//for (int i = 0; i < 6; i++)
	//	DeleteObject(fonts[i]);

}

void patches::SetBrownSkeletons()
{
	if (BrownSkeletons.get())
	{
		mem::write<byte>(0x49f297, 0xEB);
	}
	else
	{
		mem::write<byte>(0x49f297, 0x75);
	}
}
patches::patches()
{
	const char sit_stand_patch[] = { (char)0xEB, (char)0x1A };
	mem::write(0x42d14d, sit_stand_patch); //fix pet sit shortcut crash (makes default return of function the sit/stand button not sure why its passing in 0)
	
	//disable client sided hp ticking
	//mem::set(0x4b9141, 0x90, 6);
	SetBrownSkeletons();
	//disable client sided mana ticking
	mem::set(0x4C3F93, 0x90, 7);
	mem::set(0x4C7642, 0x90, 7);

	mem::write<BYTE>(0x40f07a, 0); //disable character select rotation by default
	mem::write<BYTE>(0x40f07d, 0xEB); //uncheck rotate button defaultly
	

	//the following does not work entirely needs more effort
	//mem::write<byte>(0x4A594B, 15); //load font sizes 1 to 14 (default is 6)
	//mem::write<byte>(0X4FDB6A, 15); //allow /chatfontsize to be larger than 5

	mem::write<BYTE>(0x4A14CF, 0xEB); //don't print Your XML files are not compatible with current EverQuest files, certain windows may not perform correctly.  Use "/loadskin Default 1" to load the EverQuest default skin.

	ZealService::get_instance()->hooks->Add("GetZoneInfoFromNetwork", 0x53D026, GetZoneInfoFromNetwork, hook_type_detour);

	ZealService::get_instance()->hooks->Add("ProcessDeath", 0x00528E16, ProcessDeath, hook_type_detour);
	ZealService::get_instance()->hooks->Add("CanIBreathe", 0x004C0DAB, CanIBreathe, hook_type_detour);

	//fonts();
	//*(int*)0x809458 =
}