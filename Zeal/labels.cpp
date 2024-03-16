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
	switch (EqType)
	{
	case 80:
	{
		if (!zeal->experience)
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
		Zeal::EqGame::CXStr_PrintString(str, "%d", Zeal::EqGame::get_char_info()->mana());
		*override_color = false;
		return true;
	}
	case 125:
	{
		Zeal::EqGame::CXStr_PrintString(str, "%d", Zeal::EqGame::get_char_info()->max_mana());
		*override_color = false;
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

}

labels::~labels()
{

}

labels::labels(ZealService* zeal)
{
	zeal->main_loop_hook->add_callback([this]() { callback_main(); });
	//zeal->hooks->Add("FinalizeLoot", Zeal::EqGame::EqGameInternal::fn_finalizeloot, finalize_loot, hook_type_detour);
	zeal->hooks->Add("GetLabel", Zeal::EqGame::EqGameInternal::fn_GetLabelFromEQ, GetLabelFromEq, hook_type_detour);
	zeal->hooks->Add("GetGauge", Zeal::EqGame::EqGameInternal::fn_GetGaugeLabelFromEQ, GetGaugeFromEq, hook_type_detour);
}
