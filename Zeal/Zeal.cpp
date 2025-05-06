#include "Zeal.h"
#include "EqAddresses.h"
#include "ZealSettings.h"
#include <filesystem>
#include <Windows.h>

ZealService* ZealService::ptr_service = nullptr;
int ZealService::heap_failed_line = 0;
ZealService::ZealService()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//since the hooked functions are called back via a different thread, make sure the service ptr is available immediately
	ZealService::ptr_service = this; //this setup makes it not unit testable but since the caller functions of the hooks don't know the pointers I had to make a method to retrieve the base atleast
	crash_handler = MakeCheckedShared(CrashHandler);
	hooks = MakeCheckedShared(HookWrapper);
	ini = MakeCheckedShared(IO_ini, ZealSetting<bool>::kIniFilename, true); //other functions rely on this hook
	dx = MakeCheckedShared(directx);
	//initialize the hooked function classes
	commands_hook = MakeCheckedShared(ChatCommands); //other classes below rely on this class on initialize
	callbacks = MakeCheckedShared(CallbackManager); //other functions rely on this hook
	looting_hook = MakeCheckedShared(looting);
	labels_hook = MakeCheckedShared(labels);
	pipe = MakeCheckedShared(named_pipe); //other classes below rely on this class on initialize
	binds_hook = MakeCheckedShared(Binds);
	raid_hook = MakeCheckedShared(raid);
	eqstr_hook = MakeCheckedShared(eqstr);
	equip_item_hook = MakeCheckedShared(EquipItem);
	spell_sets = MakeCheckedShared(SpellSets);
	item_displays = MakeCheckedShared(ItemDisplay);
	tooltips = MakeCheckedShared(tooltip);
	floating_damage = MakeCheckedShared(FloatingDamage);
	give = MakeCheckedShared(NPCGive);
	game_patches = MakeCheckedShared(patches);
	nameplate = MakeCheckedShared(NamePlate);
	tells = MakeCheckedShared(TellWindows);
	helm = MakeCheckedShared(HelmManager);
	music = MakeCheckedShared(MusicManager);
	charselect = MakeCheckedShared(CharacterSelect);
	entity_manager = MakeCheckedShared(EntityManager);
	camera_mods = MakeCheckedShared(CameraMods);
	cycle_target = MakeCheckedShared(CycleTarget);
	assist = MakeCheckedShared(Assist);
	experience = MakeCheckedShared(Experience);
	chat_hook = MakeCheckedShared(chat);
	chatfilter_hook = MakeCheckedShared(chatfilter);
	outputfile = MakeCheckedShared(OutputFile);
	buff_timers = MakeCheckedShared(BuffTimers);
	movement = MakeCheckedShared(PlayerMovement);
	alarm = MakeCheckedShared(Alarm);
	netstat = MakeCheckedShared(Netstat);
	ui = MakeCheckedShared(ui_manager);
	melody = MakeCheckedShared(Melody);
	autofire = MakeCheckedShared(AutoFire);
	physics = MakeCheckedShared(Physics);
	target_ring = MakeCheckedShared(TargetRing);
	zone_map = MakeCheckedShared(ZoneMap);
	survey = MakeCheckedShared(Survey);

	callbacks->AddGeneric([this]() {
		if (Zeal::EqGame::is_in_game() && print_buffer.size())
		{
			for (auto& str : print_buffer)
				Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Zeal: %s", str.c_str());
			print_buffer.clear();
		}
		
	});

	this->basic_binds();
	configuration_check();
}

void ZealService::configuration_check()
{
	std::filesystem::path zeal_ui_path = std::filesystem::current_path() /  "uifiles\\zeal";

	std::string xml_files[] = {"EQUI_GuildManagementWnd.xml",
	"EQUI_OptionsWindow.xml",
	"EQUI_Tab_Cam.xml",
	"EQUI_Tab_Colors.xml",
	"EQUI_Tab_FloatingDamage.xml",
	"EQUI_Tab_General.xml",
	"EQUI_Tab_Map.xml",
	"EQUI_Tab_Nameplate.xml",
	"EQUI_Tab_TargetRing.xml",
	"EQUI_ZealInputDialog.xml",
	"EQUI_ZealMap.xml",
	"EQUI_ZealOptions.xml"};

	std::string deprecated_ui_file = "EQUI.xml";

	bool filepathExists = std::filesystem::is_directory(zeal_ui_path);
	if (not filepathExists)
	{
		std::wstring missing = L"The directory 'zeal' is missing from the EQ uifiles directory.\nZeal will not function properly!";
		MessageBox(NULL, missing.c_str(), L"Zeal installation error", MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		std::filesystem::path deprecated_filepath = zeal_ui_path / deprecated_ui_file;
		if (std::filesystem::exists(deprecated_filepath))
		{
			std::wstring deprecated_warning = L"The deprecated Zeal 'EQUI.xml' file has been detected at the following location:\n" +
				deprecated_filepath.wstring() + L"\n" + L"Zeal will not function properly unless this file is removed.";
				MessageBox(NULL, deprecated_warning.c_str(), L"Zeal deprecated file", MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			std::wstring missing_files =  L"";
			for (std::string file : xml_files)
			{
				std::filesystem::path this_file = zeal_ui_path / file;

				if (not std::filesystem::exists(this_file))
					missing_files += this_file.wstring() + L"\n";
			}
			if (missing_files.length() > 0)
			{
				missing_files = L"The following files are missing from your 'zeal\\uifiles' directory:\n" + missing_files + L"\nZeal may not function properly!";
				MessageBox(NULL, missing_files.c_str(), L"Zeal installation error", MB_OK | MB_ICONEXCLAMATION);
			}
		}
	}
}

void ZealService::basic_binds()
{
	binds_hook->replace_cmd(28, [this](int state)
		{
			if (state && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			{
				Zeal::EqStructures::Entity* ent = ZealService::get_instance()->cycle_target->get_nearest_ent(250, 0);
				if (ent)
					Zeal::EqGame::set_target(ent);
			}
			return true;
		}); //nearest pc
	binds_hook->replace_cmd(29, [this](int state)
		{
			if (state && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
			{
				Zeal::EqStructures::Entity* ent = ZealService::get_instance()->cycle_target->get_nearest_ent(250, 1);
				if (ent)
					Zeal::EqGame::set_target(ent);
			}
			return true;
		}); //nearest npc
	binds_hook->replace_cmd(3, [this](int state) 
	{
		movement->handle_movement_binds(3, state);
		return false;
	}); //foward

	binds_hook->replace_cmd(4, [this](int state) 
	{
		movement->handle_movement_binds(4, state);
		return false;
	}); //back

	binds_hook->replace_cmd(5, [this](int state) 
	{
		camera_mods->handle_camera_motion_binds(5, state);
		movement->handle_movement_binds(5, state);
		return false;
	}); //turn right

	binds_hook->replace_cmd(6, [this](int state) 
	{
		camera_mods->handle_camera_motion_binds(6, state);
		movement->handle_movement_binds(6, state);
		return false;
	}); //turn left

	binds_hook->replace_cmd(30, [this](int state) 
	{
			netstat->toggle_netstat(state);
			return false;
	});

	for (int bind_index = 51; bind_index < 59; ++bind_index) 
	{
		binds_hook->replace_cmd(bind_index, [this, bind_index](int state) {
			movement->handle_spellcast_binds(bind_index);
			return false;
		}); // spellcasting auto-stand
	}

	binds_hook->replace_cmd(0xC8, [this](int state) 
	{
		if (ZealService::get_instance()->ui->inputDialog->isVisible())
		{
			ZealService::get_instance()->ui->inputDialog->hide();
			return true;
		}
		if (Zeal::EqGame::get_target())
		{
			Zeal::EqGame::set_target(0);
			return true;
		}

		if (!ini->getValue<bool>("Zeal", "EscapeRaidLock"))
		{

			if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->RaidOptions && Zeal::EqGame::Windows->RaidOptions->IsVisible)
			{
				Zeal::EqGame::Windows->RaidOptions->show(0, false);
				return true;
			}
			if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->Raid && Zeal::EqGame::Windows->Raid->IsVisible)
			{
				Zeal::EqGame::execute_cmd(109, 1, 0);
				Zeal::EqGame::execute_cmd(109, 0, 0);
				return true;
			}
		}

		if (ini->getValue<bool>("Zeal", "Escape"))//toggle is set to not close any windows
			return true;

		if (item_displays->close_latest_window())
			return true;

		return false;
	}); //handle escape
}


ZealService* ZealService::get_instance()
{
	return ptr_service;
}
ZealService::~ZealService()
{
	hooks.reset();
	autofire.reset();
	melody.reset();
	nameplate.reset();
	target_ring.reset();
	zone_map.reset();
	ui.reset();
	netstat.reset();
	alarm.reset();
	movement.reset();
	buff_timers.reset();
	outputfile.reset();
	chat_hook.reset();
	chatfilter_hook.reset();
	experience.reset();
	cycle_target.reset();
	assist.reset();
	camera_mods.reset();
	item_displays.reset();
	spell_sets.reset();
	eqstr_hook.reset();
	equip_item_hook.reset();
	raid_hook.reset();
	binds_hook.reset();
	labels_hook.reset();
	looting_hook.reset();
	tick.reset();
	survey.reset();
	callbacks.reset();
	helm.reset();
	commands_hook.reset();
	ini.reset();

}
