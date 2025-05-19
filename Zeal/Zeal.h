#pragma once
#include "framework.h"
#include "EqAddresses.h"
#include <crtdbg.h>
#include <typeinfo>
#include <memory>
#include <iostream>
#define ZEAL_VERSION "0.6.8-beta3"
#ifndef ZEAL_BUILD_VERSION  // Set by github actions
#define ZEAL_BUILD_VERSION "UNOFFICIAL"  // Local build
#endif
#define MakeCheckedShared(T, ...) MakeCheckedSharedImpl<T>(__FILE__, __LINE__, this, __VA_ARGS__)

template<typename T, typename Parent, typename... Args>
using constructable = std::is_constructible<T, Parent*, Args...>;

static std::atomic<bool> exitFlag(false);
class ZealService
{
public:
	//hooks
	std::shared_ptr<CrashHandler> crash_handler = nullptr;
	std::shared_ptr<IO_ini> ini = nullptr;
	std::shared_ptr<HookWrapper> hooks = nullptr;
	std::shared_ptr<NamedPipe> pipe = nullptr;
	std::shared_ptr<Looting> looting_hook = nullptr;
	std::shared_ptr<Labels> labels_hook = nullptr;
	std::shared_ptr<Binds> binds_hook = nullptr;
	std::shared_ptr<ChatCommands> commands_hook = nullptr;
	std::shared_ptr<CallbackManager> callbacks = nullptr;
	std::shared_ptr<CameraMods> camera_mods = nullptr;
	std::shared_ptr<Raid> raid_hook = nullptr;
	std::shared_ptr<EQStr> eqstr_hook = nullptr;
	std::shared_ptr<EquipItem> equip_item_hook = nullptr;
	std::shared_ptr<Chat> chat_hook = nullptr;
	std::shared_ptr<chatfilter> chatfilter_hook = nullptr;
	std::shared_ptr<SpellSets> spell_sets = nullptr;
	std::shared_ptr<ItemDisplay> item_displays = nullptr;
	std::shared_ptr<Tooltip> tooltips = nullptr;
	std::shared_ptr<Physics> physics = nullptr;
	std::shared_ptr<FloatingDamage> floating_damage = nullptr;
	std::shared_ptr<DirectX> dx = nullptr;
	std::shared_ptr<NPCGive> give = nullptr;
	std::shared_ptr<NamePlate> nameplate = nullptr;
	std::shared_ptr<TellWindows> tells = nullptr;
	std::shared_ptr<HelmManager> helm = nullptr;
	std::shared_ptr<MusicManager> music = nullptr;
	std::shared_ptr <CharacterSelect> charselect = nullptr;
	std::shared_ptr<Tick> tick = nullptr;
	std::shared_ptr<Survey> survey = nullptr;

	//other features
	std::shared_ptr<OutputFile> outputfile = nullptr;
	std::shared_ptr<Experience> experience = nullptr;
	std::shared_ptr<CycleTarget> cycle_target = nullptr;
	std::shared_ptr<Assist> assist = nullptr;
	std::shared_ptr<BuffTimers> buff_timers = nullptr;
	std::shared_ptr<PlayerMovement> movement = nullptr;
	std::shared_ptr<Alarm> alarm = nullptr;
	std::shared_ptr<Netstat> netstat = nullptr;
	std::shared_ptr<UIManager> ui = nullptr;
	std::shared_ptr<Melody> melody = nullptr;
	std::shared_ptr<AutoFire> autofire = nullptr;
	std::shared_ptr<TargetRing> target_ring = nullptr;
	std::shared_ptr<ZoneMap> zone_map = nullptr;
	std::shared_ptr<Patches> game_patches = nullptr;
	std::shared_ptr<EntityManager> entity_manager = nullptr;
	

	std::vector<std::string> print_buffer;
	ZealService();
	~ZealService();
	static ZealService* ptr_service;
	//static data/functions to get a base ptr since some hook callbacks don't have the information required
	static ZealService* get_instance();
	static int heap_failed_line;  // Non-zero if heap check failed during boot.
	void configuration_check();

	bool exit = false;
private:
	std::thread render_thread;
	BYTE orig_render_data[11];
	void basic_binds();
};


template <typename T, typename Parent, typename... Args>
std::shared_ptr<T> MakeCheckedSharedImpl(const char* file, int line, Parent* parent, Args&&... args)
{
	std::shared_ptr<T> ptr;
	static bool HasBeenNotified = false;
	if constexpr (constructable<T, Parent, Args...>::value) { //if it will accept the this pointer from zeal then pass it in, if not just use the arguments given
		ptr = std::make_shared<T>(parent, std::forward<Args>(args)...);
	}
	else {
		ptr = std::make_shared<T>(std::forward<Args>(args)...);
	}
	int result1 = HeapValidate(GetProcessHeap(), 0, NULL);
	int result2 = HeapValidate(*Zeal::EqGame::Heap, 0, NULL);
	if (result1 && result2)
		return ptr;
	if (HasBeenNotified)
		return ptr;
	ZealService::heap_failed_line = line;
	std::stringstream ss;
	ss << "Heap corruption detected after allocating " << typeid(T).name() << " at " << file << ":" << line << "\n";
	ss << "This may be a false positive or it may be real and the game *might* crash later.\n";
	ss << "You can choose to either abort so you can restart eqgame.exe, retry the check, or ignore this and continue..\n";
	int result_id = MessageBoxA(NULL, ss.str().c_str(), "Zeal boot heap check", MB_ABORTRETRYIGNORE | MB_ICONWARNING); 
	if (result_id == IDABORT)
		throw std::bad_alloc();// Will crash out the program.
	HasBeenNotified = true;
	return ptr;
}

