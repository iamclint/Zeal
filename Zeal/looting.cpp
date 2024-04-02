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
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "HideLooted", hide_looted);
	ZealService::get_instance()->ui->options->UpdateOptions();
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

void looting::init_ui()
{
}

struct formatted_msg
{
	UINT16 unk;
	UINT16 string_id;
	UINT16 type;
	char message[0];
};

void looting::looted_item()
{
	if (loot_all && Zeal::EqGame::Windows && Zeal::EqGame::Windows->Loot && Zeal::EqGame::Windows->Loot->IsVisible)
	{
		bool loot_nodrop = (Zeal::EqGame::get_active_corpse() == Zeal::EqGame::get_controlled()); //my own corpse
		byte nodrop_confirm_bypass[2] = { 0x74, 0x22 };
		for (int i = 0; i < 30; i++)
		{
			bool loot = false;
			if (Zeal::EqGame::Windows->Loot->Item[i])
			{
				if (loot_nodrop)
					loot = true;
				else if (Zeal::EqGame::Windows->Loot->Item[i]->NoDrop != 0)
					loot = true;

				if (loot)
				{
					if (loot_nodrop)
						mem::set(0x426beb, 0x90, 2);
					Zeal::EqGame::Windows->Loot->RequestLootSlot(i, true);
					if (loot_nodrop)
						mem::copy(0x426beb, nodrop_confirm_bypass, 2);
					return;
				}
			}
		}
		loot_all = false;
	}
	else
		loot_all = false;
}

looting::looting(ZealService* zeal)
{
	hide_looted = zeal->ini->getValue<bool>("Zeal", "HideLooted"); //just remembers the state
	zeal->callbacks->add_callback([this]() { init_ui(); }, callback_fn::InitUI);
	zeal->callbacks->add_callback([this]() {
		if (!Zeal::EqGame::Windows || !Zeal::EqGame::Windows->Loot || !Zeal::EqGame::Windows->Loot->IsVisible)
		{
			loot_all = false;
			return;
		}
		}, callback_fn::MainLoop);
		zeal->callbacks->add_worldmessage_callback([this](UINT opcode, char* buffer, UINT len) {
		if (opcode == 0x4236)
		{
			formatted_msg* data = (formatted_msg*)buffer;
			if (data->string_id == 467) //467 --You have looted a %1.--
				looted_item();
		}
		return false; 
		});
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
	if (Zeal::EqGame::is_in_game())
		init_ui();
}
