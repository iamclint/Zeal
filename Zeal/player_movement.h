#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "ZealSettings.h"
enum strafe_direction
{
	None,
	Left,
	Right
};

class PlayerMovement
{
public:
	void handle_movement_binds(int cmd, bool key_down);
	void handle_spellcast_binds(int cmd);
	//void set_spellbook_autostand(bool enabled);
	//bool spellbook_autostand = false;
	ZealSetting<bool> CastAutoStand = { true, "Zeal", "CastAutoStand", false };
	ZealSetting<bool> SpellBookAutoStand = { true, "Zeal", "SpellBookAutoStand", false };
	PlayerMovement(class ZealService* zeal);
private:
	void set_strafe(strafe_direction dir);
	void callback_main();
	strafe_direction current_strafe = strafe_direction::None;
	BYTE orig_reset_strafe[7] = { 0 };
};

