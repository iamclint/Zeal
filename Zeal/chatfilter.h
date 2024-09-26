#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include <functional>

struct CustomFilter {
	std::string name; //String name - Appears in the Menu
	int channelMap;   //Extended Channel Map ID - Zeal developer set
	Zeal::EqUI::ChatWnd* windowHandle; //Window Handle - Maintains the currently filtered Chat Window handle
	std::function<bool(short, std::string)> isHandled;
    // Default Constructor
    CustomFilter()
        : name(""), channelMap(0), windowHandle(nullptr), isHandled(nullptr) {
        // Optionally, add default lambda for isHandled
    }

    CustomFilter(const std::string& name, int channelMap, std::function<bool(short, std::string)> isHandled)
        : name(name), channelMap(channelMap), isHandled(isHandled) {
    }

    ~CustomFilter() {  }
};

struct damage_data
{
	Zeal::EqStructures::Entity* source = nullptr;
	Zeal::EqStructures::Entity* target = nullptr;
	WORD type = 0;
	short spell_id = 0; 
	short damage = 0; 
	int heal = 0;
};

class chatfilter
{
  public:
	chatfilter(class ZealService* pHookWrapper, class IO_ini* ini);
	std::vector<CustomFilter> Extended_ChannelMaps;
	void AddOutputText(Zeal::EqUI::ChatWnd*& wnd, std::string msg, BYTE channel);
	void LoadSettings(Zeal::EqUI::CChatManager* cm);
	bool isExtendedCM(int channelMap, int applyOffset = 0);
	bool isStandardCM(int channelMap, int applyOffset = 0);
	bool isDamage=false;
	damage_data damageData;
	~chatfilter();
};
