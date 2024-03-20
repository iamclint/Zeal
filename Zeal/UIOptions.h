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
	void AddCheckboxCallback(std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback); \
	void SetChecked(std::string name, bool checked);
	UIOptions(class ZealService* pHookWrapper, class IO_ini* ini);
	~UIOptions();
private:
	void LoadSettings(class IO_ini* ini);
};
