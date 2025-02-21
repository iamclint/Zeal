#pragma once
#include "framework.h"
#define ZEAL_VERSION "0.6.5-beta3"
#ifndef ZEAL_BUILD_VERSION  // Set by github actions
#define ZEAL_BUILD_VERSION "UNOFFICIAL"  // Local build
#endif
static std::atomic<bool> exitFlag(false);
class ZealService
{
public:
	//hooks
	std::shared_ptr<CrashHandler> crash_handler = nullptr;
	std::shared_ptr<IO_ini> ini = nullptr;
	std::shared_ptr<HookWrapper> hooks = nullptr;
	std::shared_ptr<named_pipe> pipe = nullptr;
	std::shared_ptr<looting> looting_hook = nullptr;
	std::shared_ptr<labels> labels_hook = nullptr;
	std::shared_ptr<Binds> binds_hook = nullptr;
	std::shared_ptr<ChatCommands> commands_hook = nullptr;
	std::shared_ptr<CallbackManager> callbacks = nullptr;
	std::shared_ptr<CameraMods> camera_mods = nullptr;
	std::shared_ptr<raid> raid_hook = nullptr;
	std::shared_ptr<eqstr> eqstr_hook = nullptr;
	std::shared_ptr<EquipItem> equip_item_hook = nullptr;
	std::shared_ptr<chat> chat_hook = nullptr;
	std::shared_ptr<chatfilter> chatfilter_hook = nullptr;
	std::shared_ptr<SpellSets> spell_sets = nullptr;
	std::shared_ptr<ItemDisplay> item_displays = nullptr;
	std::shared_ptr<tooltip> tooltips = nullptr;
	std::shared_ptr<Physics> physics = nullptr;
	std::shared_ptr<FloatingDamage> floating_damage = nullptr;
	std::shared_ptr<directx> dx = nullptr;
	std::shared_ptr<NPCGive> give = nullptr;
	std::shared_ptr<NamePlate> nameplate = nullptr;
	std::shared_ptr<TellWindows> tells = nullptr;
	std::shared_ptr<HelmManager> helm = nullptr;
	std::shared_ptr<MusicManager> music = nullptr;
	std::shared_ptr<Tick> tick = nullptr;

	//other features
	std::shared_ptr<OutputFile> outputfile = nullptr;
	std::shared_ptr<Experience> experience = nullptr;
	std::shared_ptr<CycleTarget> cycle_target = nullptr;
	std::shared_ptr<Assist> assist = nullptr;
	std::shared_ptr<BuffTimers> buff_timers = nullptr;
	std::shared_ptr<PlayerMovement> movement = nullptr;
	std::shared_ptr<Alarm> alarm = nullptr;
	std::shared_ptr<Netstat> netstat = nullptr;
	std::shared_ptr<ui_manager> ui = nullptr;
	std::shared_ptr<Melody> melody = nullptr;
	std::shared_ptr<AutoFire> autofire = nullptr;
	std::shared_ptr<TargetRing> target_ring = nullptr;
	std::shared_ptr<ZoneMap> zone_map = nullptr;
	std::shared_ptr<patches> game_patches = nullptr;
	std::shared_ptr<EntityManager> entity_manager = nullptr;
	

	std::vector<std::string> print_buffer;
	ZealService();
	~ZealService();
	static ZealService* ptr_service;
	//static data/functions to get a base ptr since some hook callbacks don't have the information required
	static ZealService* get_instance();
	void configuration_check();

	bool exit = false;
private:
	std::thread render_thread;
	BYTE orig_render_data[11];
	void basic_binds();
};


