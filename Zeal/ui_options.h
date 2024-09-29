#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_options
{
public:
	Zeal::EqUI::BasicWnd* wnd = nullptr;
	void UpdateOptions();
	void UpdateOptionsMap();
	void UpdateOptionsTargetRing();
	void UpdateOptionsCamera();
	void UpdateOptionsGeneral();
	void InitGeneral();
	void InitMap();
	void InitCamera();
	void InitTargetRing();
	ui_options(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_options();
private:
	bool isReady = false;
	void InitUI();
	void CleanUI();
	void RenderUI();
	void LoadSettings(class IO_ini* ini);
	ui_manager* ui;
};
