#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

struct CustomFilter {
	std::string name; //String name - Appears in the Menu
	int channelMap;   //Extended Channel Map ID - Zeal developer set
	short colorID;    //ColorID - Will redirect prints with this ColorID
	int windowHandle; //Window Handle - Maintains the currently filtered Chat Window handle
};

class chatfilter
{
  public:
	chatfilter(class ZealService* pHookWrapper, class IO_ini* ini);
	std::vector<CustomFilter> Extended_ChannelMaps;
	void LoadSettings();	
	bool isExtendedCM(int channelMap, int applyOffset = 0);
	bool isStandardCM(int channelMap, int applyOffset = 0);
	~chatfilter();
};
