#include "ui_manager.h"

ui_manager::ui_manager(ZealService* zeal, IO_ini* ini)
{
	bank = std::make_shared<ui_bank>(zeal, ini);
	options = std::make_shared<ui_options>(zeal, ini);
	loot = std::make_shared<ui_loot>(zeal, ini);
}