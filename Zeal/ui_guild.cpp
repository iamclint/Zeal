#include "ui_guild.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>



void ui_guild::CleanUI()
{

}

void ui_guild::InitUI()
{
	if (guild)
		delete guild;
	guild = new Zeal::EqUI::BasicWnd();
	guild->vtbl = (Zeal::EqUI::BaseVTable*)0x5e60f0;
	reinterpret_cast<int* (__thiscall*)(Zeal::EqUI::BasicWnd*, Zeal::EqUI::BasicWnd*, Zeal::EqUI::CXSTR name, int, int)>(0x56e1e0)(guild, 0, Zeal::EqUI::CXSTR("GuildManagementWnd"), -1, 0);
	guild->CreateChildren();
	members = (Zeal::EqUI::ListWnd*)guild->GetChildItem("MemberList");
}

ui_guild::ui_guild(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->add_generic([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->add_generic([this]() { InitUI(); }, callback_type::InitUI);
		zeal->commands_hook->add("/additem", {}, "Toggle guild management window",
		[this, mgr](std::vector<std::string>& args) {
			if (members)
			{
				mgr->AddListItems(members, { 
					{"Test Name", "65", "Officer", "Ranger", "10/11/1999", "ECommonlands", "What a loser"},
					{"Test Name2", "65", "Officer", "Ranger", "10/11/1999", "ECommonlands", "What a loser"},
					{"Test Name3", "65", "Officer", "Enchanter", "10/11/1999", "ECommonlands", "What a loser"},
					{"Test Name4", "65", "Officer", "Ranger", "10/11/1999", "East Commonlands", "What a loser"} 
				});
			}
			else
			{
				Zeal::EqGame::print_chat("Guild window not found!");
			}
			return true;
		});
		zeal->commands_hook->add("/guildwindow", {}, "Toggle guild management window",
			[this](std::vector<std::string>& args) {
					if (guild)
					{
						Zeal::EqGame::print_chat("Attempting to show guild window");
						guild->IsVisible=true;
					}
					else
					{
						Zeal::EqGame::print_chat("Guild window not found!");
					}
				return true;
			});
	//if (Zeal::EqGame::is_in_game()) InitUI();
}
ui_guild::~ui_guild()
{
}
