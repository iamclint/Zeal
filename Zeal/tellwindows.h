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
	void SetHist(bool val);
	Zeal::EqUI::ChatWnd* FindTellWnd(std::string& name);
	void AddOutputText(Zeal::EqUI::ChatWnd*& wnd, std::string& msg, short channel);
	bool enabled = false;
	bool hist_enabled = true;
private:
	Zeal::EqUI::ChatWnd* FindPreviousTellWnd();
	Zeal::EqUI::ChatWnd* FindNextTellWnd();
	std::unordered_map<std::string, std::vector<std::pair<short, std::string>>> tell_cache;
	void CleanUI();
	void LoadUI();
};

