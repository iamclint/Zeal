#include "looting.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "StringUtil.h"
//void __fastcall finalize_loot(int uk, int lootwnd_ptr)
//{
//	Zeal::EqStructures::Entity* corpse =  Zeal::EqGame::get_active_corpse();
//	if (ZealService::get_instance()->gui->Options->hidecorpse_looted && corpse)
//		corpse->ActorInfo->IsInvisible = 1;
//	ZealService::get_instance()->hooks->hook_map["FinalizeLoot"]->original(finalize_loot)(uk, lootwnd_ptr);
//}


void looting::set_hide_looted(bool val)
{
	hide_looted = val;
	ZealService::get_instance()->ui->SetChecked("Zeal_HideCorpse", hide_looted);
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "HideLooted", hide_looted);
}


void __fastcall release_loot(int uk, int lootwnd_ptr)
{
	ZealService* zeal = ZealService::get_instance();
	Zeal::EqStructures::Entity* corpse = Zeal::EqGame::get_active_corpse();
	if (corpse && zeal->looting_hook->hide_looted && corpse->Type==2)
	{
		corpse->ActorInfo->IsInvisible = 1; //this is the flag set by /hidecorpse all (so /hidecorpse none will reshow these hidden corpses)
	}
	zeal->hooks->hook_map["ReleaseLoot"]->original(release_loot)(uk, lootwnd_ptr);
}

looting::~looting()
{
}

static int __fastcall LinkAllButtonDown(Zeal::EqUI::LootWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	int rval = reinterpret_cast<int(__fastcall*)(Zeal::EqUI::LootWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)>(0x0595330)(pWnd, unused, pt, flag);
	Zeal::EqUI::ChatWnd* wnd = Zeal::EqGame::Windows->ChatManager->GetActiveChatWindow();
	if (wnd)
	{
		std::stringstream ss;
		for (int i = 0; i < 30; i++)
		{
			if (Zeal::EqGame::Windows->Loot->Item[i])
				ss << "" << std::setw(7) << std::setfill('0') << Zeal::EqGame::Windows->Loot->Item[i]->Id << Zeal::EqGame::Windows->Loot->Item[i]->Name << "";
			if (Zeal::EqGame::Windows->Loot->Item[i + 1] && i+1<30)
				ss << ", ";
		}


		Zeal::EqUI::EditWnd* input_wnd = (Zeal::EqUI::EditWnd*)wnd->edit;
		input_wnd->ReplaceSelection(ss.str(), false);
		input_wnd->SetFocus();
	}
	else
		Zeal::EqGame::print_chat("No active chat window found");
	
	return rval;
}

looting::looting(ZealService* zeal)
{
	hide_looted = zeal->ini->getValue<bool>("Zeal", "HideLooted"); //just remembers the state
	zeal->main_loop_hook->add_callback([this]() {
		Zeal::EqUI::BasicWnd* btn = Zeal::EqGame::Windows->Loot->GetChildItem("LinkAllButton");
		mem::unprotect_memory(&btn, 4);
		if (btn)
		{
			btn->SetupCustomVTable();
			btn->vtbl->HandleLButtonDown = LinkAllButtonDown;
		}
	}, callback_fn::InitUI);
	zeal->commands_hook->add("/hidecorpse", { "/hc", "/hideco", "/hidec" },
		[this](std::vector<std::string>& args) {
			if (args.size() > 1 && StringUtil::caseInsensitive(args[1], "looted"))
			{
				set_hide_looted(!hide_looted);
				if (hide_looted)
					Zeal::EqGame::print_chat("Corpses will be hidden after looting.");
				else
					Zeal::EqGame::print_chat("Corpses will no longer be hidden after looting.");
				return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			if (args.size() > 1 && args[1] == "none")
			{
				set_hide_looted(false);
				return false; 
			}
			return false;
		});
	
	zeal->hooks->Add("ReleaseLoot", Zeal::EqGame::EqGameInternal::fn_releaseloot, release_loot, hook_type_detour);
}
