#pragma once
#include "hook_wrapper.h"
#include "memory.h"

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
	void set_spellbook_autostand(bool enabled);
	bool spellbook_autostand = false;
	PlayerMovement(class ZealService* zeal, class Binds* binds, class IO_ini* ini);
	~PlayerMovement() {};
private:
	void set_strafe(strafe_direction dir);
	void callback_main();
	void load_settings();
	strafe_direction current_strafe = strafe_direction::None;
	BYTE orig_reset_strafe[7] = { 0 };
	IO_ini* ini_handle;
};

