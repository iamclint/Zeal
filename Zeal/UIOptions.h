#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class UIOptions
{
public:
	void InitUI();
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*)>> checkbox_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> checkbox_names;
	std::unordered_map<Zeal::EqUI::SliderWnd*, std::function<void(Zeal::EqUI::SliderWnd*, int)>> slider_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::SliderWnd*> slider_names;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*)>> combo_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> combo_names;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> label_names;
	void SetSliderValue(std::string name, int value);
	void AddCheckboxCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback);
	void AddSliderCallback(std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback);
	void AddComboCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback);
	void AddLabel(std::string name);
	void SetChecked(std::string name, bool checked);
	UIOptions(class ZealService* pHookWrapper, class IO_ini* ini);
	~UIOptions();
private:
	void LoadSettings(class IO_ini* ini);
};
