#pragma once
#include "ui_options.h"
#include "ui_bank.h"
#include "ui_loot.h"
#include "ui_guild.h"
#include "ui_raid.h"
#include "ui_hotbutton.h"
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

	std::unordered_map<Zeal::EqUI::BasicWnd*, std::unordered_map<std::string, Zeal::EqUI::BasicWnd*>> WindowChildren;

	Zeal::EqUI::BasicWnd* GetChild(Zeal::EqUI::BasicWnd* parent, std::string name);
	void SetLabelValue(std::string name, const char* format, ...);
	void SetSliderValue(std::string name, int value);
	void SetSliderValue(std::string name, float value);
	void SetComboValue(std::string name, int value);
	void SetChecked(std::string name, bool checked);
	void AddCheckboxCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback);
	void AddSliderCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback);
	void AddComboCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback);
	void AddLabel(Zeal::EqUI::BasicWnd* wnd, std::string name);
	void AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::vector<std::string>> data);

	ui_manager(class ZealService* zeal, class IO_ini* ini);

	std::shared_ptr<ui_options> options = nullptr;
	std::shared_ptr<ui_bank> bank = nullptr;
	std::shared_ptr<ui_loot> loot = nullptr;
	std::shared_ptr<ui_guild> guild = nullptr;
	std::shared_ptr<ui_raid> raid = nullptr;
	std::shared_ptr<ui_hotbutton> hotbutton = nullptr;
private:
	void CleanUI();
	void init_ui();
};

