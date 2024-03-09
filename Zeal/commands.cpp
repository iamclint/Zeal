#include "commands.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
#include <cctype>
#include "StringUtil.h"

void __fastcall InterpretCommand(int c, int unused, int player, char* cmd)
{
	ZealService* zeal = ZealService::get_instance();
	std::string str_cmd = cmd;
	std::vector<std::string> args =  StringUtil::split(str_cmd," ");

	if (args.size() > 0)
	{
		bool cmd_handled = false;
		if (zeal->commands_hook->CommandFunctions.count(args[0]) > 0)
		{
			cmd_handled = zeal->commands_hook->CommandFunctions[args[0]].callback(args);
		}
		else
		{
			for (auto& m : zeal->commands_hook->CommandFunctions)
			{
				for (auto& a : m.second.aliases)
				{
					if (a == args[0])
					{
						cmd_handled = m.second.callback(args);
						break;
					}
				}
			}
		}
		if (cmd_handled) {
			if (!Zeal::EqGame::is_new_ui())
				cmd[0] = '\0';
			return;
		}
	}
	zeal->hooks->hook_map["commands"]->original(InterpretCommand)(c, unused, player, cmd);
}

void ChatCommands::add(std::string cmd, std::vector<std::string>aliases, std::function<bool(std::vector<std::string>&args)> callback)
{
	CommandFunctions[cmd] = ZealCommand(aliases, callback);
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
	add("/clearchat", {},
		[](std::vector<std::string>& args) {
			// each line can hold up to 256 characters but typically doesnt reach that far.
			// each unused line is set with '@\0', so lets clear to that
			if (!Zeal::EqGame::is_new_ui()) {
				int start = 0x799D8D;
				int end = 0x7B908D;
				while (start != end) {
					mem::write<WORD>(start, 0x4000);
					start += 256;
				}
				// max scrollbar height doesn't reset properly here. Need to figure out why still.
				return true;
			}

			return false;
		});
	add("/camp", {},
		[](std::vector<std::string>& args) {

			Zeal::EqGame::change_stance(Stance::Sit);
			return false;
		});
	add("/showhelm", { "/helm" },
		[](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				Zeal::EqGame::print_chat("usage: /showhelm [on | off]");
				return true;
			}
			if (args.size() > 1 && args.size() < 3) {
				if (StringUtil::caseInsensitive(args[1], "on")) {
					Zeal::EqGame::do_say(true, "#showhelm on");
					return true;
				}
				else if (StringUtil::caseInsensitive(args[1], "off"))
				{
					Zeal::EqGame::do_say(true, "#showhelm off");
					return true;
				}
			}
			return false;
		});
	add("/showlootlockouts", { "/sll", "/showlootlockout", "/showlockouts" },
		[](std::vector<std::string>& args) {

			Zeal::EqGame::do_say(true, "#showlootlockouts");
			return true;
		});
	add("/zeal", { "/zea" },
		[this](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				Zeal::EqGame::print_chat("Available args: version"); //leave room for more args on this command for later
				return true;
			}
			if (args.size() > 1 && StringUtil::caseInsensitive(args[1], "version"))
			{
				std::stringstream ss;
				ss << "Zeal version: " << ZEAL_VERSION << std::endl;
				Zeal::EqGame::print_chat(ss.str());
				return true;
			}
			return false;
		});
	add("/alarm", {},
		[this, zeal](std::vector<std::string>& args) {
			if (Zeal::EqGame::is_new_ui()) {
				if (Zeal::EqGame::Windows->Alarm)
					Zeal::EqGame::Windows->Alarm->IsVisible = true;
				else
					Zeal::EqGame::print_chat("Alarm window not found");

				return true;
			}
			else {
				if (args.size() == 1) {
					std::ostringstream oss;
					oss << "-- ALARM COMMANDS --" << std::endl << "/alarm set #m#s" << std::endl << "/alarm halt" << std::endl;
					Zeal::EqGame::print_chat(oss.str());
					return true;
				}
				if (args.size() > 1 && args.size() < 4) {
					if (StringUtil::caseInsensitive(args[1], "set")) {
						int minutes = 0;
						int seconds = 0;
						size_t delim[2] = { args[2].find("m"), args[2].find("s") };
						if (StringUtil::tryParse(args[2].substr(0, delim[0]), &minutes) &&
								StringUtil::tryParse(args[2].substr(delim[0]+1, delim[1]), &seconds))
						{
							zeal->alarm->Set(minutes, seconds);
							return true;
						}
						else
						{
							Zeal::EqGame::print_chat("[Alarm] Failed to parse the specified duration.");
							return true;
						}
					}
					else if (StringUtil::caseInsensitive(args[1], "halt")) {
						zeal->alarm->Halt();
						return true;
					}
				}
			}
			return false;
		});
		add("/test", {},
		[this, zeal](std::vector<std::string>& args) {
				Zeal::EqGame::print_chat("Default Menu Index: %i", Zeal::EqGame::Windows->ContextMenuManager->GetDefaultMenuIndex());
			return true;
		});
	add("/help", { "/hel" },
		[this](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				std::stringstream ss;
				ss << "Format: /help <class> Where class is one of normal, emote, guild, ect.." << std::endl;
				ss << "Normal will display a list of all commands." << std::endl;
				ss << "Emote will display a list of all player emotes." << std::endl;
				ss << "Guild will display a list of guild commands." << std::endl;
				ss << "Voice will display a list of voice control commands." << std::endl;
				ss << "Chat will display a list of chat channel commands." << std::endl;
				ss << "Zeal will display a list of custom commands." << std::endl;
				Zeal::EqGame::print_chat(ss.str());
				return true;
			}
			if (args.size() > 1 && StringUtil::caseInsensitive(args[1],"zeal"))
			{
				std::stringstream ss;
				ss << "List of commands" << std::endl;
				ss << "-----------------------------------------------------" << std::endl;
				for (auto& [name, c] : CommandFunctions)
				{
					ss << name;
					if (c.aliases.size() > 0)
						ss << "  aliases  [";
					for (auto it = c.aliases.begin(); it != c.aliases.end(); ++it) {
						auto& a = *it;
						ss << a;
						if (std::next(it) != c.aliases.end()) {
							ss << ", ";
						}
					}
					if (c.aliases.size() > 0)
						ss << "]";
					ss << std::endl;
				}
				Zeal::EqGame::print_chat(ss.str());
				return true;
			}
			return false;
		});
	zeal->hooks->Add("commands", Zeal::EqGame::EqGameInternal::fn_interpretcmd, InterpretCommand, hook_type_detour);
}

