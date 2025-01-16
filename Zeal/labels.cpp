#include "labels.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "EqUI.h"
#include "Zeal.h"
#include "json.hpp"

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
				str->FreeRep();
				*str = Zeal::EqUI::CXSTR(name.c_str());
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
				if (tmp.Data)
					Zeal::EqGame::print_chat("label: %i value: %s", i, tmp.CastToCharPtr());
			}
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	// zeal->callbacks->add_generic([this]() { callback_main(); }); //causes a crash because callback_main is empty
	//zeal->hooks->Add("FinalizeLoot", Zeal::EqGame::EqGameInternal::fn_finalizeloot, finalize_loot, hook_type_detour);
	zeal->hooks->Add("GetLabel", Zeal::EqGame::EqGameInternal::fn_GetLabelFromEQ, GetLabelFromEq, hook_type_detour);
	zeal->hooks->Add("GetGauge", Zeal::EqGame::EqGameInternal::fn_GetGaugeLabelFromEQ, GetGaugeFromEq, hook_type_detour);
}
