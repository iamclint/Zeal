#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class ui_zoneselect
{
public:
	void Show();
	void Hide();
	std::unordered_map<std::string, int> zones;
	ui_zoneselect(class ZealService* zeal, class ui_manager* mgr);
	~ui_zoneselect();
private:
	Zeal::EqUI::EQWND* wnd = nullptr;
	void InitUI();  // Called in InitCharSelectUI().
	void Deactivate();
	void CleanUI();
	ui_manager* ui;
};

