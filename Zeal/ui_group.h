#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class ui_group
{
public:
	ui_group(class ZealService* zeal, class UIManager* mgr);
	~ui_group();
	void sort();
	void swap(UINT index1, UINT index2);
private:
	void InitUI();
	UIManager* ui;
};

