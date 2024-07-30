#include "ui_bank.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"


void ui_bank::change()
{
	if (Zeal::EqGame::Windows->Bank && Zeal::EqGame::Windows->Bank->IsVisible)
	{
		if (Zeal::EqGame::get_char_info()->BankGold > 0)
		{
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 1, Zeal::EqGame::get_char_info()->BankGold);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 2, -1);
		}
		if (Zeal::EqGame::get_char_info()->BankSilver > 0)
		{
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 2, Zeal::EqGame::get_char_info()->BankSilver);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 3, -1);
		}
		if (Zeal::EqGame::get_char_info()->BankCopper > 0)
		{
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 3, Zeal::EqGame::get_char_info()->BankCopper);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 0, -1);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 1, -1);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 2, -1);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x404aec)(Zeal::EqGame::Windows->Bank, 3, -1);
		}

		if (Zeal::EqGame::get_char_info()->Gold > 0)
		{
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 1, Zeal::EqGame::get_char_info()->Gold);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 2, -1);
		}
		if (Zeal::EqGame::get_char_info()->Silver > 0)
		{
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 2, Zeal::EqGame::get_char_info()->Silver);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 3, -1);
		}
		if (Zeal::EqGame::get_char_info()->Copper > 0)
		{
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 3, Zeal::EqGame::get_char_info()->Copper);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 0, -1);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 1, -1);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 2, -1);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::BasicWnd*, int, int)>(0x421876)(Zeal::EqGame::Windows->Inventory, 3, -1);
		}
	}
}

static int __fastcall ChangeButtonDown(Zeal::EqUI::BasicWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	int rval = reinterpret_cast<int(__fastcall*)(Zeal::EqUI::BasicWnd * pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)>(0x0595330)(pWnd, unused, pt, flag);
	
	ZealService::get_instance()->ui->bank->change();
	return rval;
}

void ui_bank::InitUI()
{
	Zeal::EqUI::BasicWnd* btn = Zeal::EqGame::Windows->Bank->GetChildItem("ChangeButton");
	if (btn)
	{
		btn->SetupCustomVTable();
		btn->vtbl->HandleLButtonDown = ChangeButtonDown;
	}
}
ui_bank::ui_bank(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	if (Zeal::EqGame::is_in_game()) InitUI();
}
ui_bank::~ui_bank()
{

}
