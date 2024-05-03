#pragma once
#include "ui_options.h"
#include "ui_bank.h"
#include "ui_loot.h"
#include "ui_guild.h"
#include "ui_raid.h"
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_manager
{
public:

	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*)>> checkbox_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> checkbox_names;
	std::unordered_map<Zeal::EqUI::SliderWnd*, std::function<void(Zeal::EqUI::SliderWnd*, int)>> slider_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::SliderWnd*> slider_names;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*, int)>> combo_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> combo_names;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> label_names;
	void SetLabelValue(std::string name, const char* format, ...);
	void SetSliderValue(std::string name, int value);
	void SetSliderValue(std::string name, float value);
	void SetComboValue(std::string name, int value);
	void SetChecked(std::string name, bool checked);
	void AddCheckboxCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback);
	void AddSliderCallback(std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback);
	void AddComboCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback);
	void AddLabel(std::string name);

	ui_manager(class ZealService* zeal, class IO_ini* ini);

	std::shared_ptr<ui_options> options = nullptr;
	std::shared_ptr<ui_bank> bank = nullptr;
	std::shared_ptr<ui_loot> loot = nullptr;
	std::shared_ptr<ui_guild> guild = nullptr;
	std::shared_ptr<ui_raid> raid = nullptr;
private:
	void clean_ui();
	void init_ui();
};

