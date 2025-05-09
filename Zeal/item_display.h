#pragma once
#include "EqStructures.h"
#include "EqUI.h"
#include "IO_ini.h"
#include <vector>
#include "ZealSettings.h"

class ItemDisplay
{
public:
	ItemDisplay(class ZealService* pHookWrapper);
	~ItemDisplay();
	Zeal::EqUI::ItemDisplayWnd* get_available_window(Zeal::EqStructures::EQITEMINFOBASE* item = nullptr);
	std::vector<Zeal::EqUI::ItemDisplayWnd*> get_windows() { return windows; }  // For short-term use only.
	bool close_latest_window();
	void add_to_cache(const Zeal::EqStructures::EQITEMINFO* item);
	const Zeal::EqStructures::EQITEMINFO* get_cached_item(int item_id) const;
	ZealSetting<bool> setting_enhanced_spell_info = { true, "Zeal", "EnhancedSpellInfo", false };

private:
	void InitUI();
	void CleanUI();
	void DeactivateUI();
	const int max_item_displays = 5;
	std::vector<Zeal::EqUI::ItemDisplayWnd*> windows;

	std::unordered_map<int, Zeal::EqStructures::EQITEMINFO> item_cache;  // Cache of all displayed items.
};


