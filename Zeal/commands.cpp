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

void Commands::add(std::string cmd, std::vector<std::string>aliases, std::function<bool(std::vector<std::string>&args)> callback)
{
	CommandFunctions[cmd] = callback;
	for (auto& a : aliases)
	{
		CommandAliases[a] = &CommandFunctions[cmd];
	}
}

Commands::~Commands()
{

}
Commands::Commands(ZealService* zeal)
{
	add("/autoinventory", { "/autoinv", "/ai" },
		[](std::vector<std::string>& args) {
			int a1 = *Zeal::EqGame::ptr_LocalPC;
			int a2 = a1 + 0xD78;
			Zeal::EqGame::EqGameInternal::auto_inventory(a1, a2, 0);
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	zeal->hooks->Add("commands", Zeal::EqGame::EqGameInternal::fn_interpretcmd, InterpretCommand, hook_type_detour, 9);
}
