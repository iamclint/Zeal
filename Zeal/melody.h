#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class Melody
{
public:
	bool start(const std::vector<int>& new_songs); //returns true if no errors
	void end();
	void handle_stop_cast_callback(BYTE reason, WORD spell_id);
	Melody(class ZealService* pHookWrapper, class IO_ini* ini);
	~Melody();
private:
	void tick();
	void stop_current_cast();
	int current_index = 0;  // Active song index. -1 if not started yet.
	std::vector<int> songs; // Gem indices (base 0) for melody.
	int retry_count = 0; // Tracks unsuccessful song casts.
	WORD casting_melody_spell_id = kInvalidSpellId; // Current melody song being cast. Is only a valid id while cast window is visible (actively casting).
};

