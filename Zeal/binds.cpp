#include "binds.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
Binds::~Binds()
{
}

void ExecuteCmd(int cmd, int isdown, int unk2)
{
	ZealService* zeal = ZealService::get_instance();
	if (zeal->binds_hook->KeyMapFunctions.count(cmd) > 0)
		zeal->binds_hook->KeyMapFunctions[cmd](isdown);
	else
		zeal->hooks->hook_map["executecmd"]->original(ExecuteCmd)(cmd, isdown, unk2);
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

void set_strafe(strafe_direction dir) 
{
	//slightly revised so the game properly sets speed based on encumber and handles the stance checks
	if (dir == strafe_direction::None)
	{
		ZealService::get_instance()->hooks->hook_map["executecmd"]->original(ExecuteCmd)(0x5, 0, 0);
		ZealService::get_instance()->hooks->hook_map["executecmd"]->original(ExecuteCmd)(0x6, 0, 0);
		mem::write<byte>(0x53F633, 0x75); //reset the jump to be conditional
		mem::write<byte>(0x53FA3B, 0x75);
	}
	else
	{
		mem::write<byte>(0x53F633, 0xEB); //make this jump unconditional
		mem::write<byte>(0x53FA3B, 0xEB);
		if (dir == strafe_direction::Right)
			ZealService::get_instance()->hooks->hook_map["executecmd"]->original(ExecuteCmd)(0x5, 1, 0);
		else
			ZealService::get_instance()->hooks->hook_map["executecmd"]->original(ExecuteCmd)(0x6, 1, 0);

	}
}

void Binds::add_binds()
{
	//just start binds at 211 to avoid overwriting any existing cmd/bind
	add_bind(211, "Strafe Left", "StrafeLeft", key_category::Movement, [](int key_down) {
		if (!key_down )
			set_strafe(strafe_direction::None);
		else
			set_strafe(strafe_direction::Left);
	});
	add_bind(212, "Strafe Right", "StrafeRight", key_category::Movement, [](int key_down) {
		if (!key_down)
			set_strafe(strafe_direction::None);
		else
			set_strafe(strafe_direction::Right);
	});
	add_bind(213, "Cycle through nearest NPCs", "CycleTargetNPC", key_category::Target, [](int key_down) {

		if (!key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqStructures::Entity* ent = ZealService::get_instance()->cycle_target->get_next_ent(250, 1);
			if (ent)
				Zeal::EqGame::set_target(ent);
		}
	});
	add_bind(214, "Cycle through nearest PCs", "CycleTargetPC", key_category::Target, [](int key_down) {
		if (!key_down && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
		{
			Zeal::EqStructures::Entity* ent = ZealService::get_instance()->cycle_target->get_next_ent(250, 0);
			if (ent)
				Zeal::EqGame::set_target(ent);
		}
		});

	add_bind(255, "Auto Inventory", "AutoInventory", key_category::Commands | key_category::Macros, [](int key_down) {

		if (key_down)
		{
			int a1 = *Zeal::EqGame::ptr_LocalPC;
			int a2 = a1 + 0xD78;
			Zeal::EqGame::EqGameInternal::auto_inventory(a1, a2, 0);
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



Binds::Binds(ZealService* zeal)
{
	for (int i = 0; i < 128; i++)
		KeyMapNames[i] = *(char**)(0x611220 + (i * 4)); //copy the original short names to the new array
	mem::write(0x52507A, (int)KeyMapNames);//write ini keymap
	mem::write(0x5254D9, (int)KeyMapNames);//clear ini keymap
	mem::write(0x525544, (int)KeyMapNames);//read ini keymap
	mem::write(0x42C52F, (byte)0xEB); //remove the check for max index of 116 being stored in client ini
	mem::write(0x52485A, (int)256); //increase this for loop to look through all 256
	mem::write(0x52591C, (int)(Zeal::EqGame::ptr_AlternateKeyMap + (256 * 4))); //fix another for loop to loop through all 256
	zeal->hooks->Add("initbinds", Zeal::EqGame::EqGameInternal::fn_initkeyboardassignments, InitKeyboardAssignments, hook_type_detour);
	zeal->hooks->Add("executecmd", Zeal::EqGame::EqGameInternal::fn_executecmd, ExecuteCmd, hook_type_detour);
}
