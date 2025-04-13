#include "ui_inspect.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "items.h"
#include <format>


static int __fastcall InspectItemClickDown(Zeal::EqUI::InvSlotWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	if (pWnd->invSlot && pWnd->invSlot->Item && Zeal::EqGame::get_wnd_manager()->AltKeyState)
	{
		short id = Zeal::Items::lookup(pWnd->invSlot->Item->Name);
		if (id > 0)
		{
			Zeal::Packets::ItemViewRequest_Struct tmp;
			memset(&tmp, 0, sizeof(tmp));
			tmp.item_id = id;
			Zeal::EqGame::send_message(Zeal::Packets::opcodes::ItemLinkResponse, (int*)&tmp, sizeof(tmp), 0);
		}
		else
			Zeal::EqGame::print_chat("Unable to find id for item: %s", pWnd->invSlot->Item->Name);
	}
	return 0;
}

void ui_inspect::InitUI()
{
	for (int i = 1; i < 22; i++)
	{
		std::string slot_name = std::format("InvSlot{}", i);
		Zeal::EqUI::InvSlotWnd* wnd = (Zeal::EqUI::InvSlotWnd*)Zeal::EqGame::Windows->Inspect->GetChildItem(slot_name);
		if (wnd)
		{
			wnd->SetupCustomVTable(sizeof(Zeal::EqUI::ButtonWndVTable));
			wnd->vtbl->HandleLButtonDown = InspectItemClickDown;
		}
	}
}
ui_inspect::ui_inspect(ZealService* zeal, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
}
ui_inspect::~ui_inspect()
{
}
