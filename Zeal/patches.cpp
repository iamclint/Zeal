#include "patches.h"
#include "Zeal.h"
#include "string_util.h"

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


// These patches improve /follow reliability. There is logic in /follow to turn run mode on and off
// and this actually makes your character crash out of the game if your framerate is high enough.
// There is also a smooth turning function to circle around to the followed target which is
// framerate dependent and causes follow failures.
// 
// Both of these things are disabled by this mod, and it also adds an adjustable follow distance.
void patches::SyncAutoFollow(bool first_boot)
{
	// To patch the follow distance, we modify the pointer to a float in an instruction.
	// We don't modify the value directly since that value is shared elsewhere in the code.
	const DWORD follow_distance_address = (0x00507D92 + 2);  // FADD dword ptr[addr]
	const float* follow_distance_default = reinterpret_cast<const float*>(0x005e44d4); // 15.0.
	static float follow_distance_modified = 15.0f;  // Provides the static value to point to.

	// Support disabling the rapid on/off toggling of run mode which can cause LDs or crashes.
	const DWORD run_mode_address = 0x00507DB0;  // FLD dword ptr[EBP + local_8]
	const BYTE run_mode_toggle_default[3] = { 0xd9, 0x45, 0xfc };  // Original client code.
	const BYTE run_mode_toggle_disabled[3] = { 0xeb, 0x43, 0x90 };  // Patched to disable (jmp 0x45, nop).

	// Support disabling the 'smoothing' where it only turns a little bit at a time if more than
	// a quarter circle off course.
	const DWORD turn_smoothing_address = 0x00507CB1;  // JNC LAB_00507cc1
	const BYTE turn_smoothing_default[2] = { 0x73, 0x0e };  // Original client code.
	const BYTE turn_smoothing_disabled[2] = { 0x90, 0x90 };  // Patched to disable (nop, nop).

	if (AutoFollowEnable.get())
	{
		follow_distance_modified = max(1.f, min(50.f, AutoFollowDistance.get()));
		mem::write(follow_distance_address, &follow_distance_modified);
		mem::write(run_mode_address, run_mode_toggle_disabled);
		mem::write(turn_smoothing_address, turn_smoothing_disabled);
	}
	else if (!first_boot)  // Do nothing at boot if Zeal mode is disabled.
	{
		mem::write(follow_distance_address, follow_distance_default);
		mem::write(run_mode_address, run_mode_toggle_default);
		mem::write(turn_smoothing_address, turn_smoothing_default);
	}
}

patches::patches()
{
	const char sit_stand_patch[] = { (char)0xEB, (char)0x1A };
	mem::write(0x42d14d, sit_stand_patch); //fix pet sit shortcut crash (makes default return of function the sit/stand button not sure why its passing in 0)
	
	//disable client sided hp ticking
	//mem::set(0x4b9141, 0x90, 6);
	SetBrownSkeletons();
	SyncAutoFollow(true);  // Sync only if non-default.

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

	ZealService::get_instance()->commands_hook->Add("/follow", {}, "Additional /follow options to improve reliability.",
		[this](std::vector<std::string>& args) {

			if (args.size() == 1)
				return false;  // No special flags, return false to let normal path handle.

			float distance = 10;
			if (args.size() == 3 && args[1] == "zeal")
			{
				if (args[2] == "on")
				{
					Zeal::EqGame::print_chat("Setting /follow mode to use Zeal patches");
					AutoFollowEnable.set(true);
					return true;  // Done
				}
				if (args[2] == "off")
				{
					Zeal::EqGame::print_chat("Setting /follow mode to use default client behavior");
					AutoFollowEnable.set(false);
					return true;  // Done
				}
			}
			else if (args.size() == 3 && args[1] == "distance" && Zeal::String::tryParse(args[2], &distance))
			{
				distance = max(1.f, min(50.f, distance));
				Zeal::EqGame::print_chat("Setting /follow distance in zeal mode to: %f", distance);
				AutoFollowDistance.set(distance);
				return true;  // Done
			}

			Zeal::EqGame::print_chat("Usage: /follow: Enable / disable auto-follow.");
			Zeal::EqGame::print_chat("Usage: /follow zeal on, /follow zeal off: Enable / disable zeal follow mode.");
			Zeal::EqGame::print_chat("Usage: /follow distance <value>: Sets the follow distance in zeal mode.");

			return true;
		});

}