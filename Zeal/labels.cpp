#include "labels.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"


bool GetLabelFromEq(int EqType, Zeal::EqStructures::CXSTR* str, int* unk, ULONG* color)
{
	ZealService* zeal = ZealService::get_instance();
	switch (EqType)
	{
	case 80:
	{
		int max_mana = Zeal::EqGame::EqGameInternal::get_max_mana(*Zeal::EqGame::ptr_LocalPC, 0);
		int mana = Zeal::EqGame::EqGameInternal::get_cur_mana(*Zeal::EqGame::ptr_LocalPC, 0);
		Zeal::EqGame::CXStr_PrintString(str, "%d/%d", mana, max_mana);
		*unk = 1;
		*color = 0xffc0c0c0;
		return true;
	}
	case 81:
	{
		Zeal::EqGame::CXStr_PrintString(str, "%.f", zeal->experience->exp_per_hour_pct_tot);
		*unk = 1;
		*color = 0xffc0c0c0;
		return true;
	}
	default:
		break;
	}
	return ZealService::get_instance()->hooks->hook_map["GetLabel"]->original(GetLabelFromEq)(EqType, str, unk, color);
}

int GetGaugeFromEq(int EqType, Zeal::EqStructures::CXSTR* str)
{
	ZealService* zeal = ZealService::get_instance();
	switch (EqType)
	{
		case 23:
		{
			float fpct = zeal->experience->exp_per_hour_pct_tot / 100.f;
			return 1000.f * fpct;
		}
		default:
			break;
	}

	return ZealService::get_instance()->hooks->hook_map["GetGauge"]->original(GetGaugeFromEq)(EqType, str);
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
	zeal->hooks->Add("GetLabel", Zeal::EqGame::EqGameInternal::fn_GetLabelFromEQ, GetLabelFromEq, hook_type_detour, 6);
	zeal->hooks->Add("GetGauge", Zeal::EqGame::EqGameInternal::fn_GetGaugeLabelFromEQ, GetGaugeFromEq, hook_type_detour);
}
