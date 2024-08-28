#include "commands.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "EqPackets.h"
#include "Zeal.h"
#include <algorithm>
#include <cctype>
#include "string_util.h"


void ChatCommands::print_commands()
{
	std::stringstream ss;
	ss << "List of commands" << std::endl;
	ss << "-----------------------------------------------------" << std::endl;
	for (auto& [name, c] : CommandFunctions)
	{
		ss << name;
		if (c.aliases.size() > 0)
			ss << " [";
		for (auto it = c.aliases.begin(); it != c.aliases.end(); ++it) {
			auto& a = *it;
			ss << a;
			if (std::next(it) != c.aliases.end()) {
				ss << ", ";
			}
		}
		if (c.aliases.size() > 0)
			ss << "]";

		ss << " " << c.description;
		ss << std::endl;
	}
	Zeal::EqGame::print_chat(ss.str());
}

void __fastcall InterpretCommand(int c, int unused, int player, char* cmd)
{
	ZealService* zeal = ZealService::get_instance();
	std::string str_cmd = Zeal::String::trim_and_reduce_spaces(cmd);
	if (str_cmd.length() == 0)
		return;
	if (str_cmd.length() > 0 && str_cmd.front() != '/')
	{
		if (zeal->tells && zeal->tells->HandleTell(str_cmd))
		{
			InterpretCommand(c, unused, player, (char*)str_cmd.c_str());
			return;
		}
		str_cmd = "/" + str_cmd;
	}
	std::vector<std::string> args = Zeal::String::split(str_cmd," ");
	const std::string& cmd_name = args.front();
	if (!args.empty() && !cmd_name.empty() && Zeal::EqGame::is_in_game())
	{
		bool cmd_handled = false;
		auto& command_functions = zeal->commands_hook->CommandFunctions;
		if (command_functions.count(cmd_name))
		{
			cmd_handled = command_functions[args[0]].callback(args);
		} 
		else
		{
			//attempt to find and call the function via an alias
			for (const auto& command_pair : command_functions) {
				const auto& command = command_pair.second;
				if (command.callback && !command.aliases.empty()) {
					for (const auto alias : command.aliases) {
						if (!alias.empty() && alias == cmd_name) {
							cmd_handled = command.callback(args);
							break;
						}
					}
				}
			}
		}
		if (cmd_handled) {
			return;
		}
	}
	zeal->hooks->hook_map["commands"]->original(InterpretCommand)(c, unused, player, cmd);
}

void ChatCommands::Add(std::string cmd, std::vector<std::string>aliases, std::string description, std::function<bool(std::vector<std::string>&args)> callback)
{
	CommandFunctions[cmd] = ZealCommand(aliases, description, callback);
}

ChatCommands::~ChatCommands()
{

}
ChatCommands::ChatCommands(ZealService* zeal)
{
	Add("o", {}, "Removes the o command that is switching ui from new to old.",
		[](std::vector<std::string>& args) {
			if (Zeal::String::compare_insensitive(args[0], "o"))
				return true;
			return false;
		});
	Add("/target", { "/cleartarget" }, "Adds clear target functionality to the /target command if you give it no arguments.",
		[](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				Zeal::EqGame::set_target(0);
				return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			return false;
		});
	Add("/corpsedrag", { "/drag"}, "Attempts to corpse drag your current target.",
		[](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				if (Zeal::EqGame::get_target())
				{
					Zeal::Packets::CorpseDrag_Struct tmp;
					memset(&tmp, 0, sizeof(tmp));
					strcpy_s(tmp.CorpseName, 30, Zeal::EqGame::get_target()->Name);
					strcpy_s(tmp.DraggerName, 30, Zeal::EqGame::get_self()->Name);
					Zeal::EqGame::send_message(Zeal::Packets::opcodes::CorpseDrag, (int*)&tmp, sizeof(tmp), 0);
				}
				return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			return false;
		});
	Add("/corpsedrop", { "/drop"}, "Attempts to drop a corpse (your current target). To drop all use /corpsedrop all",
		[](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				if (Zeal::EqGame::get_target())
				{
					Zeal::Packets::CorpseDrag_Struct tmp;
					memset(&tmp, 0, sizeof(tmp));
					strcpy_s(tmp.CorpseName, 30, Zeal::EqGame::get_target()->Name);
					strcpy_s(tmp.DraggerName, 30, Zeal::EqGame::get_self()->Name);
					Zeal::EqGame::send_message(Zeal::Packets::opcodes::CorpseDrop, (int*)&tmp, sizeof(tmp), 0);
				}
				return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			else if (Zeal::String::compare_insensitive(args[1], "all"))
			{
				Zeal::EqGame::send_message(Zeal::Packets::opcodes::CorpseDrop, 0, 0, 0);
				return true;
			}
			return false;
		});
	Add("/trade", { "/opentrade", "/ot" }, "Opens a trade window with your current target.",
		[](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				if ((Zeal::EqGame::Windows->Trade->IsVisible || Zeal::EqGame::Windows->Give->IsVisible) && Zeal::EqGame::get_char_info()->CursorItem)
				{
					int trade_size = 4;
					int ptr1 = *(int*)0x7f94c8;
					if (*(BYTE*)(ptr1 + 0xa8) == 0)
						trade_size = 8;
					for (int i = 0; i < trade_size; i++) //look for an empty slot
					{
						if (!Zeal::EqGame::Windows->Trade->Item[i])
						{
							Zeal::EqGame::move_item(0, i + 0xbb8, 0, 1); //move item
							break;
						}
					}
				}
				else
				{
					if (Zeal::EqGame::get_target())
					{
						Zeal::Packets::TradeRequest_Struct tmp;
						memset(&tmp, 0, sizeof(tmp));
						tmp.from_id = Zeal::EqGame::get_self()->SpawnId;
						tmp.to_id = Zeal::EqGame::get_target()->SpawnId;
						Zeal::EqGame::send_message(Zeal::Packets::opcodes::RequestTrade, (int*)&tmp, sizeof(tmp), 0);
					}
				}
				return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			return false;
		});
	Add("/useitem", {}, "Use an item's right click function arugment is 0-29 which indicates the slot",
		[](std::vector<std::string>& args) {
			Zeal::EqStructures::EQCHARINFO* chr = Zeal::EqGame::get_char_info();
			if (!chr)
			{
				Zeal::EqGame::print_chat(USERCOLOR_SHOUT,"[Fatal Error] Failed to get charinfo for useitem!");
				return true;
			}
			int item_index = 0;
			if (args.size() > 1 && Zeal::String::tryParse(args[1], &item_index))
			{
				Zeal::EqStructures::EQITEMINFO* item = nullptr;
				if (item_index > 29) //just early out of it this is beyond your inventory indices
				{
					Zeal::EqGame::print_chat("useitem requires an item slot between 0 and 29, you tried to use %i", item_index);
					return true;
				}
				if (item_index < 21)
					item = chr->InventoryItem[item_index];
				else
					item = chr->InventoryPackItem[item_index-22]; //-22 to make it back to 0 index

				if (!item)
				{
					Zeal::EqGame::print_chat("There isn't an item at %i", item_index);
					return true;
				}
				if (item->Common.SpellId)
				{
					chr->cast(0xA, 0, (int*)&item, item_index < 21 ? item_index+1 : item_index);
				}
				else
				{
					if (item)
						Zeal::EqGame::print_chat("item %s does not have a spell attached to it.", item->Name);
				}
			}
			else
			{
				Zeal::EqGame::print_chat("useitem requires an item slot between 0 and 29");
			}
			return true;
		}
	);
	Add("/autoinventory", { "/autoinv", "/ai" }, "Puts whatever is on your cursor into your inventory.",
		[](std::vector<std::string>& args) {
			if (Zeal::EqGame::can_inventory_item(Zeal::EqGame::get_char_info()->CursorItem))
			{
				Zeal::EqGame::EqGameInternal::auto_inventory(Zeal::EqGame::get_char_info(), &Zeal::EqGame::get_char_info()->CursorItem, 0);
			}
			else
			{
				if (Zeal::EqGame::get_char_info()->CursorItem)
					Zeal::EqGame::print_chat(USERCOLOR_LOOT, "Cannot auto inventory %s", Zeal::EqGame::get_char_info()->CursorItem->Name);
			}
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	Add("/testinventory", { }, "Test cursor item to see if it can be inventoried",
		[](std::vector<std::string>& args) {


			Zeal::EqGame::print_chat("You %s inventory that item", Zeal::EqGame::can_inventory_item(Zeal::EqGame::get_char_info()->CursorItem) ? "can" : "cannot");
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	Add("/autobank", { "/autoba", "/ab" }, "Changes your money into its highest denomination in bank and inventory (requires bank to be open).",
		[](std::vector<std::string>& args) {
			ZealService::get_instance()->ui->bank->change();
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	Add("/aspectratio", { "/ar"}, "Change your aspect ratio.",
		[](std::vector<std::string>& args) {
			Zeal::EqStructures::CameraInfo* ci = Zeal::EqGame::get_camera();
			if (ci)
			{
				float ar = 0;
				if (args.size() > 1 && Zeal::String::tryParse(args[1], &ar))
				{
					ci->AspectRatio = ar;
				}
				else
				{
					Zeal::EqGame::print_chat("Current Aspect Ratio [%f]", ci->AspectRatio);
				}

			}

			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	Add("/clearchat", {}, "Adds clearchat functionality to oldui.",
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
	Add("/sit", {}, "Adds arguments on/down to force you to sit down instead of just toggling.",
		[](std::vector<std::string>& args) {
			if (args.size() > 1 && args.size() < 3) {
				if (Zeal::String::compare_insensitive(args[1], "on") || Zeal::String::compare_insensitive(args[1], "down")) {
					Zeal::EqGame::get_self()->ChangeStance(Stance::Sit);
					return true;
				}
			}
			return false;
		});
	Add("/camp", {}, "Adds auto sit when attempting to camp.",
		[](std::vector<std::string>& args) {
			if (Zeal::EqGame::is_in_game())
				Zeal::EqGame::get_self()->ChangeStance(Stance::Sit);
			return false;
		});
	Add("/showhelm", { "/helm" }, "Sends server command #showhelm with arugments on/off.",
		[](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				Zeal::EqGame::print_chat("usage: /showhelm [on | off]");
				return true;
			}
			if (args.size() > 1 && args.size() < 3) {
				if (Zeal::String::compare_insensitive(args[1], "on")) {
					Zeal::EqGame::do_say(true, "#showhelm on");
					return true;
				}
				else if (Zeal::String::compare_insensitive(args[1], "off"))
				{
					Zeal::EqGame::do_say(true, "#showhelm off");
					return true;
				}
			}
			return false;
		});
	Add("/showlootlockouts", { "/sll", "/showlootlockout", "/showlockouts" }, "Sends #showlootlockouts to server.",
		[](std::vector<std::string>& args) {

			Zeal::EqGame::do_say(true, "#showlootlockouts");
			return true;
		});
	Add("/zeal", { "/zea" }, "Help and version information.",
		[this](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				Zeal::EqGame::print_chat("Available args: version, help"); //leave room for more args on this command for later
				return true;
			}
			if (args.size() > 1 && Zeal::String::compare_insensitive(args[1], "version"))
			{
				std::stringstream ss;
				ss << "Zeal version: " << ZEAL_VERSION << " (" << ZEAL_BUILD_VERSION << ")" << std::endl;
				Zeal::EqGame::print_chat(ss.str());
				return true;
			}
			if (args.size() > 1 && Zeal::String::compare_insensitive(args[1], "help"))
			{
				print_commands();
				return true;
			}
			return false;
		});
	
	Add("/clientmanatick", { "/cmt" }, "Toggle client mana tick (disabled by default in this client).",
		[this](std::vector<std::string>& args) {
			BYTE orig1[7] = { 0x66, 0x01, 0xBE, 0x9A, 0x0, 0x0, 0x0 }; //0x4C3F93
			BYTE orig2[7] = { 0x66, 0x01, 0x87, 0x9A, 0x0, 0x0, 0x0 }; //0x4c7642
			if (*(BYTE*)0x4C3F93 == 0x90)
			{
				mem::copy(0x4C3F93, (int)orig1, sizeof(orig1));
				mem::copy(0x4C7642, (int)orig2, sizeof(orig2));
				Zeal::EqGame::print_chat("Client sided mana tick re-enabled");
			}
			else
			{
				mem::set(0x4C3F93, 0x90, sizeof(orig1));
				mem::set(0x4C7642, 0x90, sizeof(orig2));
				Zeal::EqGame::print_chat("Client sided mana tick disabled");
			}
			return true;
		});

	Add("/reloadskin", {}, "Reload your current ui with ini.",
		[this](std::vector<std::string>& args) {
			mem::write<BYTE>(0x8092d9, 1); //reload skin
			mem::write<BYTE>(0x8092da, 1);  //reload with ui
			return true;
		});
	Add("/alarm", {}, "Open the alarm ui and gives alarm functionality on old ui.",
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
					if (Zeal::String::compare_insensitive(args[1], "set")) {
						int minutes = 0;
						int seconds = 0;
						size_t delim[2] = { args[2].find("m"), args[2].find("s") };
						if (Zeal::String::tryParse(args[2].substr(0, delim[0]), &minutes) &&
								Zeal::String::tryParse(args[2].substr(delim[0]+1, delim[1]), &seconds))
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
					else if (Zeal::String::compare_insensitive(args[1], "halt")) {
						zeal->alarm->Halt();
						return true;
					}
				}
			}
			return false;
		});
		Add("/inspect", {}, "Inspect your current target.",
		[this, zeal](std::vector<std::string>& args) {
				if (args.size() > 1 && args[1] == "target")
				{
					Zeal::EqStructures::Entity* t = Zeal::EqGame::get_target();
					if (!t || t->Type!=0)
					{
						Zeal::EqGame::print_chat("That is not a valid target to inspect!");
						return true;
					}
					else
					{
						Zeal::EqGame::do_inspect(t);
						return true;
					}
				}
			return false;
		});

	Add("/help", { "/hel" }, "Adds zeal to the help list.",
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
			if (args.size() > 1 && Zeal::String::compare_insensitive(args[1],"zeal"))
			{
				print_commands();
				return true;
			}
			return false;
		});
	zeal->hooks->Add("commands", Zeal::EqGame::EqGameInternal::fn_interpretcmd, InterpretCommand, hook_type_detour);
}

