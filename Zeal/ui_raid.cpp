#include "ui_raid.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

void ui_raid::CleanUI()
{
	Zeal::EqGame::print_debug("Clean UI RAID");
}

void ui_raid::InitUI()
{

}

ui_raid::ui_raid(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	//if (Zeal::EqGame::is_in_game()) InitUI();
}
ui_raid::~ui_raid()
{
}
