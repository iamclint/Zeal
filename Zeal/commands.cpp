#include "commands.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
#include <cctype>

static bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2) {
	// Check if the strings are of different lengths, if so, they can't match
	if (str1.length() != str2.length()) {
		return false;
	}

	// Convert both strings to lowercase and then compare
	std::string str1Lower = str1;
	std::string str2Lower = str2;
	std::transform(str1Lower.begin(), str1Lower.end(), str1Lower.begin(), ::tolower);
	std::transform(str2Lower.begin(), str2Lower.end(), str2Lower.begin(), ::tolower);

	// Now, perform a case-insensitive comparison
	return str1Lower == str2Lower;
}


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
		if (cmd_handled)
			return;
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

	add("/hidecorpse", { "/hc", "/hideco", "/hidec"},
		[](std::vector<std::string>& args) {
			if (args.size() > 1 && caseInsensitiveStringCompare(args[1], "looted"))
			{
				ZealService::get_instance()->looting_hook->set_hide_looted(!ZealService::get_instance()->looting_hook->hide_looted);
				if (ZealService::get_instance()->looting_hook->hide_looted)
					Zeal::EqGame::print_chat("Corpses will be hidden after looting.");
				else
					Zeal::EqGame::print_chat("Corpses will no longer be hidden after looting.");
				return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			if (args.size() > 1 && args[1] == "none")
			{
				ZealService::get_instance()->looting_hook->set_hide_looted(false);
				return false; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
			}
			return false;
		});
	add("/zealcam", {"/smoothing"},
		[](std::vector<std::string>& args) {

			if (args.size() == 3) //the first arg is the command name itself
			{
				float x_sens = 0;
				float y_sens = 0;
				try {
					x_sens = std::stof(args[1]);
					ZealService::get_instance()->camera_mods->user_sensitivity_x = x_sens;
					ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd = x_sens;
				}
				catch (const std::invalid_argument& e) {
					Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
					return true;
				}
				catch (const std::out_of_range& e) {
					Zeal::EqGame::print_chat("Out of range: %s", e.what());
					return true;
				}
				try {
					y_sens = std::stof(args[2]);
					ZealService::get_instance()->camera_mods->user_sensitivity_y = y_sens;
					ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd = y_sens;
				}
				catch (const std::invalid_argument& e) {
					Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
					return true;
				}
				catch (const std::out_of_range& e) {
					Zeal::EqGame::print_chat("Out of range: %s", e.what());
					return true;
				}
				ZealService::get_instance()->camera_mods->set_smoothing(true);

				
				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX", ZealService::get_instance()->camera_mods->user_sensitivity_x);
				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY", ZealService::get_instance()->camera_mods->user_sensitivity_y);
				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX3rd", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY3rd", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
				Zeal::EqGame::print_chat("New camera sensitivity [%f] [%f]", ZealService::get_instance()->camera_mods->user_sensitivity_x, ZealService::get_instance()->camera_mods->user_sensitivity_y);
			}
			else if (args.size() == 5)
			{
				float x_sens = 0;
				float y_sens = 0;
				float x_sens_3rd = 0;
				float y_sens_3rd = 0;
				try {
					x_sens = std::stof(args[1]);
					ZealService::get_instance()->camera_mods->user_sensitivity_x = x_sens;
				}
				catch (const std::invalid_argument& e) {
					Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
					return true;
				}
				catch (const std::out_of_range& e) {
					Zeal::EqGame::print_chat("Out of range: %s", e.what());
					return true;
				}
				try {
					y_sens = std::stof(args[2]);
					ZealService::get_instance()->camera_mods->user_sensitivity_y = y_sens;
				}
				catch (const std::invalid_argument& e) {
					Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
					return true;
				}
				catch (const std::out_of_range& e) {
					Zeal::EqGame::print_chat("Out of range: %s", e.what());
					return true;
				}
				try {
					x_sens_3rd = std::stof(args[3]);
					ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd = x_sens_3rd;
				}
				catch (const std::invalid_argument& e) {
					Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
					return true;
				}
				catch (const std::out_of_range& e) {
					Zeal::EqGame::print_chat("Out of range: %s", e.what());
					return true;
				}
				try {
					y_sens_3rd = std::stof(args[4]);
					ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd = y_sens_3rd;
				}
				catch (const std::invalid_argument& e) {
					Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
					return true;
				}
				catch (const std::out_of_range& e) {
					Zeal::EqGame::print_chat("Out of range: %s", e.what());
					return true;
				}
				ZealService::get_instance()->camera_mods->set_smoothing(true);


				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX", ZealService::get_instance()->camera_mods->user_sensitivity_x);
				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY", ZealService::get_instance()->camera_mods->user_sensitivity_y);
				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX3rd", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
				ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY3rd", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);

				Zeal::EqGame::print_chat("New camera sensitivity FirstPerson: [%f] [%f] ThirdPerson: [%f] [%f]", ZealService::get_instance()->camera_mods->user_sensitivity_x, ZealService::get_instance()->camera_mods->user_sensitivity_y, ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd, ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
			} 
			else
			{
				ZealService::get_instance()->camera_mods->set_smoothing(!ZealService::get_instance()->camera_mods->smoothing);
				if (ZealService::get_instance()->camera_mods->smoothing)
					Zeal::EqGame::print_chat("Zeal mouse look smoothing enabled.");
				else
					Zeal::EqGame::print_chat("Zeal mouse look smoothing disabled.");
			}
			return true;
		});
	add("/camp", {},
		[](std::vector<std::string>& args) {

			Zeal::EqGame::change_stance(Stance::Sit);
			return false;
		});
	add("/showlootlockouts", {"/sll", "/showlootlockout", "/showlockouts"},
		[](std::vector<std::string>& args) {

			Zeal::EqGame::do_say(true, "#showlootlockouts");
			return true;
		});
	add("/help", {"/hel"},
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
			if (args.size() > 1 && caseInsensitiveStringCompare(args[1],"zeal"))
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

