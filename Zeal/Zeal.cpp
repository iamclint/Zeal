#include "Zeal.h"
ZealService* ZealService::ptr_service = nullptr;
ZealService::ZealService()
{
	//since the hooked functions are called back via a different thread, make sure the service ptr is available immediately
	ZealService::ptr_service = this; //this setup makes it not unit testable but since the caller functions of the hooks don't know the pointers I had to make a method to retrieve the base atleast
	hooks = std::shared_ptr<HookWrapper>(new HookWrapper());
	ini = std::shared_ptr<IO_ini>(new IO_ini(".\\eqclient.ini")); //other functions rely on this hook
	//initialize the hooked function classes
	commands_hook = std::shared_ptr<ChatCommands>(new ChatCommands(this)); //other classes below rely on this class on initialize
	main_loop_hook = std::shared_ptr<MainLoop>(new MainLoop(this)); //other functions rely on this hook
	looting_hook = std::shared_ptr<looting>(new looting(this));
	labels_hook = std::shared_ptr<labels>(new labels(this));
	binds_hook = std::shared_ptr<Binds>(new Binds(this));
	raid_hook = std::shared_ptr<raid>(new raid(this));
	eqstr_hook = std::shared_ptr<eqstr>(new eqstr(this));
	spell_sets = std::shared_ptr<SpellSets>(new SpellSets(this));
	this->apply_patches();

	camera_mods = std::shared_ptr<CameraMods>(new CameraMods(this, ini.get()));
	cycle_target = std::shared_ptr<CycleTarget>(new CycleTarget(this));
	experience = std::shared_ptr<Experience>(new Experience(this));
	chat_hook = std::shared_ptr<chat>(new chat(this, ini.get()));
	outputfile = std::shared_ptr<OutputFile>(new OutputFile(this));
	buff_timers = std::shared_ptr<BuffTimers>(new BuffTimers(this));
	auto_stand = std::shared_ptr<AutoStand>(new AutoStand(this));
	

	binds_hook->replace_bind(3, [this](int state) {
		auto_stand->handle_movement_binds(3, state);
		return false;
	}); //foward
	binds_hook->replace_bind(4, [this](int state) {
		auto_stand->handle_movement_binds(4, state);
		return false;
	}); //back
	binds_hook->replace_bind(5, [this](int state) {
		camera_mods->handle_camera_motion_binds(5, state); 
		auto_stand->handle_movement_binds(5, state);
		return false;
	}); //turn right
	binds_hook->replace_bind(6, [this](int state) {
		camera_mods->handle_camera_motion_binds(6, state); 
		auto_stand->handle_movement_binds(6, state);
		return false;
	}); //turn left
	for (int bind_index = 51; bind_index < 58; ++bind_index) {
		binds_hook->replace_bind(bind_index, [this, bind_index](int state) {
			auto_stand->handle_spellcast_binds(bind_index);
			return false;
		}); // spellcasting auto-stand
	}
	binds_hook->replace_bind(72, [this](int state) {
		Zeal::EqGame::change_stance(Stance::Sit);
		return false;
	}); // hotkey camp auto-sit

	looting_hook->hide_looted = ini->getValue<bool>("Zeal", "HideLooted"); //just remembers the state
}


void ZealService::apply_patches()
{
	const char sit_stand_patch[] = { (char)0xEB, (char)0x1A };
	mem::write(0x42d14d, sit_stand_patch); //fix pet sit shortcut crash (makes default return of function the sit/stand button not sure why its passing in 0)
}

ZealService* ZealService::get_instance()
{
	return ptr_service;
}
ZealService::~ZealService()
{
	
}
