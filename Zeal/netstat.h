#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "ZealSettings.h"

class Netstat
{
public:
	void toggle_netstat(int keydown);
	Netstat(class ZealService* zeal);
	~Netstat() {};
private:
	ZealSetting<bool> is_visible = { true, "Zeal", "NetstatVisibilityState", false };
	void callback_main();
	void callback_characterselect();
	void load_settings();
	void update_netstat_state();

	bool netstat_flag_was_reset = true;
};
