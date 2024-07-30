#include "ui_loot.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
static int __fastcall LinkAllButtonDown(Zeal::EqUI::LootWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	int rval = reinterpret_cast<int(__fastcall*)(Zeal::EqUI::LootWnd * pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)>(0x0595330)(pWnd, unused, pt, flag);
	Zeal::EqUI::ChatWnd* wnd = Zeal::EqGame::Windows->ChatManager->GetActiveChatWindow();
	if (wnd)
	{
		Zeal::EqUI::EditWnd* input_wnd = (Zeal::EqUI::EditWnd*)wnd->edit;
		std::stringstream ss;
		bool has_added_link = false;
		for (int i = 0; i < 30; i++)
		{
			if (Zeal::EqGame::Windows->Loot->Item[i] && input_wnd->item_link_count < 0xA)
			{
				if (has_added_link)
					input_wnd->ReplaceSelection(", ", false);
				input_wnd->AddItemTag(Zeal::EqGame::Windows->Loot->Item[i]->ID, Zeal::EqGame::Windows->Loot->Item[i]->Name);
				has_added_link = true;
			}
		}
		input_wnd->SetFocus();
	}
	else
		Zeal::EqGame::print_chat("No active chat window found");

	return rval;
}

static int __fastcall LootAllButtonDown(Zeal::EqUI::LootWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	int rval = reinterpret_cast<int(__fastcall*)(Zeal::EqUI::LootWnd * pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)>(0x0595330)(pWnd, unused, pt, flag);
	ZealService* zeal = ZealService::get_instance();
	zeal->looting_hook->loot_all = true;
	zeal->looting_hook->looted_item();
	return rval;
}

void ui_loot::InitUI()
{
	Zeal::EqUI::BasicWnd* btn = Zeal::EqGame::Windows->Loot->GetChildItem("LinkAllButton");
	if (btn)
	{
		btn->SetupCustomVTable();
		btn->vtbl->HandleLButtonDown = LinkAllButtonDown;
	}

	btn = Zeal::EqGame::Windows->Loot->GetChildItem("LootAllButton");
	if (btn)
	{
		btn->SetupCustomVTable();
		btn->vtbl->HandleLButtonDown = LootAllButtonDown;
	}
}
ui_loot::ui_loot(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	if (Zeal::EqGame::is_in_game()) InitUI();
	/*zeal->callbacks->add_packet([this](UINT opcode, char* buffer, UINT size) {
		Zeal::EqGame::print_chat("Opcode: 0x%x Size: %i Buffer: %s", opcode, size, StringUtil::byteArrayToHexString(buffer, size).c_str());
		 return false;
	}, callback_type::SendMessage_);*/
}
ui_loot::~ui_loot()
{

}
