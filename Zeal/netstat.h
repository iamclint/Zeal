#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "IO_ini.h"

class Netstat
{
public:
	void toggle_netstat(int keydown);
	Netstat(class ZealService* zeal, class IO_ini* ini);
	~Netstat() {};
private:
	void callback_main();
	void callback_characterselect();
	void load_settings();
	void update_netstat_state();
	bool is_visible;
	IO_ini* ini_handle;

	bool netstat_flag_was_reset = true;
};
