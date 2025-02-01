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

	// /protect functionality.  Command line-only for now.
	bool is_cursor_protected(Zeal::EqStructures::EQCHARINFO* char_info);
protected:
	ZealSetting<bool> setting_protect_enable = { false, "Protect", "Enabled", true };
	ZealSetting<int> setting_protect_value = { 10, "Protect", "Value", true };

	struct ProtectedItem {
		int id;
		std::string name;
	};
	bool parse_protect(const std::vector<std::string>& args);
	void update_protected_item(int item_id, const std::string& name);
	void load_protected_items();
	std::vector<ProtectedItem> protected_items;

private:
	int last_looted = -1;
};
