#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "ZealSettings.h"

class looting
{
public:
	void set_hide_looted(bool val);
	bool hide_looted;
	void link_all(const char* channel = nullptr) const;
	void init_ui();
	bool loot_all=false;
	ULONGLONG loot_next_item_time = 0;
	void looted_item();
	looting(class ZealService* zeal);
	~looting();

	ZealSetting<bool> setting_alt_delimiter = { false, "Zeal", "LinkAllAltDelimiter", false };
private:
	int last_looted = -1;
};
