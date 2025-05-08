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

static void __fastcall ButtonDestructor(Zeal::EqUI::BasicWnd* pWnd, int unusedEDX, byte delete_me)
{
	// Deletes the custom vtable and swaps back to the CButtonWnd default vtable before calling the Deconstructor.
	// The CButtonWnd default deconstructor basically does the same table swap to the default a few lines in.
	pWnd->DeleteCustomVTable(reinterpret_cast<Zeal::EqUI::BaseVTable*>(0x005eaf00));  // CButtonWnd::vtable
	pWnd->Deconstruct(delete_me);  // Calls the vtable's destructor.
}

// The InitUI is called immediately after all new UI objects are allocated and initialized. The
// ButtonDestructor should get called by the normal framework cleanup in CleanUI.
void ui_bank::InitUI()
{
	// The ChangeButton is optional in the XML files so support the case where it is not found.
	Zeal::EqUI::BasicWnd* btn = Zeal::EqGame::Windows->Bank->GetChildItem("ChangeButton");
	if (btn)
	{
		btn->SetupCustomVTable(sizeof(Zeal::EqUI::ButtonWndVTable));
		btn->vtbl->HandleLButtonDown = ChangeButtonDown;
		btn->vtbl->Deconstructor = ButtonDestructor;
	}
}
ui_bank::ui_bank(ZealService* zeal, UIManager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
}
ui_bank::~ui_bank()
{

}
