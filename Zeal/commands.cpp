#include "commands.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"


std::vector<std::string> splitStringBySpace(const std::string& str) {
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;

	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}

	return tokens;
}

void __fastcall InterpretCommand(int c, int unused, int player, char* cmd)
{
	ZealService* zeal = ZealService::get_instance();
	std::string str_cmd = cmd;
	std::vector<std::string> args = splitStringBySpace(str_cmd);

	if (args.size() > 0)
	{
		bool cmd_handled = false;
		if (zeal->commands_hook->CommandFunctions.count(args[0]) > 0)
		{
			cmd_handled = zeal->commands_hook->CommandFunctions[args[0]](args);
		}
		else if (zeal->commands_hook->CommandAliases.count(args[0]) > 0)
		{
			cmd_handled = (*zeal->commands_hook->CommandAliases[args[0]])(args);
		}
		if (cmd_handled)
			return;
	}
	zeal->hooks->hook_map["commands"]->original(InterpretCommand)(c, unused, player, cmd);
}

void ChatCommands::add(std::string cmd, std::vector<std::string>aliases, std::function<bool(std::vector<std::string>&args)> callback)
{
	CommandFunctions[cmd] = callback;
	for (auto& a : aliases)
	{
		CommandAliases[a] = &CommandFunctions[cmd];
	}
}

ChatCommands::~ChatCommands()
{

}
ChatCommands::ChatCommands(ZealService* zeal)
{

	//just going to use lambdas for simple commands
	add("/autoinventory", { "/autoinv", "/ai" },
		[](std::vector<std::string>& args) {
			int a1 = *Zeal::EqGame::ptr_LocalPC;
			int a2 = a1 + 0xD78;
			Zeal::EqGame::EqGameInternal::auto_inventory(a1, a2, 0);
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});

	add("/hidecorpse", { "/hc", "/hideco" },
		[](std::vector<std::string>& args) {
			if (args.size() > 1 && args[1] == "looted")
			{
				ZealService::get_instance()->looting_hook->set_hide_looted(!ZealService::get_instance()->looting_hook->hide_looted);
				if (ZealService::get_instance()->looting_hook->hide_looted)
					Zeal::EqGame::print_chat("Corpses will be hidden after looting.");
				else
					Zeal::EqGame::print_chat("Corpses will no longer hidden after looting.");
				return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			if (args.size() > 1 && args[1] == "none")
			{
				ZealService::get_instance()->looting_hook->set_hide_looted(false);
				return false; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
		});
	add("/smoothing", {},
		[](std::vector<std::string>& args) {

			ZealService::get_instance()->camera_mods->set_smoothing(!ZealService::get_instance()->camera_mods->smoothing);
			if (ZealService::get_instance()->camera_mods->smoothing)
				Zeal::EqGame::print_chat("Zeal mouse look smoothing enabled.");
			else
				Zeal::EqGame::print_chat("Zeal mouse look smoothing disabled.");
			return true;
		});
	add("/camp", {},
		[](std::vector<std::string>& args) {

			Zeal::EqGame::change_stance(Stance::Sit);
			return false;
		});
	zeal->hooks->Add("commands", Zeal::EqGame::EqGameInternal::fn_interpretcmd, InterpretCommand, hook_type_detour, 9);
}

