#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class ui_zoneselect
{
public:
	void Show();
	void Hide();
	void ShowButton();
	void HideButton();
	std::unordered_map<std::string, int> zones;
	ui_zoneselect(class ZealService* zeal, class UIManager* mgr);
	~ui_zoneselect();
private:
	Zeal::EqUI::EQWND* wnd = nullptr;
	Zeal::EqUI::EQWND* btn_wnd = nullptr;  // Optional button if not in xml.
	void InitUI();  // Called in InitCharSelectUI().
	void Deactivate();
	void CleanUI();
	UIManager* ui;
};

