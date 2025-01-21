#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include "ZealSettings.h"

class ui_options
{
public:
	void UpdateOptions();
	void UpdateOptionsMap();
	void UpdateOptionsTargetRing();
	void UpdateOptionsNameplate();
	void UpdateOptionsCamera();
	void UpdateOptionsGeneral();
	void UpdateOptionsFloatingDamage();
	void SaveColors() const;
	void LoadColors();
	DWORD GetColor(int index) const;
	ui_options(class ZealService* zeal, class IO_ini* ini, class ui_manager* mgr);
	~ui_options();

	ZealSetting<bool> setting_enable_container_lock = { false, "Zeal", "EnableContainerLock", false };

private:
	void InitUI();
	void InitColors();
	void InitGeneral();
	void InitMap();
	void InitCamera();
	void InitTargetRing();
	void InitNameplate();
	void InitFloatingDamage();
	void UpdateDynamicUI();
	void CleanUI();
	void CleanDynamicUI();
	void RenderUI();
	void Deactivate();
	int FindComboIndex(std::string combobox, std::string text_value);
	void UpdateComboBox(const std::string& name, const std::string& label,
		const std::string& default_label);

	Zeal::EqUI::EQWND* wnd = nullptr;
	std::unordered_map<int, Zeal::EqUI::BasicWnd*> color_buttons;
	ui_manager* const ui;
};
