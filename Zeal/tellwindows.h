#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
class TellWindows
{
public:
	bool HandleKeyPress(int key, bool down, int modifier);
	bool HandleTell(std::string& cmd_data);
	bool IsTellWindow(struct Zeal::EqUI::ChatWnd* wnd);
	TellWindows(class ZealService* zeal, class IO_ini* ini);
	~TellWindows();
	void SetEnabled(bool val);
	Zeal::EqUI::ChatWnd* FindTellWnd(std::string& name);
	bool enabled = false;
	
private:
	Zeal::EqUI::ChatWnd* FindPreviousTellWnd();
	Zeal::EqUI::ChatWnd* FindNextTellWnd();
	
	void CleanUI();
	void LoadUI();
};

