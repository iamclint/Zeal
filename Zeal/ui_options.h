#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_options
{
public:
	Zeal::EqUI::EQWND* wnd = nullptr;
	void UpdateOptions();
	void UpdateOptionsMap();
	void UpdateOptionsTargetRing();
	void UpdateOptionsNameplate();
	void UpdateOptionsCamera();
	void UpdateOptionsGeneral();
	void InitColors();
	void InitGeneral();
	void InitMap();
	void InitCamera();
	void InitTargetRing();
	void InitNameplate();
	void SaveColors();
	void LoadColors();
	DWORD GetColor(int index);
	ui_options(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_options();
private:
	bool isReady = false;
	void InitUI();
	void CleanUI();
	void RenderUI();
	void Deactivate();
	void LoadSettings(class IO_ini* ini);

	std::unordered_map<int, Zeal::EqUI::BasicWnd*> color_buttons;
	ui_manager* ui;
};
