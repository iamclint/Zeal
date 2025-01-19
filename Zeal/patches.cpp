#include "patches.h"
#include "Zeal.h"

// We use SpawnAppearance packets to do client<->server feature flag handshakes. This SpawnAppearance type (257) is a unique value we use for Zeal messages.
// Also defined in eq_constants.h on the server, but we can use any 256+ value as long as its unique.
constexpr WORD kSpawnAppearanceTypeZealFeature = 257;

// These are feature flag IDs that can be put into the cliemt<->server feature flag handshakes to setup features that require server cooperation.
// These IDs must match up with the server (eq_constants.h) so it knows what feature we are enabling.
constexpr DWORD kZealFeatureFixBuffStacking_Id = 1; // "fix buffstacking" feature

// The value to send for turning on the "fix buffstacking" feature flag.
constexpr DWORD kZealFeatureFixBuffStacking_Version_1 = 1;

// SPA spelleffect
constexpr int SE_MovementSpeed = 3;

// Status of the buffstacking fix. Right now it's 0 or 1, unless we support different versions/iterations of this.
WORD buffstacking_fix = 0;
bool buffstacking_in_find_affect_slot = false;

void SendFeatureHandshake(DWORD feature_id, DWORD feature_value, bool is_request) {
	Zeal::Packets::SpawnAppearance_Struct handshake;
	handshake.type = kSpawnAppearanceTypeZealFeature;
	handshake.spawn_id = 0;
	handshake.parameter = (feature_id << 16) | feature_value;
	if (is_request)
		handshake.parameter &= 0x7FFFFFFFu;
	else
		handshake.parameter |= 0x80000000u;
	Zeal::EqGame::send_message(Zeal::Packets::opcodes::SpawnAppearance, (int*)&handshake, sizeof(Zeal::Packets::SpawnAppearance_Struct), 0);
}

void HandleFeatureHandshake(DWORD feature_id, DWORD feature_value, bool is_request)
{
	// Got a handshake message from the server. Turn on our feature flags if we support this feature.
	if (feature_id == kZealFeatureFixBuffStacking_Id) {
		if (feature_value > kZealFeatureFixBuffStacking_Version_1) {
			feature_value = kZealFeatureFixBuffStacking_Version_1;
		}
		buffstacking_fix = feature_value;
	}
	// If the server initiated the handshake, respond to the server with our feature status.
	if (is_request) {
		SendFeatureHandshake(feature_id, feature_value, false);
	}
}

// Used for FeatureHandshake negotiation
BYTE __fastcall HandleSpawnAppearanceMessage_hk(int unk1, int unused_edx, int unk2, int opcode, Zeal::Packets::SpawnAppearance_Struct* sa)
{
	// Intercept any SpawnAppearance packets using our special Zeal handshake type
	if (sa->type == kSpawnAppearanceTypeZealFeature) {
		// Note: The server should send spawn_id 0 for these types of handshakes
		if (sa->spawn_id == 0 || Zeal::EqGame::get_self() && Zeal::EqGame::get_self()->SpawnId == sa->spawn_id) {
			bool is_request = (sa->parameter >> 31) == 0;
			DWORD feature_id = sa->parameter >> 16 & 0x7FFFu;
			DWORD feature_value = sa->parameter & 0xFFFFu;
			HandleFeatureHandshake(feature_id, feature_value, is_request);
		}
		return 1;
	}
	return ZealService::get_instance()->hooks->hook_map["HandleSpawnAppearanceMessage"]->original(HandleSpawnAppearanceMessage_hk)(unk1, unused_edx, unk2, opcode, sa);
}

void onZone() {
	// Fix Buff Stacking - Initiates feature handshakes on every zone, since the ZoneServer changes.
	buffstacking_fix = 0; // Turn off the patch until the Zone acknowledges which setting to use.
	SendFeatureHandshake(kZealFeatureFixBuffStacking_Id, kZealFeatureFixBuffStacking_Version_1, true);
}

// BuffStacking Patch - FindAffectSlot()
// ---------------------------------------------------------------------------------------------------
// Without Patch - Uses caster's class rather than just using the spell's class.
// * is_bard_song = spell->IsBardsong() && caster->Class == Bard
// * is_caster_bard = caster->Class == Bard
// ---------------------------------------------------------------------------------------------------
// With Patch - Uses the spell's class only for better detection.
// * is_bard_song = spell->IsBardsong()
// * is_caster_bard = is_bard_song
// ---------------------------------------------------------------------------------------------------
Zeal::EqStructures::_EQBUFFINFO* __fastcall FindAffectSlot_hk(Zeal::EqStructures::EQCHARINFO* this_, int unused, WORD spellid, Zeal::EqStructures::Entity* caster, DWORD* out_slot, int flag)
{
	// To achieve the above psuedocode, we temporarily set the caster's class to Bard/not-Bard to match whatever spell is being cast.
	// The EQ logic gets the rest correct if we do this.
	if (buffstacking_fix) {
		BYTE origClass = caster->Class;
		caster->Class = 1;
		if (spellid >= 0 && spellid < EQ_NUM_SPELLS) {
			Zeal::EqStructures::SPELL* spell = Zeal::EqGame::get_spell_mgr()->Spells[spellid];
			if (spell && spell->IsBardsong()) {
				caster->Class = 8; // Bard
			}
		}
		buffstacking_in_find_affect_slot = true;
		Zeal::EqStructures::_EQBUFFINFO* result = ZealService::get_instance()->hooks->hook_map["FindAffectSlot"]->original(FindAffectSlot_hk)(this_, unused, spellid, caster, out_slot, flag);
		buffstacking_in_find_affect_slot = false;
		caster->Class = origClass;
		return result;
	}
	return ZealService::get_instance()->hooks->hook_map["FindAffectSlot"]->original(FindAffectSlot_hk)(this_, unused, spellid, caster, out_slot, flag);
}

// BuffStacking Patch - IsStackBlocked()
// ---------------------------------------------------------------------------------------------------
// Without Patch - Very broken!! Uses the target's class instead and the spell's class for detecting a bard song.
// * is_bard_song == spell->IsBardsong() && this->IsBard()
// ---------------------------------------------------------------------------------------------------
// With Patch - Ignores the class of recipient, only cares whether the spell is a bard spell or not.
// * is_bard_song == spell->IsBardsong()
// ---------------------------------------------------------------------------------------------------
bool __fastcall IsStackBlocked_hk(Zeal::EqStructures::EQCHARINFO* this_char_info, int unused, Zeal::EqStructures::SPELL* spell)
{
	// To achieve the above psuedocode, we temporarily set our class to Bard/not-Bard to match whatever spell is being cast.
	// The EQ logic gets the rest correct if we do this.
	if (buffstacking_fix) {
		BYTE origClass = this_char_info->Class;
		this_char_info->Class = spell->IsBardsong() ? 8 : 1;
		bool result = ZealService::get_instance()->hooks->hook_map["IsStackBlocked"]->original(IsStackBlocked_hk)(this_char_info, unused, spell);
		this_char_info->Class = origClass;
		return result;
	}
	return ZealService::get_instance()->hooks->hook_map["IsStackBlocked"]->original(IsStackBlocked_hk)(this_char_info, unused, spell);
}

// BuffStacking Patch - SpellAffectIndex()
// ---------------------------------------------------------------------------------------------------
// Without Patch - Client uses 'is_movement_spell' to apply a lot of special-case logic to block bard movement songs.
// * is_movement_spell = HasSpellEffect(SE_MovementSpeed)
// ---------------------------------------------------------------------------------------------------
// With Patch - We set 'is_movement_spell' to false on bard songs, which bypasses the special-case logic, letting the default (fixed/correct now) stacking logic apply.
// * is_movement_spell = HasSpellEffect(SE_MovementSpeed) && (!is_bard_song || is_detrimental)
// ---------------------------------------------------------------------------------------------------
BYTE __fastcall SpellAffectIndex_hk(Zeal::EqStructures::SPELL* spell, int unused_edx, int effectType) {
	// All the special-case bard logic involves the following check.
	// We just have to return 0/false when queried about whether a bard song is a movement effect, and it effectly bypasses all the weird stacking logic that stops Selos from stacking with JBoots/SoW.
	// The EQ logic handles buff stacking normally once we get it to skip over all this special-case stuff.
	if (buffstacking_fix && buffstacking_in_find_affect_slot) { // Only do this logic while inside FindAffectSlot(..)'s execution
		if (effectType == SE_MovementSpeed && spell->IsBeneficial() && spell->IsBardsong()) {
			return 0;
		}
	}
	return ZealService::get_instance()->hooks->hook_map["SpellAffectIndex"]->original(SpellAffectIndex_hk)(spell, unused_edx, effectType);
}

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
patches::patches()
{
	const char sit_stand_patch[] = { (char)0xEB, (char)0x1A };
	mem::write(0x42d14d, sit_stand_patch); //fix pet sit shortcut crash (makes default return of function the sit/stand button not sure why its passing in 0)
	
	//disable client sided hp ticking
	//mem::set(0x4b9141, 0x90, 6);

	//disable client sided mana ticking
	mem::set(0x4C3F93, 0x90, 7);
	mem::set(0x4C7642, 0x90, 7);

	//the following does not work entirely needs more effort
	//mem::write<byte>(0x4A594B, 15); //load font sizes 1 to 14 (default is 6)
	//mem::write<byte>(0X4FDB6A, 15); //allow /chatfontsize to be larger than 5
	

	ZealService::get_instance()->callbacks->AddGeneric([this]() { ; }, callback_type::InitUI);
	ZealService::get_instance()->callbacks->AddGeneric([]() { onZone(); }, callback_type::Zone);

	mem::write<BYTE>(0x4A14CF, 0xEB); //don't print Your XML files are not compatible with current EverQuest files, certain windows may not perform correctly.  Use "/loadskin Default 1" to load the EverQuest default skin.

	// Client/Server patch handshake support
	ZealService::get_instance()->hooks->Add("HandleSpawnAppearanceMessage", 0x004DF52A, HandleSpawnAppearanceMessage_hk, hook_type_detour);

	// Buffstacking Patches
	ZealService::get_instance()->hooks->Add("FindAffectSlot", 0x004C7A3E, FindAffectSlot_hk, hook_type_detour);
	ZealService::get_instance()->hooks->Add("IsStackBlocked", 0x004C830B, IsStackBlocked_hk, hook_type_detour);
	ZealService::get_instance()->hooks->Add("SpellAffectIndex", 0x004D79C8, SpellAffectIndex_hk, hook_type_detour);

	ZealService::get_instance()->hooks->Add("GetZoneInfoFromNetwork", 0x53D026, GetZoneInfoFromNetwork, hook_type_detour);

	ZealService::get_instance()->hooks->Add("ProcessDeath", 0x00528E16, ProcessDeath, hook_type_detour);
	ZealService::get_instance()->hooks->Add("CanIBreathe", 0x004C0DAB, CanIBreathe, hook_type_detour);

	//fonts();
	//*(int*)0x809458 =
}