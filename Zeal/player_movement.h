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
	PlayerMovement(class ZealService* zeal, class Binds* binds, class IO_ini* ini);
	~PlayerMovement() {};
private:
	void set_strafe(strafe_direction dir);
	void callback_main();
	void load_settings(IO_ini* ini);
	bool spellbook_left_autostand;
	bool spellbook_right_autostand;
	bool spellbook_left_strafe_autostand;
	bool spellbook_right_strafe_autostand;
	strafe_direction current_strafe = strafe_direction::None;
	BYTE orig_reset_strafe[7] = { 0 };
};

