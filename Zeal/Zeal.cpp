#include "Zeal.h"
ZealService* ZealService::ptr_service = nullptr;


ZealService::ZealService()
{
	//since the hooked functions are called back via a different thread, make sure the service ptr is available immediately
	ZealService::ptr_service = this; //this setup makes it not unit testable but since the caller functions of the hooks don't know the pointers I had to make a method to retrieve the base atleast
	hooks = std::make_shared<HookWrapper>();
	ini = std::make_shared<IO_ini>(".\\eqclient.ini"); //other functions rely on this hook
	//initialize the hooked function classes
	commands_hook = std::make_shared<ChatCommands>(this); //other classes below rely on this class on initialize
	callbacks = std::make_shared<CallbackManager>(this); //other functions rely on this hook
	looting_hook = std::make_shared<looting>(this);
	labels_hook = std::make_shared<labels>(this);
	binds_hook = std::make_shared<Binds>(this);
	raid_hook = std::make_shared<raid>(this);
	eqstr_hook = std::make_shared<eqstr>(this);
	spell_sets = std::make_shared<SpellSets>(this);
	item_displays = std::make_shared<ItemDisplay>(this, ini.get());
	this->apply_patches();

	camera_mods = std::make_shared<CameraMods>(this, ini.get());
	cycle_target = std::make_shared<CycleTarget>(this);
	experience = std::make_shared<Experience>(this);
	chat_hook = std::make_shared<chat>(this, ini.get());
	outputfile = std::make_shared<OutputFile>(this);
	buff_timers = std::make_shared<BuffTimers>(this);
	movement = std::make_shared<PlayerMovement>(this, binds_hook.get(), ini.get());
	alarm = std::make_shared<Alarm>(this);
	netstat = std::make_shared<Netstat>(this, ini.get());
	ui = std::make_shared<ui_manager>(this, ini.get());
	melody = std::make_shared<Melody>(this, ini.get());
	binds_hook->replace_bind(3, [this](int state) {
		movement->handle_movement_binds(3, state);
		return false;
	}); //foward
	binds_hook->replace_bind(4, [this](int state) {
		movement->handle_movement_binds(4, state);
		return false;
	}); //back
	binds_hook->replace_bind(5, [this](int state) {
		camera_mods->handle_camera_motion_binds(5, state);
		movement->handle_movement_binds(5, state);
		return false;
	}); //turn right
	binds_hook->replace_bind(6, [this](int state) {
		camera_mods->handle_camera_motion_binds(6, state);
		movement->handle_movement_binds(6, state);
		return false;
	}); //turn left
	binds_hook->replace_bind(30, [this](int state) {
		netstat->toggle_netstat(state);
		return false;
	});
	for (int bind_index = 51; bind_index < 59; ++bind_index) {
		binds_hook->replace_bind(bind_index, [this, bind_index](int state) {
			movement->handle_spellcast_binds(bind_index);
			return false;
		}); // spellcasting auto-stand
	}
	binds_hook->replace_bind(72, [this](int state) {
		Zeal::EqGame::get_self()->ChangeStance(Stance::Sit);
		return false;
	}); // hotkey camp auto-sit

	
}


void ZealService::apply_patches()
{
	const char sit_stand_patch[] = { (char)0xEB, (char)0x1A };
	mem::write(0x42d14d, sit_stand_patch); //fix pet sit shortcut crash (makes default return of function the sit/stand button not sure why its passing in 0)


	//disable client sided mana ticking
	mem::set(0x4C3F93, 0x90, 7);
	mem::set(0x4C7642, 0x90, 7);
}

ZealService* ZealService::get_instance()
{
	return ptr_service;
}
ZealService::~ZealService()
{
	
}
