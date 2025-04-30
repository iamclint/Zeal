#include "labels.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "EqUI.h"
#include "Zeal.h"

void default_empty(Zeal::EqUI::CXSTR* str, bool* override_color, ULONG* color)
{
	*override_color = 1;
	*color = 0xffc0c0c0;
	Zeal::EqGame::CXStr_PrintString(str, "");
}


bool GetLabelFromEq(int EqType, Zeal::EqUI::CXSTR* str, bool* override_color, ULONG* color)
{
	ZealService* zeal = ZealService::get_instance();
	if (!Zeal::EqGame::is_in_game())
		return ZealService::get_instance()->hooks->hook_map["GetLabel"]->original(GetLabelFromEq)(EqType, str, override_color, color);
	switch (EqType)
	{
	case 80:
	{
		if (!zeal->experience && Zeal::EqGame::get_char_info())
			return true;
		int max_mana = Zeal::EqGame::get_char_info()->max_mana();//  Zeal::EqGame::EqGameInternal::get_max_mana(*Zeal::EqGame::ptr_LocalPC, 0);
		int mana = Zeal::EqGame::get_char_info()->mana(); //Zeal::EqGame::EqGameInternal::get_cur_mana(*Zeal::EqGame::ptr_LocalPC, 0);
		Zeal::EqGame::CXStr_PrintString(str, "%d/%d", mana, max_mana);
		*override_color = false;
		return true;
	}
	case 81:
	{
		if (!zeal->experience)
			return true;
		Zeal::EqGame::CXStr_PrintString(str, "%.f", zeal->experience->exp_per_hour_pct_tot);
		*override_color = false;
		return true;
	}
	case 82:
	{
		Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
		if (target && target->PetOwnerSpawnId > 0)
		{
			Zeal::EqStructures::Entity* owner = Zeal::EqGame::get_entity_by_id(target->PetOwnerSpawnId);
			if (owner)
			{
				Zeal::EqGame::CXStr_PrintString(str, "%s", owner->Name);
				*override_color = false;
			}
		}
		else
		{
			default_empty(str, override_color, color);
		}
		return true;
	}
	case 83:
		Zeal::EqGame::CXStr_PrintString(str, "%d", Zeal::EqGame::get_num_empty_inventory_slots());
		*override_color = false;
		return true;
	case 84:
		Zeal::EqGame::CXStr_PrintString(str, "%d", Zeal::EqGame::get_num_inventory_slots());
		*override_color = false;
		return true;
	case 124:
	{
		if (Zeal::EqGame::get_char_info())
			Zeal::EqGame::CXStr_PrintString(str, "%d", Zeal::EqGame::get_char_info()->mana());
		*override_color = false;
		return true;
	}
	case 125:
	{
		if (Zeal::EqGame::get_char_info())
			Zeal::EqGame::CXStr_PrintString(str, "%d", Zeal::EqGame::get_char_info()->max_mana());
		*override_color = false;
		return true;
	}
	case 134:
	{
		if (Zeal::EqGame::get_controlled() && Zeal::EqGame::get_controlled()->ActorInfo)
		{
			if (Zeal::EqGame::get_controlled()->ActorInfo->CastingSpellId) {
				int spell_id = Zeal::EqGame::get_controlled()->ActorInfo->CastingSpellId;
				if (spell_id == kInvalidSpellId) spell_id = 0; // avoid crash while player is not casting a spell
				Zeal::EqStructures::SPELL* casting_spell = Zeal::EqGame::get_spell_mgr()->Spells[spell_id];
				Zeal::EqGame::CXStr_PrintString(str, "%s", casting_spell->Name);
				*override_color = false;
			}
			
		}
		return true;
	}
	case 135: // Buff 16
	case 136: // Buff 17
	case 137: // Buff 18
	case 138: // Buff 19
	case 139: // Buff 20
	case 140: // Buff 21
	case 141: // Buff 22
	case 142: // Buff 23
	case 143: // Buff 24
	case 144: // Buff 25
	case 145: // Buff 26
	case 146: // Buff 27
	case 147: // Buff 28
	case 148: // Buff 29
	case 149: // Buff 30
		break; // Reserved - These labels are supported by the eqgame.dll
	case 255: //debug label
	{
		Zeal::EqGame::CXStr_PrintString(str, "%s", ZealService::get_instance()->labels_hook->debug_info.c_str());
		ZealService::get_instance()->labels_hook->debug_info = "";
		*override_color = false;
		return true;
	}
	default:
		break;
	}
	return ZealService::get_instance()->hooks->hook_map["GetLabel"]->original(GetLabelFromEq)(EqType, str, override_color, color);
}

static int get_remaining_cast_recovery_time()
{
	auto self = Zeal::EqGame::get_self();
	auto actor_info = self ? self->ActorInfo : nullptr;
	int* this_display = *(int**)Zeal::EqGame::Display;
	if (!self || !actor_info || !this_display)
		return 0;

	int game_time = this_display[200 / 4];  // The client uses this display offset as a timestamp.
	if (actor_info->FizzleTimeout <= game_time)
		return 0;  // Idle state.

	int time_left = max(0, actor_info->FizzleTimeout - game_time);
	return time_left;
}

static int get_attack_timer_gauge(Zeal::EqUI::CXSTR* str)
{
	// Logic for the attack recovery timer was copied from DoProcessTime() which sets 0x007cd844.
	auto self = Zeal::EqGame::get_self();
	auto actor_info = self ? self->ActorInfo : nullptr;
	auto char_info = Zeal::EqGame::get_char_info();
	bool ready_to_attack = *reinterpret_cast<bool*>(0x007cd844);  // 0 = attacking, 1 = ready.
	if (!self || !actor_info || !char_info || ready_to_attack) {
		if (str)
			str->Set("");
		return 0;
	}

	UINT range_delay = 0;
	bool is_bow = false;
	if (actor_info->LastAttack == 11) {  // Ranged.
		// Calculate ranged time.
		auto range_item = char_info->InventoryItem[10];  // Ranged slot.
		if (range_item && range_item->Common.AttackDelay) {
			if (range_item->Common.Skill < 6 || range_item->Common.Skill == 0xd)
				range_delay = range_item->Common.AttackDelay * 100;
			else if (range_item->Common.Skill == 0x16)
				range_delay = reinterpret_cast<UINT**>(0x007f7aec)[range_item->Common.AttackDelay][1];
			is_bow = range_delay && (range_item->Common.Skill == 5);
		}
	}

	UINT attack_delay = range_delay;  // Use range_delay if it was set non-zero above.
	if (attack_delay == 0) {
		auto primary_item = char_info->InventoryItem[12];  // Primary slot.
		if (!primary_item || !primary_item->Common.AttackDelay)  // No weapon or not a weapon.
			attack_delay = Zeal::EqGame::get_hand_to_hand_delay() * 100;
		else if (primary_item->Common.Skill < 6 || primary_item->Common.Skill == 0xd)
			attack_delay = primary_item->Common.AttackDelay * 100;
		else if (primary_item->Common.Skill == 0x16)  // Hand-to-hand skilldict lookup.
			attack_delay = reinterpret_cast<UINT**>(0x007f7aec)[primary_item->Common.AttackDelay][1];
	}

	if (attack_delay)
		attack_delay = self->ModifyAttackSpeed(attack_delay, is_bow);

	UINT delay_time = Zeal::EqGame::get_eq_time() - actor_info->AttackTimer;
	if (attack_delay == 0 || attack_delay <= delay_time) {
		if (str)
			str->Set("");
		return 0;
	}

	int time_left = attack_delay - delay_time;
	if (str) {
		int secs_left = (time_left + 999) / 1000;  // Show 3, 2, 1, 0 countdown effectively.
		Zeal::EqGame::CXStr_PrintString(str, "%i", secs_left);
	}

	const int full_duration = attack_delay;  // Use 4 seconds as the normalization factor.
	return max(0, min(1000, 1000 * time_left / attack_delay));
}

static int get_recast_time_gauge(int index, Zeal::EqUI::CXSTR* str)
{
	bool invalid_index = index < 0 || index >= EQ_NUM_SPELL_GEMS;

	auto self = Zeal::EqGame::get_self();
	auto actor_info = self ? self->ActorInfo : nullptr;
	auto char_info = Zeal::EqGame::get_char_info();
	int* this_display = *(int**)Zeal::EqGame::Display;
	if (invalid_index || !self || !actor_info || !char_info || !this_display) {
		if (str)
			str->Set("0");
		return 0;
	}

	// Empty gauge if recast timeout is < current game time or the fizzle timeout (GCD).
	int game_time = this_display[200 / 4];  // The client uses this display offset as a timestamp.
	int spell_id = char_info->MemorizedSpell[index];
	if (!Zeal::EqGame::Spells::IsValidSpellIndex(spell_id) ||
		actor_info->CastingSpellId == spell_id ||
		actor_info->RecastTimeout[index] <= game_time ||
		actor_info->RecastTimeout[index] <= actor_info->FizzleTimeout) {
		if (str)
			str->Set("0");
		return 0;
	}

	int time_left = actor_info->RecastTimeout[index] - game_time;
	if (str) {
		int secs_left = (time_left + 500) / 1000;
		Zeal::EqGame::CXStr_PrintString(str, "%i", secs_left);
	}

	auto sp_mgr = Zeal::EqGame::get_spell_mgr();
	int full_duration = sp_mgr ? sp_mgr->Spells[spell_id]->RecastTime : 0;
	full_duration = max(1000, full_duration);  // Ensure non-zero and reasonable number.
	return max(0, min(1000, 1000 * time_left / full_duration));
}

int GetGaugeFromEq(int EqType, Zeal::EqUI::CXSTR* str)
{
	ZealService* zeal = ZealService::get_instance();
	switch (EqType)
	{
		case 23:
		{
			if (!zeal->experience) //possible nullptr crash (race condition)
				return 0;
			float fpct = zeal->experience->exp_per_hour_pct_tot / 100.f;
			return (int)(1000.f * fpct);
		}
		case 24: // Server Tick
		{
			if (zeal->tick)
				return zeal->tick->GetTickGauge(str);
			return 0;
		}
		case 25:  // Global cast recovery gauge.
		{
			static constexpr int kNominalMaxRecoveryTime = 2500; // In milliseconds.
			int recovery_time = get_remaining_cast_recovery_time();
			int value = max(0, min(1000, recovery_time * 1000 / kNominalMaxRecoveryTime));
			Zeal::EqGame::CXStr_PrintString(str, "%i", (recovery_time + 500) / 1000);
			return value;
		}
		case 26:  // Spell0 recast time.
		case 27:  // Spell1 recast time.
		case 28:  // Spell2 recast time.
		case 29:  // Spell3 recast time.
		case 30:  // Spell4 recast time.
		case 31:  // Spell5 recast time.
		case 32:  // Spell6 recast time.
		case 33:  // Spell7 recast time.
			return get_recast_time_gauge(EqType - 26, str);
		case 34:  // Attack recovery timer.
			return get_attack_timer_gauge(str);
		default:
			break;
	}

	int result = ZealService::get_instance()->hooks->hook_map["GetGauge"]->original(GetGaugeFromEq)(EqType, str);

	switch (EqType)
	{
		case 11:  // Intercept the player HP gauges (group window typically) to tag the leader.
		case 12:
		case 13:
		case 14:
		case 15:
		{
			const Zeal::EqStructures::GroupInfo* group_info = Zeal::EqGame::GroupInfo;
			if (group_info->is_in_group() && strcmp(str->CastToCharPtr(), group_info->LeaderName) == 0)
			{
				std::string name = std::string(*str) + "*";
				str->Set(name.c_str());
			}
		}
		break;
		default:
			break;
	}
	return result;
}

void labels::print_debug_info(std::string data)
{
	debug_info = data;
}
void labels::print_debug_info(const char* format, ...)
{
	va_list argptr;
	char buffer[512];
	va_start(argptr, format);
	//printf()
	vsnprintf(buffer, 511, format, argptr);
	va_end(argptr);
	if (debug_info.length()>0)
		debug_info += "\n" + std::string(buffer);
	else
		debug_info += std::string(buffer);
}


void labels::callback_main()
{

	
}

bool labels::GetLabel(int EqType, std::string& str)
{
	Zeal::EqUI::CXSTR tmp("");
	bool override = false;
	ULONG color = 0;
	bool val = GetLabelFromEq(EqType, (Zeal::EqUI::CXSTR*)&tmp, &override, &color);
	if (tmp.Data)
	{
		str = std::string(tmp);
		tmp.FreeRep();
	}
	return val;
}
int labels::GetGauge(int EqType, std::string& str)
{
	Zeal::EqUI::CXSTR tmp("");
	int value = GetGaugeFromEq(EqType, (Zeal::EqUI::CXSTR*)&tmp);
	if (tmp.Data)
	{
		str = std::string(tmp);
		tmp.FreeRep();
	}
	return value;
}


labels::~labels()
{

}

labels::labels(ZealService* zeal)
{
	zeal->commands_hook->Add("/labels", {}, "prints all labels",
		[this](std::vector<std::string>& args) {
			for (int i = 0; i < 200; i++)
			{
				Zeal::EqUI::CXSTR tmp("");
				bool override = false;
				ULONG color = 0;
				GetLabelFromEq(i, (Zeal::EqUI::CXSTR*)&tmp, &override, &color);
				if (tmp.Data) {
					Zeal::EqGame::print_chat("label: %i value: %s", i, tmp.CastToCharPtr());
					tmp.FreeRep();
				}
			}
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	// zeal->callbacks->add_generic([this]() { callback_main(); }); //causes a crash because callback_main is empty
	//zeal->hooks->Add("FinalizeLoot", Zeal::EqGame::EqGameInternal::fn_finalizeloot, finalize_loot, hook_type_detour);
	zeal->hooks->Add("GetLabel", Zeal::EqGame::EqGameInternal::fn_GetLabelFromEQ, GetLabelFromEq, hook_type_detour);
	zeal->hooks->Add("GetGauge", Zeal::EqGame::EqGameInternal::fn_GetGaugeLabelFromEQ, GetGaugeFromEq, hook_type_detour);
}
