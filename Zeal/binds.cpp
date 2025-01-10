#include "binds.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
Binds::~Binds()
{
}

bool Binds::execute_cmd(UINT cmd, bool isdown)
{
	ZealService* zeal = ZealService::get_instance();
	// Don't call our binds on keydown when the game wants input except for reply cycling.
	bool reply_cycle = (cmd == 0x3c || cmd == 0x3d);
	if (!Zeal::EqGame::game_wants_input() || !isdown || reply_cycle)
	{
		//if (isdown)
		//	Zeal::EqGame::print_chat("cmd: %i down: %i", cmd, isdown);
		if (zeal->binds_hook->ReplacementFunctions.count(cmd) > 0)
		{
			for (auto& fn : zeal->binds_hook->ReplacementFunctions[cmd])
			if (fn(isdown)) //if the replacement function returns true, end here otherwise its really just adding more to the command 
				return true;
		}

		if (zeal->binds_hook->KeyMapFunctions.count(cmd) > 0)
			zeal->binds_hook->KeyMapFunctions[cmd](isdown);
		else
			return false;
	}

	return false;
}

void __fastcall InitKeyboardAssignments(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->binds_hook->ptr_binds = t;
	zeal->binds_hook->add_binds();
	zeal->binds_hook->read_ini();
	zeal->hooks->hook_map["initbinds"]->original(InitKeyboardAssignments)(t, unused);
}


UINT32 read_internal_from_ini(int index, int key_type)
{
	int fn = 0x525520;
	__asm
	{
		push key_type
		push index
		call fn
		pop ecx
		pop ecx
	}
}

void Binds::read_ini()
{
	int size = sizeof(KeyMapNames) / sizeof(KeyMapNames[0]);
	for (int i = 128; i < size; i++) //the game will load its own properly
	{
		if (KeyMapNames[i]) //check if its not nullptr
		{
			int keycode = read_internal_from_ini(i, 0);
			int keycode_alt = read_internal_from_ini(i, 1);
			if (keycode != -0x2)
			{
				Zeal::EqGame::ptr_PrimaryKeyMap[i] = keycode;
			}
			if (keycode_alt != -0x2)
			{
				Zeal::EqGame::ptr_AlternateKeyMap[i] = keycode_alt;
			}

		}
	}
}

static void cycle_targets(int key_down, Zeal::EqEnums::EntityTypes type)
{
	if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
	{
		Zeal::EqStructures::Entity* ent = ZealService::get_instance()->cycle_target->get_next_ent(250, type);
		if (ent)
			Zeal::EqGame::set_target(ent);
	}
}

void Binds::add_binds()
{
	//just start binds at 211 to avoid overwriting any existing cmd/bind
	add_bind(211, "Strafe Left",	"StrafeLeft",		key_category::Movement,	[this](int key_down) {});	// stub
	add_bind(212, "Strafe Right", "StrafeRight",	key_category::Movement, [this](int key_down) {});	// stub
	add_bind(213, "Cycle through nearest NPCs", "CycleTargetNPC", key_category::Target, [](int key_down) {
			cycle_targets(key_down, Zeal::EqEnums::NPC);
		});
	add_bind(214, "Cycle through nearest PCs", "CycleTargetPC", key_category::Target, [](int key_down) {
			cycle_targets(key_down, Zeal::EqEnums::Player);
		});
	add_bind(215, "Toggle all containers", "OpenCloseContainers", key_category::UI | key_category::Commands, [](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			std::vector<std::pair<Zeal::EqStructures::_EQITEMINFO*,int>> containers;
			std::vector<Zeal::EqStructures::_EQITEMINFO*> opened_containers; //don't need an index to close 
			for (int i = 0; i < 8; i++) //8 inventory slots for containers
			{
				Zeal::EqStructures::_EQITEMINFO* item = self->CharInfo->InventoryPackItem[i];
				if (item && item->Type==1 && item->Container.Capacity>0)
				{
					containers.push_back({ item, i });
					if (item->Container.IsOpen)
						opened_containers.push_back(item);
				}
			}
			if (opened_containers.size() == containers.size()) //all the containers are open..
			{
				Zeal::EqGame::EqGameInternal::CloseAllContainers(*Zeal::EqGame::ptr_ContainerMgr, 0);
			}
			else
			{
				for (auto& [c, index] : containers)
				{
					if (!c->Container.IsOpen)
						Zeal::EqGame::EqGameInternal::OpenContainer(*Zeal::EqGame::ptr_ContainerMgr, 0, (int)&c->Name, 22 + index);
				}
			}


		}
		});
	add_bind(216, "Toggle last two targets", "ToggleLastTwo", key_category::Target, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
			if (target)
			{
				if (last_targets.first == target->SpawnId && last_targets.second!=0)
				{
					Zeal::EqStructures::Entity* ent = Zeal::EqGame::get_entity_by_id(last_targets.second);
					if (ent)
						Zeal::EqGame::set_target(ent);
				}
			}
			else
			{
				if (last_targets.first != 0)
				{
					Zeal::EqStructures::Entity* ent = Zeal::EqGame::get_entity_by_id(last_targets.first);
					if (ent)
						Zeal::EqGame::set_target(ent);
				}
			}
		}
		});
	add_bind(217, "Reply Target", "ReplyTarget", key_category::Target, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqGame::EqGameInternal::ReplyTarget(Zeal::EqGame::get_self(), "");
		}
		});
	add_bind(218, "Pet Attack", "PetAttack", key_category::Commands, [this](int key_down) { //probably need to add a check if you have a pet
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
			if (target)
				Zeal::EqGame::pet_command(Zeal::EqEnums::PetCommand::Attack, target->SpawnId);
		}
		});
	add_bind(219, "Pet Guard", "PetGuard", key_category::Commands, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqGame::pet_command(Zeal::EqEnums::PetCommand::Guard, 0);
		}
		});
	add_bind(220, "Pet Back", "PetBack", key_category::Commands, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqGame::pet_command(Zeal::EqEnums::PetCommand::Back, 0);
		}
		});
	add_bind(221, "Pet Follow", "PetFollow", key_category::Commands, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqGame::pet_command(Zeal::EqEnums::PetCommand::Follow, 0);
		}
		});
	add_bind(222, "Pet Sit", "PetSit", key_category::Commands, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqGame::pet_command(Zeal::EqEnums::PetCommand::Sit, 0);
		}
		});
	add_bind(223, "Slow Turn Right", "SlowMoveRight", key_category::Movement, [this](int key_down) {
		if (!Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqGame::execute_cmd(5, key_down, 0);
			if (key_down)
			{
				mem::write<BYTE>(0x53fb60, 4);
				mem::write<BYTE>(0x53fb66, 4);
			}
			else
			{
				if (*(BYTE*)0x53fb60 != 12)
				{
					mem::write<BYTE>(0x53fb60, 12);
					mem::write<BYTE>(0x53fb66, 12);
				}
			}
			
		}
		});
	add_bind(224, "Slow Turn Left", "SlowMoveLeft", key_category::Movement, [this](int key_down) {
		if (!Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqGame::execute_cmd(6, key_down, 0);
			if (key_down)
			{
				mem::write<BYTE>(0x53f758, 4);
				mem::write<BYTE>(0x53f75E, 4);
			}
			else
			{
				if (*(BYTE*)0x53f758 != 12)
				{
					mem::write<BYTE>(0x53f758, 12);
					mem::write<BYTE>(0x53f75E, 12);
				}
			}
		}
		});
	add_bind(225, "Auto Fire", "AutoFire", key_category::Commands, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->autofire->SetAutoFire(!ZealService::get_instance()->autofire->autofire, true);
		}
		});
	add_bind(226, "Target Nearest NPC Corpse", "TargetNPCCorpse", key_category::Target, [](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqStructures::Entity* ent = ZealService::get_instance()->cycle_target->get_nearest_ent(250, 2);
			if (ent)
				Zeal::EqGame::set_target(ent);
		}
		});
	add_bind(227, "Target Nearest PC Corpse", "TargetPCCorpse", key_category::Target, [](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqStructures::Entity* ent = ZealService::get_instance()->cycle_target->get_nearest_ent(250, 3);
			if (ent)
				Zeal::EqGame::set_target(ent);
		}
	});
	add_bind(228, "Toggle Map", "ToggleMap", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->zone_map->set_enabled(!ZealService::get_instance()->zone_map->is_enabled());
		}
		});
	add_bind(229, "Toggle Map Background", "ToggleMapBackground", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->zone_map->toggle_background();
		}
		});
	add_bind(230, "Toggle Map Zoom", "ToggleMapZoom", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->zone_map->toggle_zoom();
		}
		});
	add_bind(231, "Toggle Map Labels", "ToggleMapLabels", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->zone_map->toggle_labels();
		}
		});
	add_bind(232, "Toggle Map Level Up", "ToggleMapLevelUp", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->zone_map->toggle_level_up();
		}
		});
	add_bind(233, "Toggle Map Level Down", "ToggleMapLevelDown", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->zone_map->toggle_level_down();
		}
		});
	add_bind(234, "Toggle Map Show Raid", "ToggleMapShowRaid", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			ZealService::get_instance()->zone_map->set_show_raid(
				!ZealService::get_instance()->zone_map->is_show_raid_enabled(), false);
		}
		});
	add_bind(235, "Toggle Nameplate Colors", "ToggleNameplateColors", key_category::Target, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			ZealService::get_instance()->nameplate->setting_colors.toggle(false);
		});
	add_bind(236, "Toggle Nameplate Con Colors", "ToggleNameplateConColors", key_category::Target, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			ZealService::get_instance()->nameplate->setting_con_colors.toggle(false);
		});
	add_bind(237, "Toggle Map Member Names", "FlashMapMemberNames", key_category::UI, [this](int key_down) {
		// Left the short name as "Flash" to stay consistent with previous keybinds.
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck()) {
			ZealService::get_instance()->zone_map->set_show_all_names_override(
				!ZealService::get_instance()->zone_map->is_show_all_names_override());
		}
		});
	add_bind(238, "Toggle Nameplate Self", "ToggleNameplateSelf", key_category::Target, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			ZealService::get_instance()->nameplate->setting_hide_self.toggle(false);
		});
	add_bind(239, "Toggle Nameplate Self as X", "ToggleNameplateX", key_category::Target, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			ZealService::get_instance()->nameplate->setting_x.toggle(false);
		});
	add_bind(240, "Toggle Nameplate Raid Pets", "ToggleNameplateRaidPets", key_category::Target, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			ZealService::get_instance()->nameplate->setting_hide_raid_pets.toggle(false);
		});
	add_bind(241, "Toggle Map Grid Lines", "ToggleMapGridLines", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck()) 
			ZealService::get_instance()->zone_map->set_show_grid(
				!ZealService::get_instance()->zone_map->is_show_grid_enabled(), false);
		});
	add_bind(242, "Toggle Map Interactive Mode", "ToggleMapInteractiveMode", key_category::UI, [this](int key_down) {
		if (key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			 ZealService::get_instance()->zone_map->set_interactive_enable(
				!ZealService::get_instance()->zone_map->is_interactive_enabled(), false);
		});
	add_bind(243, "Cycle through near PC corpses", "CycleTargetPCCorpses", key_category::Target, [](int key_down) {
			cycle_targets(key_down, Zeal::EqEnums::PlayerCorpse);
		});
	add_bind(255, "Auto Inventory", "AutoInventory", key_category::Commands | key_category::Macros, [](int key_down)
	{
		if (key_down)
		{
			if (Zeal::EqGame::can_inventory_item(Zeal::EqGame::get_char_info()->CursorItem))
			{
				Zeal::EqGame::EqGameInternal::auto_inventory(Zeal::EqGame::get_char_info(), &Zeal::EqGame::get_char_info()->CursorItem, 0);
			}
			else
			{
				if (Zeal::EqGame::get_char_info()->CursorItem)
					Zeal::EqGame::print_chat(USERCOLOR_LOOT, "Cannot auto inventory %s", Zeal::EqGame::get_char_info()->CursorItem->Name);
			}
		}
	});

}

void Binds::add_bind(int cmd, const char* name, const char* short_name, int category, std::function<void(int state)> callback)
{

	char* str = new char[64]; 
	strcpy_s(str, 64, short_name);
	KeyMapNames[cmd] = str;

	int options = ZealService::get_instance()->binds_hook->ptr_binds;
	if (!options)
		return;
	Zeal::EqGame::EqGameInternal::InitKeyBindStr((options + cmd * 0x8 + 0x20c), 0, (char*)name);
	*(int*)((options + cmd * 0x8 + 0x210)) = category;
	KeyMapFunctions[cmd] = callback;
}

void Binds::replace_cmd(int cmd, std::function<bool(int state)> callback)
{
	ReplacementFunctions[cmd].push_back(callback);
}

void Binds::main_loop()
{

	if (Zeal::EqGame::get_target() && Zeal::EqGame::get_target()->SpawnId != last_targets.first)
	{
		last_targets.second = last_targets.first;
		last_targets.first = Zeal::EqGame::get_target()->SpawnId;
	}
}

void Binds::on_zone()
{
	last_targets.first = 0;
	last_targets.second = 0;
}


Binds::Binds(ZealService* zeal)
{
	zeal->callbacks->AddGeneric([this]() { main_loop(); }, callback_type::MainLoop);
	zeal->callbacks->AddGeneric([this]() { on_zone(); }, callback_type::Zone);
	zeal->callbacks->AddCommand([this](UINT opcode, bool state) { return execute_cmd(opcode, state); }, callback_type::ExecuteCmd);
	for (int i = 0; i < 128; i++)
		KeyMapNames[i] = *(char**)(0x611220 + (i * 4)); //copy the original short names to the new array
	mem::write(0x52507A, (int)KeyMapNames);//write ini keymap
	mem::write(0x5254D9, (int)KeyMapNames);//clear ini keymap
	mem::write(0x525544, (int)KeyMapNames);//read ini keymap
	mem::write(0x42C52F, (byte)0xEB); //remove the check for max index of 116 being stored in client ini
	mem::write(0x52485A, (int)256); //increase this for loop to look through all 256
	mem::write(0x52591C, (int)(Zeal::EqGame::ptr_AlternateKeyMap + (256 * 4))); //fix another for loop to loop through all 256
	zeal->hooks->Add("initbinds", Zeal::EqGame::EqGameInternal::fn_initkeyboardassignments, InitKeyboardAssignments, hook_type_detour);
	
}
