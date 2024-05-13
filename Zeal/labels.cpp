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


const std::map<int, std::string> LabelNames = {
	{1, "Name"},
	{2, "Level"},
	{3, "Class"},
	{4, "Deity"},
	{5, "Strength"},
	{6, "Stamina"},
	{7, "Dexterity"},
	{8, "Agility"},
	{9, "Wisdom"},
	{10, "Intelligence"},
	{11, "Charisma"},
	{12, "SaveVsPoison"},
	{13, "SaveVsDisease"},
	{14, "SaveVsFire"},
	{15, "SaveVsCold"},
	{16, "SaveVsMagic"},
	{17, "CurrentHP"},
	{18, "MaxHP"},
	{19, "HPPerc"},
	{20, "ManaPerc"},
	{21, "STAPerc"},
	{22, "CurrentMitigation"},
	{23, "CurrentOffense"},
	{24, "Weight"},
	{25, "MaxWeight"},
	{26, "ExpPerc"},
	{27, "AltExpPerc"},
	{28, "TargetName"},
	{29, "TargetHPPerc"},
	{30, "GroupMember1Name"},
	{31, "GroupMember2Name"},
	{32, "GroupMember3Name"},
	{33, "GroupMember4Name"},
	{34, "GroupMember5Name"},
	{35, "GroupMember1HPPerc"},
	{36, "GroupMember2HPPerc"},
	{37, "GroupMember3HPPerc"},
	{38, "GroupMember4HPPerc"},
	{39, "GroupMember5HPPerc"},
	{40, "GroupPet1HPPerc"},
	{41, "GroupPet2HPPerc"},
	{42, "GroupPet3HPPerc"},
	{43, "GroupPet4HPPerc"},
	{44, "GroupPet5HPPerc"},
	{45, "Buff0"},
	{46, "Buff1"},
	{47, "Buff2"},
	{48, "Buff3"},
	{49, "Buff4"},
	{50, "Buff5"},
	{51, "Buff6"},
	{52, "Buff7"},
	{53, "Buff8"},
	{54, "Buff9"},
	{55, "Buff10"},
	{56, "Buff11"},
	{57, "Buff12"},
	{58, "Buff13"},
	{59, "Buff14"},
	{60, "Spell1XMLName0"},
	{61, "Spell2XMLName1"},
	{62, "Spell3XMLName2"},
	{63, "Spell4XMLName3"},
	{64, "Spell5XMLName4"},
	{65, "Spell6XMLName5"},
	{66, "Spell7XMLName6"},
	{67, "Spell8XMLName7"},
	{68, "PlayerPetName"},
	{69, "PlayerPetHPPerc"},
	{70, "PlayerCurrentHPMaxHP"},
	{71, "CurrentAAPoints"},
	{72, "CurrentAAPerc"},
	{73, "LastName"},
	{74, "Title"},
	{80, "Mana/MaxMana"},
	{81, "ExpPH"},
	{82, "TargetPetOwner"},
	{124, "Mana"},
	{125, "MaxMana"},
	{134, "CastingName"},
};
const std::map<int, std::string> GaugeNames = {
   {1, "HP"},
   {2, "Mana"},
   {3, "Stamina"},
   {4, "Experience"},
   {5, "AltExp"},
   {6, "Target"},
   {7, "Casting"},
   {8, "Breath"},
   {9, "Memorize"},
   {10, "Scribe"},
   {11, "Group1HP"},
   {12, "Group2HP"},
   {13, "Group3HP"},
   {14, "Group4HP"},
   {15, "Group5HP"},
   {16, "PetHP"},
   {17, "Group1PetHP"},
   {18, "Group2PetHP"},
   {19, "Group3PetHP"},
   {20, "Group4PetHP"},
   {21, "Group5PetHP"},
   {23, "ExpPerHR"}
};

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
				if (spell_id == 65535) spell_id = 0; // avoid crash while player is not casting a spell
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

	return ZealService::get_instance()->hooks->hook_map["GetGauge"]->original(GetGaugeFromEq)(EqType, str);
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
	static auto last_output = GetTickCount64();
	if (GetTickCount64() - last_output > 500)
	{
		for (auto& [id, name] : LabelNames)
		{
			Zeal::EqUI::CXSTR tmp("");
			bool override = false;
			ULONG color = 0;
			GetLabelFromEq(id, (Zeal::EqUI::CXSTR*)&tmp, &override, &color);
			if (tmp.Data)
			{
				nlohmann::json data = { {"type", id}, {"value", tmp.Data->Text} };
				ZealService::get_instance()->pipe->write(data.dump(), pipe_data_type::label);
			}
		}
		for (auto& [id, name] : GaugeNames)
		{
			Zeal::EqUI::CXSTR tmp("");
			bool override = false;
			ULONG color = 0;
			GetGaugeFromEq(id, (Zeal::EqUI::CXSTR*)&tmp);
			if (tmp.Data)
			{
				nlohmann::json data = { {"type", id}, {"value", tmp.Data->Text} };
				ZealService::get_instance()->pipe->write(data.dump(), pipe_data_type::gauge);
			}
		}
		last_output = GetTickCount64();
	}
	
}

labels::~labels()
{

}

labels::labels(ZealService* zeal)
{
	zeal->commands_hook->add("/labels", {}, "prints all labels",
		[this](std::vector<std::string>& args) {
			for (int i = 0; i < 200; i++)
			{
				Zeal::EqUI::CXSTR tmp("");
				bool override = false;
				ULONG color = 0;
				GetLabelFromEq(i, (Zeal::EqUI::CXSTR*)&tmp, &override, &color);
				if (tmp.Data)
					Zeal::EqGame::print_chat("label: %i value: %s", i, tmp.Data->Text);
			}
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	zeal->callbacks->add_generic([this]() { callback_main(); });
	//zeal->hooks->Add("FinalizeLoot", Zeal::EqGame::EqGameInternal::fn_finalizeloot, finalize_loot, hook_type_detour);
	zeal->hooks->Add("GetLabel", Zeal::EqGame::EqGameInternal::fn_GetLabelFromEQ, GetLabelFromEq, hook_type_detour);
	zeal->hooks->Add("GetGauge", Zeal::EqGame::EqGameInternal::fn_GetGaugeLabelFromEQ, GetGaugeFromEq, hook_type_detour);
}
