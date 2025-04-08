#pragma once
#include "EqStructures.h"
#include "EqUI.h"
#include "IO_ini.h"
#include <vector>

class ItemDisplay
{
public:
	ItemDisplay(class ZealService* pHookWrapper, class IO_ini* ini);
	~ItemDisplay();
	Zeal::EqUI::ItemDisplayWnd* get_available_window(Zeal::EqStructures::EQITEMINFOBASE* item = nullptr);
	std::vector<Zeal::EqUI::ItemDisplayWnd*> get_windows() { return windows; }  // For short-term use only.
	bool close_latest_window();
	void add_to_cache(const Zeal::EqStructures::EQITEMINFO* item);
	const Zeal::EqStructures::EQITEMINFO* get_cached_item(int item_id) const;

private:
	void InitUI();
	void CleanUI();
	void DeactivateUI();
	const int max_item_displays = 5;
	std::vector<Zeal::EqUI::ItemDisplayWnd*> windows;

	std::unordered_map<int, Zeal::EqStructures::EQITEMINFO> item_cache;  // Cache of all displayed items.
};


