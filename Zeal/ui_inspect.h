#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class ui_inspect
{
public:
	ui_inspect(class ZealService* zeal, class ui_manager* mgr);
	~ui_inspect();
private:
	void InitUI();
	ui_manager* ui;
};

