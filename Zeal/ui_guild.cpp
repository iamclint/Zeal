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
}

ui_guild::ui_guild(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->add_generic([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->add_generic([this]() { InitUI(); }, callback_type::InitUI);
			zeal->commands_hook->add("/guildwindow", { "" }, "",
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
