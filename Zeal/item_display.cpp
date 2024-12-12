#include "item_display.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

void ItemDisplay::init_ui()
{
	if (!windows.empty())
		Zeal::EqGame::print_chat("Warning: InitUI and CleanUI out of sync in ItemDisplay");
	windows.clear();

	if (Zeal::EqGame::Windows->ItemWnd)
		windows.push_back(Zeal::EqGame::Windows->ItemWnd);
	else
		Zeal::EqGame::print_chat("Error: Client ItemDisplay is null");

	for (int i = 0; i < max_item_displays; i++)
	{
		Zeal::EqUI::ItemDisplayWnd* new_wnd = Zeal::EqUI::ItemDisplayWnd::Create(0);
		if (!new_wnd) {
			Zeal::EqGame::print_chat("Error: Memory allocation failed in ItemDisplay");
			break;
		}
		windows.push_back(new_wnd);
		// new_wnd->SetupCustomVTable();  // Re-enable this and the deleter if custom vtables are required.
		new_wnd->Location.Top += 20 * (i + 1);
		new_wnd->Location.Left += 20 * (i + 1);
		new_wnd->Location.Bottom += 20 * (i + 1);
		new_wnd->Location.Right += 20 * (i + 1);
	}
}

Zeal::EqUI::ItemDisplayWnd* ItemDisplay::get_available_window(Zeal::EqStructures::_EQITEMINFO* item)
{
	if (item)
	{
		/*check if the item is already being displayed*/
		for (auto& w : windows)
		{
			if (w->Item.ID == item->ID)
				return w;
		}
	}

	for (auto& w : windows)
	{
		if (!w->IsVisible)
			return w;
	}
	return windows.back();
}

std::string CopperToAll(unsigned long long copper) {
	unsigned long long platinum = copper / 1000;
	unsigned long long gold = (copper % 1000) / 100;
	unsigned long long silver = (copper % 100) / 10;
	unsigned long long remainingCopper = copper % 10;

	std::ostringstream result;
	result << platinum << "p " << gold << "g " << silver << "s " << remainingCopper << "c";

	return result.str();
}

void __fastcall SetItem(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, Zeal::EqStructures::_EQITEMINFO* item, bool show)
{
	ZealService* zeal = ZealService::get_instance();
	wnd = zeal->item_displays->get_available_window(item);
	zeal->item_displays->current_item = item;
	zeal->hooks->hook_map["SetItem"]->original(SetItem)(wnd, unused, item, show);
	if (item->NoDrop!=0)
		wnd->DisplayText.Append("Value: " + CopperToAll(item->Cost) + "<BR>");

	// wnd->IconBtn->ZLayer = wnd->ZLayer;  // Disabled since it was causing bank/merchant wnd z-layer issues.
	wnd->Activate();
}
char* build_token_string_PARAM(char* buffer, int stringID, char* string1, char* string2, int u1, int u2, int u3, int u4, int u5, int u6, int u7)
{
	if (strcmp(string1, "Haste") == 0)
	{
		if (strcmp(string2, "(Worn)") == 0)
		{
			ZealService* zeal = ZealService::get_instance();
			Zeal::EqStructures::_EQITEMINFO* item = zeal->item_displays->current_item;
			sprintf_s(buffer, 16, "Haste: %+d%%", item->Common.CastingLevel+1);
			return buffer;
		}
	}
	return ZealService::get_instance()->hooks->hook_map["ModifyHaste"]->original(build_token_string_PARAM)(buffer, stringID, string1, string2, 0, 0, 0, 0, 0, 0, 0);
}

void __fastcall SetSpell(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, int spell_id, bool show, int unknown)
{
	ZealService* zeal = ZealService::get_instance();
	wnd = zeal->item_displays->get_available_window(0);
	zeal->hooks->hook_map["SetSpell"]->original(SetSpell)(wnd, unused, spell_id, show, unknown);
	wnd->IconBtn->ZLayer = wnd->ZLayer;
	wnd->Activate();
}

// Replaces a _mbscpy() call with a simple "copy" that appends the instrument modifier percentage.
static char* ModifyInstrumentString(char* destination, char* source) {
	const int kDestinationSize = 32;  // ASSUMED SAFE BUFFER SIZE
	int modifier = 0;
	ZealService* zeal = ZealService::get_instance();
	if (zeal && zeal->item_displays && zeal->item_displays->current_item)
	{
		const auto& item = *zeal->item_displays->current_item;
		if (item.Common.BardType != 0 && item.Common.BardValue > 10)
			modifier = (item.Common.BardValue - 10) * 10;  // 18 = +80%, 24 = +140%
	}
	if (modifier)
		snprintf(destination, kDestinationSize, "%s: %+d%%", source, modifier);
	else
		snprintf(destination, kDestinationSize, "%s", source);  // Null-terminated copy.
	return destination;
}

void ItemDisplay::CleanUI()
{
	// The first item is the original one registered with the framework which will be cleaned
	// automatically, so skip that and delete the rest.
	for (int i = 1; i < windows.size(); ++i)
	{
		if (windows[i])
		{
			// We assume that deactivate_ui() was called by the framework already (so not needed here).
			// windows[i]->DeleteCustomVTable();  // Re-enable if custom vtables are required.
			windows[i]->Destroy();
		}
	}
	windows.clear();
}

void ItemDisplay::DeactivateUI()
{
	// Skip the first one since framework will handle that.
	for (int i = 1; i < windows.size(); ++i)
	{
		if (windows[i])
			windows[i]->show(0, false);
	}
}


ItemDisplay::ItemDisplay(ZealService* zeal, IO_ini* ini)
{
	windows.clear();
	zeal->hooks->Add("SetItem", 0x423640, SetItem, hook_type_detour);
	zeal->hooks->Add("SetSpell", 0x425957, SetSpell, hook_type_detour);
	zeal->hooks->Add("ModifyInstrumentString", 0x423d0f, ModifyInstrumentString, hook_type_replace_call);
	zeal->hooks->Add("ModifyHaste", 0x424a95, build_token_string_PARAM, hook_type_replace_call);
	zeal->callbacks->AddGeneric([this]() { init_ui(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { DeactivateUI(); }, callback_type::DeactivateUI);

	mem::write<BYTE>(0x4090AB, 0xEB); //for some reason the game when setting spell toggles the item display window unlike with items..this just disables that feature
	mem::write<BYTE>(0x40a4c4, 0xEB); //for some reason the game when setting spell toggles the item display window unlike with items..this just disables that feature
	mem::set(0x421EBF, 0x90, 14); //remove the auto focus of the main item window and handle it ourselves
	mem::set(0x4e81e0, 0x90, 14); //remove the auto focus of the main item window and handle it ourselves
	mem::set(0x4229BE, 0x90, 14); //remove the opening of the window when holding right click (allow zeal to handle it)

}

ItemDisplay::~ItemDisplay()
{
}

