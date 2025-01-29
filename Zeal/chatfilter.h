#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
#include <functional>
#include "ZealSettings.h"

#define CHANNEL_MYPETDMG 1000
#define CHANNEL_OTHERPETDMG 1001
#define CHANNEL_MYPETSAY 1002
#define CHANNEL_OTHERPETSAY 1003

struct CustomFilter {
	std::string name; //String name - Appears in the Menu
	int channelMap;   //Extended Channel Map ID - Zeal developer set
	Zeal::EqUI::ChatWnd* windowHandle; //Window Handle - Maintains the currently filtered Chat Window handle
	std::function<bool(short&, std::string)> isHandled;
    // Default Constructor
    CustomFilter()
        : name(""), channelMap(0), windowHandle(nullptr), isHandled(nullptr) {
        // Optionally, add default lambda for isHandled
    }

    CustomFilter(const std::string& name, int channelMap, std::function<bool(short&, std::string)> isHandled)
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
	Zeal::EqUI::ContextMenu* ZealMenu = nullptr;
	void AddOutputText(Zeal::EqUI::ChatWnd*& wnd, std::string msg, short& channel);
	void LoadSettings(Zeal::EqUI::CChatManager* cm);
	void callback_clean_ui();
	ZealSetting<bool> setting_suppress_missed_notes = { false, "Zeal", "SuppressMissedNotes", false };
	bool isExtendedCM(int channelMap, int applyOffset = 0);
	bool isStandardCM(int channelMap, int applyOffset = 0);
	bool isDamage=false;
	bool isMyPetSay=false;
	bool isPetMessage=false;
	int menuIndex = -1;
	damage_data damageData;
	~chatfilter();
};
