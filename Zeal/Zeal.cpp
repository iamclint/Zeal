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
	this->apply_patches();

	camera_mods = std::shared_ptr<CameraMods>(new CameraMods(this, ini.get()));
	cycle_target = std::shared_ptr<CycleTarget>(new CycleTarget(this));
	experience = std::shared_ptr<Experience>(new Experience(this));
	chat_hook = std::shared_ptr<chat>(new chat(this, ini.get()));
	outputfile = std::shared_ptr<OutputFile>(new OutputFile(this));
  buff_timers = std::shard_ptr<BuffTimers>(new BuffTimers(this));


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
