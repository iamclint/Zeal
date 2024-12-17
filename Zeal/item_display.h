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
	bool close_latest_window();
private:
	void InitUI();
	void CleanUI();
	void DeactivateUI();
	const int max_item_displays = 5;
	std::vector<Zeal::EqUI::ItemDisplayWnd*> windows;
};


