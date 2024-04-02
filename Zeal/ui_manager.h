#pragma once
#include "ui_options.h"
#include "ui_bank.h"
#include "ui_loot.h"
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"

class ui_manager
{
public:
	ui_manager(class ZealService* zeal, class IO_ini* ini);
	std::shared_ptr<ui_options> options = nullptr;
	std::shared_ptr<ui_bank> bank = nullptr;
	std::shared_ptr<ui_loot> loot = nullptr;
};

