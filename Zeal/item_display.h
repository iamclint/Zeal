#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ItemDisplay
{
public:
	ItemDisplay(class ZealService* pHookWrapper, class IO_ini* ini);
	~ItemDisplay();
	Zeal::EqUI::ItemDisplayWnd* get_available_window(Zeal::EqStructures::_EQITEMINFO* item);
	DWORD orig_deconstructor;
private:
	std::vector<Zeal::EqUI::ItemDisplayWnd*> display_windows;
	void init_ui();
	void clean_ui();
	const int max_item_displays = 5;
};


