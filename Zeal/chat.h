#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include "ZealSettings.h"

class Chat
{
public:


	ZealSetting<bool> UseClassicClassNames = { false, "Zeal", "ClassicClasses", false, [this](bool val) { set_classes(); }, true };
	ZealSetting<bool> UseBlueCon = { true, "Zeal", "Bluecon", false };
	ZealSetting<bool> UseZealInput = { true, "Zeal", "ZealInput", false };
	ZealSetting<bool> UseUniqueNames = { false, "Zeal", "UniqueNames", false };
	ZealSetting<int> TimeStampsStyle = { 0, "Zeal", "ChatTimestamps", false };

	void set_classes();
	void DoPercentReplacements(std::string& str_data);
	Chat(class ZealService* pHookWrapper);
	~Chat();
private:
	void InitPercentReplacements();
	std::vector<std::function<void(std::string& str_data)>> percent_replacements;
};
