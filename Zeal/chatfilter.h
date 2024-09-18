#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class chatfilter
{
  public:
	chatfilter(class ZealService* pHookWrapper, class IO_ini* ini);
	int extendedChannelMaps[2];
	void LoadSettings();
	~chatfilter();
};
