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

// This replaces the previous /camp command with a direct hook of the EverQuest::Camp() client
// method in order to support all camping pathways (buttons, hotkeyed button, and /camp).
static void __fastcall EverquestCamp(void* this_everquest, int unused_edx)
{
	// Support auto-sitting (but peek ahead to see if the camp command is likely allowed).
	if (Zeal::EqGame::is_in_game() && !Zeal::EqGame::EqGameInternal::IsNoSlashWndActive())
		Zeal::EqGame::sit();
	if (ZealService::get_instance()->outputfile->setting_export_on_camp.get()) {
		ZealService::get_instance()->outputfile->export_inventory();
		ZealService::get_instance()->outputfile->export_spellbook();
	}
	ZealService::get_instance()->hooks->hook_map["EverquestCamp"]->original(EverquestCamp)(this_everquest, unused_edx);
}

void __fastcall InterpretCommand(int c, int unused, Zeal::EqStructures::Entity* player, const char* cmd)
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
		//Zeal::EqGame::print_chat("Command: %s", str_cmd.c_str()); //if the problem comes up again use this with the test to see what happening
		//str_cmd = "/" + str_cmd;
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

//call interpret command without hitting the detour, useful for aliasing default commands
void ForwardCommand(std::string cmd)
{
	reinterpret_cast<bool(__thiscall*)(int cEverquest, Zeal::EqStructures::Entity* player, const char* cmd)>(ZealService::get_instance()->hooks->hook_map["commands"]->trampoline)(*(int*)0x809478, Zeal::EqGame::get_self(), cmd.c_str());
}

ChatCommands::ChatCommands(ZealService* zeal)
{
	Add("/crash", {}, "Tests a crash",
		[](std::vector<std::string>& args) {
			int* x = 0;
			*x = 0; //nullptr exception
			return false;
		});
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
	Add("/cls", { }, "Adds cls alias for clearchat.",
		[](std::vector<std::string>& args) {
			ForwardCommand("/clearchat");
			return true;
		});
	Add("/corpsedrag", { "/drag"}, "Attempts to corpse drag your current target. Use /corpsedrag nearest to auto-target.",
		[](std::vector<std::string>& args) {
			bool nearest = (args.size() == 2 && args[1] == "nearest");
			if (args.size() == 1 || nearest)
			{
				if (nearest)
				{
					auto* ent = ZealService::get_instance()->cycle_target->get_nearest_ent(250, Zeal::EqEnums::PlayerCorpse);
					if (ent)
						Zeal::EqGame::set_target(ent);
				}

				if (Zeal::EqGame::get_target() && (Zeal::EqGame::get_target()->Type == Zeal::EqEnums::PlayerCorpse))
				{
					Zeal::Packets::CorpseDrag_Struct tmp;
					memset(&tmp, 0, sizeof(tmp));
					strcpy_s(tmp.CorpseName, 30, Zeal::EqGame::get_target()->Name);
					strcpy_s(tmp.DraggerName, 30, Zeal::EqGame::get_self()->Name);
					Zeal::EqGame::send_message(Zeal::Packets::opcodes::CorpseDrag, (int*)&tmp, sizeof(tmp), 0);
				}
				else if (nearest)
					Zeal::EqGame::print_chat("No corpse found nearby to drag.");
				else
					Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Need to target a corpse to /drag (or use /drag nearest)");

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
				else
					Zeal::EqGame::print_chat("Need to target a corpse to /drop (or use /drop all)");
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
				if (Zeal::EqGame::Windows->Trade->IsVisible || Zeal::EqGame::Windows->Give->IsVisible)
				{
					// Disabled the auto-drop from the cursor since it needs more work (see /singlegive notes).
					Zeal::EqGame::print_chat("Trade window already open");
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
			Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			if (!char_info || !self || !self->ActorInfo)
			{
				Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "[Fatal Error] Failed to get entity for useitem!");
				return true;
			}
			int item_index = 0;
			if (args.size() > 1 && Zeal::String::tryParse(args[1], &item_index))
			{
				if (char_info->Class == Zeal::EqEnums::ClassTypes::Bard && ZealService::get_instance()->melody->use_item(item_index))
					return true;
				bool quiet = args.size() > 2 && args[2] == "quiet";
				Zeal::EqGame::use_item(item_index, quiet);
			}
			else
			{
				Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "useitem requires an item slot between 0 and 29");
				Zeal::EqGame::print_chat("0: Left ear, 1: Head, 2: Face, 3: Right Ear, 4: Neck, 5: Shoulders");
				Zeal::EqGame::print_chat("6: Arms, 7: Back, 8: Left Wrist, 9: Right Wrist, 10: Ranged");
				Zeal::EqGame::print_chat("11: Hands, 12: Primary, 13: Secondary, 14: Left Finger, 15: Right Finger");
				Zeal::EqGame::print_chat("16: Chest, 17: Legs, 18: Feet, 19: Waist, 20: Ammo");
				Zeal::EqGame::print_chat("Inventory: 22: Top Left, 25: Bottom left, 26: Top Right, 29: Bottom Right");
			}
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
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
					Zeal::EqGame::sit();
					return true;
				}
			}
			return false;
		});
	Add("/run", {}, "Sets run mode (toggle, on, off (walk)).",
		[](const std::vector<std::string>& args) {
			BYTE* run_mode = reinterpret_cast<BYTE*>(0x0079856d);
			if (args.size() == 2 && Zeal::String::compare_insensitive(args[1], "on"))
				*run_mode = 1;
			else if (args.size() == 2 && Zeal::String::compare_insensitive(args[1], "off"))
				*run_mode = 0;
			else if (args.size() == 1) {
				*run_mode = (*run_mode == 0);
				Zeal::EqGame::print_chat("%s", *run_mode ? "Run on" : "Walk on");
			}
			else
				Zeal::EqGame::print_chat("Usage: /run (toggles), /run on, /run off");
			return true;
		});
	Add("/showhelm", { "/helm" }, "Toggles your show helm setting on/off.",
		[](std::vector<std::string>& args) {
			if (args.size() == 1) {
				ZealService::get_instance()->helm->ShowHelmEnabled.toggle();
				return true;
			}
			if (args.size() == 2) {
				if (Zeal::String::compare_insensitive(args[1], "on")) {
					ZealService::get_instance()->helm->ShowHelmEnabled.set(true);
					return true;
				}
				if (Zeal::String::compare_insensitive(args[1], "off")) {
					ZealService::get_instance()->helm->ShowHelmEnabled.set(false);
					return true;
				}
			}
			Zeal::EqGame::print_chat("Usage: /showhelm or /showhelm <on|off>");
			return true;
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
			if (args.size() == 2 && args[1] == "check")  // Report state and do basic debug integrity checks.
			{
				ZealService::get_instance()->entity_manager.get()->Dump();
				int heap_valid1 = HeapValidate(GetProcessHeap(), 0, NULL);
				Zeal::EqGame::print_chat("Process HeapValidate: %s", heap_valid1 ? "Pass" : "Fail");
				int heap_valid2 = HeapValidate(*Zeal::EqGame::Heap, 0, NULL);
				Zeal::EqGame::print_chat("Game HeapValidate: %s", heap_valid2 ? "Pass" : "Fail");

				HEAP_SUMMARY heap_summary;
				memset(&heap_summary, 0, sizeof(heap_summary));
				heap_summary.cb = sizeof(heap_summary);
				HeapSummary(*Zeal::EqGame::Heap, 0, &heap_summary);
				Zeal::EqGame::print_chat("Game Heap: Alloc: 0x%08x, Commit: 0x%08x",
					heap_summary.cbAllocated, heap_summary.cbCommitted);

				return true;
			}
			if (args.size() == 3 && args[1] == "get_command")
			{
				auto command = Zeal::EqGame::get_command_struct(args[2]);
				if (command)
					Zeal::EqGame::print_chat("%s: id: %i, name: %s, localized: %s, gm: %i, category: %i, fn: 0x%08x",
						args[2].c_str(), command->string_id, command->name ? command->name : "null",
						command->localized_name ? command->localized_name : "null",
						command->gm_command, command->category, command->fn);
				else
					Zeal::EqGame::print_chat("no matches");

				return true;
			}
			if (args.size() == 2 && args[1] == "list_keybinds")  // Just a utility to check native keybind mapping.
			{
				const char** cmd = reinterpret_cast<const char**>(0x00611220);
				for (int i = 0; cmd[i] != nullptr; ++i)
					Zeal::EqGame::print_chat("[%d]: %s", i, cmd[i]);
				return true;
			}
			if (args.size() == 2 && args[1] == "target_name")  // Report name parsing of current target.
			{
				Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
				if (target) {
					std::string original = target->Name;
					std::string trimmed = Zeal::EqGame::trim_name(target->Name);
					std::string stripped = Zeal::EqGame::strip_name(target->Name);
					Zeal::EqGame::print_chat("Raw: %s, Trim: %s, Strip: %s, Equal: %s",
						original.c_str(), trimmed.c_str(), stripped.c_str(), trimmed == stripped ? "true" : "false");
					if (target->ActorInfo && target->ActorInfo->DagHeadPoint && target->ActorInfo->DagHeadPoint->StringSprite) {
						auto& sprite = *target->ActorInfo->DagHeadPoint->StringSprite;
						if (sprite.MagicValue == sprite.kMagicValidValue)
							Zeal::EqGame::print_chat("Sprite: %s, len: %i", sprite.Text, sprite.TextLength);
					}
				}
				return true;
			}
			int sound_index = 0;
			if (args.size() == 3 && args[1] == "wave_play" && Zeal::String::tryParse(args[2], &sound_index))
			{
				Zeal::EqGame::WavePlay(sound_index);
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
		Add("/lead", {}, "Print current group and raid leaders", [this, zeal](std::vector<std::string>& args) {
			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;

			if (raid_info->is_in_raid())
			{
				bool show_all = (args.size() == 2 && args[1] == "all");
				bool show_open = (args.size() == 2 && args[1] == "open");
				Zeal::EqGame::print_raid_leaders(show_all, show_open);
			}
			else
				Zeal::EqGame::print_group_leader();

			if (args.size() == 2 && args[1] == "dump")
				Zeal::EqGame::dump_raid_state();

			return true;
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
	zeal->hooks->Add("EverquestCamp", 0x00530c7b, EverquestCamp, hook_type_detour);
}

