#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class Melody
{
public:
	bool start(const std::vector<int>& new_songs); //returns true if no errors
	void end();
	void handle_stop_cast_callback(BYTE reason, SHORT spell_id);
	Melody(class ZealService* pHookWrapper, class IO_ini* ini);
	~Melody();
private:
	void tick();
	void stop_current_cast();
	int current_index = 0;  // Active song index. -1 if not started yet.
	std::vector<int> songs; // Gem indices (base 0) for melody.
	int retry_count = 0; // Tracks unsuccessful song casts.
	short casting_melody_song = -1; // Current melody song being cast, or -1. This is used to determine if an interruptted song was the current melody song.
};

