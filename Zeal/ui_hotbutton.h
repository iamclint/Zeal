#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include <map>

class hotbutton_state
{
public:
	void tick();
	bool active();
	void set(int _duration);
	Zeal::EqUI::BasicWnd* wnd = 0;
	hotbutton_state(Zeal::EqUI::BasicWnd* btn);
	hotbutton_state() {};
private:
	ULONGLONG start_time = 0;
	int duration = 0;
};

class ui_hotbutton
{
public:
	int last_button = 0;
	int last_page = 0;
	bool is_btn_active(Zeal::EqUI::BasicWnd* btn);
	ui_hotbutton(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_hotbutton();
private:
	std::unordered_map<int, Zeal::EqUI::BasicWnd*> buttons;
	std::unordered_map<int, std::unordered_map<int, hotbutton_state>> states;
	void InitUI();
	void Render();
	
	ui_manager* ui;
};
