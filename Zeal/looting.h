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
	void handle_hide_looted();
	void set_last_hidden_corpse(Zeal::EqStructures::Entity* corpse);
	looting(class ZealService* zeal);
	~looting();

	ZealSetting<bool> setting_alt_delimiter = { false, "Zeal", "LinkAllAltDelimiter", false };

	// /protect functionality.  Command line-only for now.
	bool is_cursor_protected(const Zeal::EqStructures::EQCHARINFO* char_info) const;
	bool is_item_protected_from_selling(const Zeal::EqStructures::EQITEMINFO* item_info) const;
	bool is_trade_protected(struct Zeal::EqUI::TradeWnd* wnd) const;

protected:
	ZealSetting<bool> setting_protect_enable = { false, "Protect", "Enabled", true };
	ZealSetting<int> setting_protect_value = { 10, "Protect", "Value", true };
	ZealSetting<int> setting_loot_last_item = { 0, "Zeal", "LootLastItem", true };

	struct ProtectedItem {
		int id;
		std::string name;
	};
	bool parse_loot_last(const std::vector<std::string>& args);
	bool parse_protect(const std::vector<std::string>& args);
	void update_protected_item(int item_id, const std::string& name, bool add_only = false);
	void load_protected_items();
	void unhide_last_hidden();

	std::vector<ProtectedItem> protected_items;
	Zeal::EqStructures::Entity* last_hidden_entity = nullptr;
	int last_hidden_spawnid = 0;

private:
	int last_looted = -1;
};
