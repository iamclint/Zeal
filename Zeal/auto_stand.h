#pragma once
#include "hook_wrapper.h"
#include "memory.h"
class AutoStand
{
public:
	void handle_movement_binds(int cmd, bool key_down);
	void handle_spellcast_binds(int cmd);
	AutoStand(class ZealService* zeal, class IO_ini* ini);
	~AutoStand();
private:
	void load_settings(IO_ini* ini);
	bool spellbook_left_autostand;
	bool spellbook_right_autostand;
};

