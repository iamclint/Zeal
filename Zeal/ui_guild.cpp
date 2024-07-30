#include "ui_guild.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>



void ui_guild::CleanUI()
{
	Zeal::EqGame::print_debug("Clean UI GUILD");
	if (guild)
	{
		guild->IsVisible = false;
		delete guild;
	}
	members = nullptr;
}

void ui_guild::InitUI()
{
	guild = new Zeal::EqUI::BasicWnd();
	guild->vtbl = (Zeal::EqUI::BaseVTable*)0x5e60f0;
	reinterpret_cast<int* (__thiscall*)(Zeal::EqUI::BasicWnd*, Zeal::EqUI::BasicWnd*, Zeal::EqUI::CXSTR name, int, int)>(0x56e1e0)(guild, 0, Zeal::EqUI::CXSTR("GuildManagementWnd"), -1, 0);
	guild->CreateChildren();
	members = (Zeal::EqUI::ListWnd*)guild->GetChildItem("MemberList");
}

ui_guild::ui_guild(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	guild = nullptr;
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
		zeal->commands_hook->Add("/read", {}, "",
			[this](std::vector<std::string>& args) {
				if (members)
				{

					for (int i = 1; i < members->ItemCount; i++)
					{
						Zeal::EqUI::CXSTR name;
						Zeal::EqUI::CXSTR level;
						Zeal::EqUI::CXSTR _class;
						members->GetItemText(&name, i, 0);
						members->GetItemText(&level, i, 1);
						members->GetItemText(&_class, i, 2);
						Zeal::EqGame::print_chat("%s %s %s", name.Data->Text, level.Data->Text, _class.Data->Text);
					}
				}
				return true;
			});
		zeal->commands_hook->Add("/sortskill", {}, "",
			[this](std::vector<std::string>& args) {
				Zeal::EqUI::ListWnd* wnd = (Zeal::EqUI::ListWnd*)Zeal::EqGame::Windows->Skills->GetChildItem("SkillList");
				if (wnd)
				{
					wnd->Sort(0);
				}
				else
				{
					Zeal::EqGame::print_chat("skillist not found");
				}
			return true;
			});
		zeal->commands_hook->Add("/guildwindow", {}, "Toggle guild management window",
			[this, mgr](std::vector<std::string>& args) {
					if (guild)
					{
						Zeal::EqGame::print_chat("Attempting to show guild window");
						guild->IsVisible=true;
						//some mock data -->
						std::vector<std::vector<std::string>> players;
						Zeal::EqStructures::Entity* ent = Zeal::EqGame::get_entity_list();
						while (ent->Next)
						{
							if (ent->Type == 0)
							{
								players.push_back(
									{
										ent->Name, std::to_string(ent->Level),Zeal::EqGame::class_name_short(ent->Class),"Peasant", "5/7/2024", "Commons", ""
									}
								);
							}
							ent = ent->Next;

						}
						mgr->AddListItems(members, players);
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
