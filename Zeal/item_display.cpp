#include "item_display.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include "hook_wrapper.h"
#include "memory.h"
#include <algorithm>
#include "string_util.h"
#include <format>
#include <regex>

void ItemDisplay::InitUI()
{
	if (!windows.empty())
		Zeal::EqGame::print_chat("Warning: InitUI and CleanUI out of sync in ItemDisplay");
	
	windows.clear();
	for (int i = 0; i < max_item_displays; i++)
	{
		Zeal::EqUI::ItemDisplayWnd* new_wnd = Zeal::EqUI::ItemDisplayWnd::Create(0);
		if (!new_wnd) {
			Zeal::EqGame::print_chat("Error: Memory allocation failed in ItemDisplay");
			break;
		}
		windows.push_back(new_wnd);
		// new_wnd->SetupCustomVTable();  // Re-enable this and the deleter if custom vtables are required.

		// For an unclear reason the constructor above is not configuring the window relationships
		// correctly, which causes the ItemDescription window to pop above the IconBtn when clicked.
		new_wnd->HasChildren = true;  // Both flags seem to need to be set even if only one is true.
		new_wnd->HasSiblings = true;
		new_wnd->FirstChildWnd = new_wnd->ItemDescription;
		new_wnd->ItemDescription->NextSiblingWnd = new_wnd->IconBtn;
		new_wnd->ItemDescription->HasChildren = true;
		new_wnd->ItemDescription->HasSiblings = true;
		new_wnd->IconBtn->HasChildren = true;
		new_wnd->IconBtn->HasSiblings = true;

		// Set up independent ini settings for these new windows and reload using the new name.
		new_wnd->EnableINIStorage = 0x19;  // Magic value to use the INIStorageName.
		new_wnd->INIStorageName.Set(std::format("ZealItemDisplay{}", i));
		auto vtable = static_cast<Zeal::EqUI::ItemDisplayVTable*>(new_wnd->vtbl);
		auto load_ini = reinterpret_cast<void(__fastcall*)(Zeal::EqUI::ItemDisplayWnd*, int unused)>(vtable->LoadIniInfo);
		load_ini(new_wnd, 0);
	}
}

// Returns a window to display the item or spell (use nullptr) in.
Zeal::EqUI::ItemDisplayWnd* ItemDisplay::get_available_window(Zeal::EqStructures::EQITEMINFOBASE* item)
{
	if (item)
	{
		/*check if the item is already being displayed*/
		for (auto& w : windows)
		{
			if (w->IsVisible && w->ItemValid && w->Item.ID == item->ID)
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

bool ItemDisplay::close_latest_window()
{
	// We don't keep track of the latest, so just close starting from the end of the list.
	for (auto rit = windows.rbegin(); rit != windows.rend(); ++rit) {
		Zeal::EqUI::ItemDisplayWnd* wnd = *rit;
		if (wnd->IsVisible)
		{
			wnd->Deactivate();
			return true;
		}
	}
	return false;
}

static std::string CopperToAll(unsigned long long copper) {
	unsigned long long platinum = copper / 1000;
	unsigned long long gold = (copper % 1000) / 100;
	unsigned long long silver = (copper % 100) / 10;
	unsigned long long remainingCopper = copper % 10;

	std::ostringstream result;
	result << platinum << "p " << gold << "g " << silver << "s " << remainingCopper << "c";

	return result.str();
}

static std::string GetSpellClassLevels(const Zeal::EqStructures::_EQITEMINFO& item, const std::string& original) {
	if (item.Type != 0 || item.Common.Skill != 0x14)
		return original;

	const auto* spell_mgr = Zeal::EqGame::get_spell_mgr();
	int spell_id = item.Common.SpellIdEx;
	if (spell_id < 1 || spell_id >= EQ_NUM_SPELLS || !spell_mgr)
		return original;

	const auto* spell = spell_mgr->Spells[spell_id];
	if (!spell)
		return original;

	// Cycle through the classes adding levels.
	std::string result("Class: ");
	for (int i = Zeal::EqEnums::ClassTypes::Warrior; i <= Zeal::EqEnums::ClassTypes::Beastlord; ++i)
	{
		unsigned int class_bit = 1 << (i - 1);
		if ((item.Common.Classes & class_bit) == 0)
			continue;
		int class_level = spell->ClassLevel[i];
		auto class_name = Zeal::EqGame::class_name_short(i);
		std::transform(class_name.begin(), class_name.end(), class_name.begin(), ::toupper);
		result += std::format(" {} ({})", class_name, class_level);
	}

	return result;
}


static void ApplySpellInfo(Zeal::EqStructures::_EQITEMINFO* item, std::string& s)
{
	if (item->Type == 0 && item->Common.Skill != 0x14 && item->Common.IsStackable > 1 && item->Common.IsStackable < 5)
	{
		// Items with Clicky/Proc/Worn Spells
		if (item->Common.IsStackableEx >= Zeal::EqEnums::ItemEffectCombatProc && item->Common.IsStackableEx <= Zeal::EqEnums::ItemEffectCanEquipClick
			&& item->Common.SpellIdEx > 0 && item->Common.SpellIdEx < 4000)
		{
			int effect_level_req = Zeal::EqGame::get_effect_required_level(item);

			if (item->Common.IsStackableEx == Zeal::EqEnums::ItemEffectCombatProc && s.find(" (Combat)") != std::string::npos)
			{
				// Combat Effect: Spell Name (Level 10)
				s = std::string("Combat ") + s.substr(0, s.find(" (Combat)"));
				if (effect_level_req > 1)
				{
					s += std::format(" (Level {})", effect_level_req);
				}
			}
			else if (item->Common.IsStackableEx == Zeal::EqEnums::ItemEffectWorn && s.find("Effect: Haste (Worn)") != std::string::npos)
			{
				s = std::format("Effect: Haste: +{}%", item->Common.CastingLevel + 1);
			}
			else if (item->Common.IsStackableEx == Zeal::EqEnums::ItemEffectClick && s.find("Casting Time:") != std::string::npos)
			{
				if (effect_level_req > 1)
				{
					std::string effect_level_str = std::string("Level: ") + std::to_string(effect_level_req) + std::string(". ");
					s = s.insert(s.find("Casting Time:"), effect_level_str);
				}
			}
			else if (item->Common.IsStackableEx == Zeal::EqEnums::ItemEffectMustEquipClick && s.find("(Must Equip") != std::string::npos)
			{
				if (effect_level_req > 1)
				{
					std::string effect_level_str = std::string("Level: ") + std::to_string(effect_level_req) + std::string(". ");
					s = s.insert(s.find("(Must Equip") + 1, effect_level_str);
				}
			}
			else if (item->Common.IsStackableEx == Zeal::EqEnums::ItemEffectCanEquipClick && s.find("Casting Time:") != std::string::npos)
			{
				if (effect_level_req > 1)
				{
					std::string effect_level_str = std::string("Level: ") + std::to_string(effect_level_req) + std::string(". ");
					s = s.insert(s.find("Casting Time:"), effect_level_str);
				}
				s = s.insert(s.find("Casting Time:"), "Can Equip. ");
			}
		}
	}
	if (item->Type == 0 && item->Common.Skill == 0x14 && s.starts_with("Class: "))
	{
		s = GetSpellClassLevels(*item, s);
	}
}
static void ApplyInstrumentModifiers(Zeal::EqStructures::_EQITEMINFO* item, std::string& s)
{
	if (item->Type == 0 && item->Common.BardType != 0 && item->Common.BardValue > 10 &&
		s.ends_with("Instruments") || s.ends_with("Instrument Types"))
	{
		int modifier = (item->Common.BardValue - 10) * 10;  // 18 = +80%, 24 = +140%
		//s += (std::string(": ") + std::to_string(modifier) + std::string("%"));
		s += std::format(": {}%", modifier);
	}
}
static void ApplyWeaponRatio(Zeal::EqStructures::_EQITEMINFO* item, std::string& s)
{

	if (item->Common.Damage <= 0 || item->Common.AttackDelay <= 0)
		return;

	size_t pos = s.find("DMG: ");
	if (pos != std::string::npos)
	{
		pos = s.find(" ", pos + strlen("DMG: "));  // Find next space (if any) after dmg.
		pos = (pos == std::string::npos) ? s.length() : pos;
		float ratio = (float)item->Common.Damage / (float)item->Common.AttackDelay;
		s = s.insert(pos, std::format(" ({:.3f})", ratio));
	}
	
}

// Cache of all items displayed so we can access them later if needed.
void ItemDisplay::add_to_cache(const Zeal::EqStructures::EQITEMINFO* item)
{
	// EQITEMINFO is a union of different structs with type 0 (common) the largest, so
	// we only cache the common type to ensure the copy below is valid.
	if (item && item->Type == 0)
		item_cache[item->ID] = *item;
}

const Zeal::EqStructures::EQITEMINFO* ItemDisplay::get_cached_item(int item_id) const
{
	auto it = item_cache.find(item_id);
	if (it != item_cache.end())
		return &(it->second);
	return nullptr;
}

// Generate our customized item description text.
static void UpdateSetItemText(Zeal::EqUI::ItemDisplayWnd* wnd, Zeal::EqStructures::_EQITEMINFO* item) {

	if (!item || wnd->DisplayText.Data == nullptr)
		return;

	// Split the existing text into separate lines, release it, and then update line by line.
	const std::string stml_line_break = "<BR>";
	auto strings = Zeal::String::split_text(std::string(wnd->DisplayText), stml_line_break);
	wnd->DisplayText.Set("");
	for (auto& s : strings) {
		// Perform partial iteminfo filtering in combination with substrings.
		ApplySpellInfo(item, s);
		ApplyInstrumentModifiers(item, s);
		ApplyWeaponRatio(item, s);
		s += stml_line_break;
		wnd->DisplayText.Append(s.c_str());
	}
	if (item->NoDrop != 0)
		wnd->DisplayText.Append("Value: " + CopperToAll(item->Cost) + stml_line_break);
}

void __fastcall SetItem(Zeal::EqUI::ItemDisplayWnd* wnd, int unused,
	Zeal::EqStructures::_EQITEMINFO* item, bool show)
{
	ZealService::get_instance()->hooks->hook_map["SetItem"]->original(SetItem)(wnd, unused, item, show);

	if (ZealService::get_instance() && ZealService::get_instance()->item_displays)
		ZealService::get_instance()->item_displays->add_to_cache(item);

	// The callers of SetItem always call Activate() on it immediately after which includes
	// a call to update the window text, so we just need to update the DisplayText here.
	UpdateSetItemText(wnd, item);
}

static std::string get_target_type_string(int target_type)
{
	const char* type = nullptr;
	switch (target_type)
	{
	case 3:
	case 41:  // Bard songs at least.
		type = "Group";
		break;
	case 4:
		type = "Area of effect (Point blank)";
		break;
	case 5:
		type = "Single";
		break;
	case 6:
		type = "Self";
		break;
	case 8:
		type = "Area of effect (Targeted)";
		break;
	case 9:
		type = "Animal";
		break;
	case 10:
		type = "Undead";
		break;
	case 11:
		type = "Summoned";
		break;
	case 14:
		type = "Pet";
		break;
	case 16:
		type = "Plant";
		break;
	default:
		break;
	}
	if (!type)
		return std::format("Target: Unknown ({})", target_type);
	return std::string("Target: ") + std::string(type);
}

static void UpdateSetSpellText(Zeal::EqUI::ItemDisplayWnd* wnd, int spell_id)
{
	auto* spell_mgr = Zeal::EqGame::get_spell_mgr();
	if (!spell_mgr || spell_id < 1 || spell_id >= EQ_NUM_SPELLS || wnd->DisplayText.Data == nullptr)
		return;

	const auto* sp_data = Zeal::EqGame::get_spell_mgr()->Spells[spell_id];
	if (!sp_data)
		return;

	const std::string stml_line_break = "<BR>";
	wnd->DisplayText.Append(get_target_type_string(sp_data->TargetType) + stml_line_break);

	if (sp_data->Resist >= 0 && sp_data->Resist < 6)
	{
		const char* resist_lut[6] = {"Unresistable", "Magic", "Fire", "Cold", "Poison", "Disease"}; // No chromatic yet
		std::string resist_type(resist_lut[sp_data->Resist]);
		// std::string resist_adj = std::format(" ({:+d})", sp_data->ResistAdj);  // Unexpected values.
		wnd->DisplayText.Append("Resist: " + resist_type + stml_line_break);
	}
	if (sp_data->SpellType == 0)
		wnd->DisplayText.Append(std::string("Detrimental") + stml_line_break);
	if (sp_data->Location > 0 && sp_data->Location < 3)
		wnd->DisplayText.Append(std::string("Location: ") +
			std::string((sp_data->Location == 1) ? "Outdoors" : "Dungeons") + stml_line_break);
}

void __fastcall SetSpell(Zeal::EqUI::ItemDisplayWnd* wnd, int unused, int spell_id, bool show, int unknown)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->hooks->hook_map["SetSpell"]->original(SetSpell)(wnd, unused, spell_id, show, unknown);
	UpdateSetSpellText(wnd, spell_id);
}

void ItemDisplay::CleanUI()
{
	for (auto& w : windows)
	{
		if (w)
		{
			if (w->IsVisible)  // Should never happen.
				w->Deactivate();

			if (reinterpret_cast<uint32_t>(w->vtbl) != Zeal::EqUI::ItemDisplayWnd::kDefaultVTableAddr)
				w->DeleteCustomVTable(); 
			w->Destroy();
		}
	}
	windows.clear();
}

void ItemDisplay::DeactivateUI()
{
	for (auto& w : windows)
	{
		if (w && w->IsVisible)
			w->Deactivate();  // Calls show(0) and clears IsActivated.
	}
}

// Response handler for OP_LinkRequest that calls SetItem and Activate().
void __cdecl msg_request_inspect_item(Zeal::EqStructures::_EQITEMINFO* item)
{
	auto* default_item_display_wnd = Zeal::EqGame::Windows->ItemWnd;  // Cache the default.
	Zeal::EqGame::Windows->ItemWnd = ZealService::get_instance()->item_displays->get_available_window(item);
	if (Zeal::EqGame::Windows->ItemWnd->IsVisible)
		Zeal::EqGame::Windows->ItemWnd->Deactivate();  // Avoid double activation.

	ZealService::get_instance()->hooks->hook_map["msg_request_inspect_item"]->original(msg_request_inspect_item)(item);
	Zeal::EqGame::Windows->ItemWnd = default_item_display_wnd;  // Restore.
}

// Replaces the default vtable callback to allow temporarily replacing the global pointer.
static int __fastcall InvSlotWnd_HandleLButtonUp(Zeal::EqUI::InvSlotWnd* wnd, int unused_edx, int mouse_x, int mouse_y, unsigned int flags)
{
	// If there is an item, modify the ItemWnd global pointer to point to one of our windows.
	auto* default_item_display_wnd = Zeal::EqGame::Windows->ItemWnd;
	if (wnd->IsActive && wnd->invSlot && wnd->invSlot->Item)
		Zeal::EqGame::Windows->ItemWnd = ZealService::get_instance()->item_displays->get_available_window(wnd->invSlot->Item);

	if (Zeal::EqGame::Windows->ItemWnd->IsVisible)
		Zeal::EqGame::Windows->ItemWnd->Deactivate();  // Avoid double activation.

	int result = wnd->HandleLButtonUp(mouse_x, mouse_y, flags);

	Zeal::EqGame::Windows->ItemWnd = default_item_display_wnd;
	return result;
}

static int __fastcall CastSpellWnd_WndNotification(Zeal::EqUI::CastSpellWnd* wnd, int unused_edx, Zeal::EqUI::BasicWnd* src_wnd, int param_2, void* param_3) {
	// Forward all messages except for left mouse click messages with the alt key depressed.
	if (param_2 != 1 || !Zeal::EqGame::get_wnd_manager() || !Zeal::EqGame::get_wnd_manager()->AltKeyState)
		return wnd->WndNotification(src_wnd, param_2, param_3);

	// Temporarily modify the ItemWnd global pointer to point to one of our windows.
	auto* default_item_display_wnd = Zeal::EqGame::Windows->ItemWnd;
	Zeal::EqGame::Windows->ItemWnd = ZealService::get_instance()->item_displays->get_available_window();

	// The HandleSpellInfoDisplay() will toggle off a visible window, so deactivate it if needed.
	if (Zeal::EqGame::Windows->ItemWnd->IsVisible)
		Zeal::EqGame::Windows->ItemWnd->Deactivate();

	// Invoke CCastSpellWnd::HandleSpellInfoDisplay() which calls SetSpell() and activates.
	wnd->HandleSpellInfoDisplay(src_wnd);

	Zeal::EqGame::Windows->ItemWnd = default_item_display_wnd;
	return 0;
}

static int __fastcall SpellBookWnd_WndNotification(Zeal::EqUI::SpellBookWnd* wnd, int unused_edx, Zeal::EqUI::BasicWnd* src_wnd, int param_2, void* param_3) {
	// Forward all messages except for left mouse click messages with the alt key depressed.
	if (param_2 != 1 || !Zeal::EqGame::get_wnd_manager() || !Zeal::EqGame::get_wnd_manager()->AltKeyState)
		return wnd->WndNotification(src_wnd, param_2, param_3);

	// Temporarily modify the ItemWnd global pointer to point to one of our windows.
	auto* default_item_display_wnd = Zeal::EqGame::Windows->ItemWnd;
	Zeal::EqGame::Windows->ItemWnd = ZealService::get_instance()->item_displays->get_available_window();

	// The DisplaySpellInfo() will toggle off a visible window, so deactivate it if needed.
	if (Zeal::EqGame::Windows->ItemWnd->IsVisible)
		Zeal::EqGame::Windows->ItemWnd->Deactivate();

	// Invoke CSpellBookWnd::DisplaySpellInfo() which calls SetSpell() and activates.
	wnd->DisplaySpellInfo(src_wnd);

	Zeal::EqGame::Windows->ItemWnd = default_item_display_wnd;
	return 0;
}

ItemDisplay::ItemDisplay(ZealService* zeal, IO_ini* ini)
{
	windows.clear();
	zeal->hooks->Add("SetItem", 0x423640, SetItem, hook_type_detour);    // CItemDisplayWnd::SetItem
	zeal->hooks->Add("SetSpell", 0x425957, SetSpell, hook_type_detour);  // CItemDisplayWnd::SetSpell
	zeal->hooks->Add("msg_request_inspect_item", 0x004e81c6, msg_request_inspect_item, hook_type_detour);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { DeactivateUI(); }, callback_type::DeactivateUI);

	// Modify the Alt + Left Mouse click SetItem() related callback of CInvSlotWnd.
	auto* inv_slot_wnd_vtable = Zeal::EqUI::InvSlotWnd::default_vtable;
	mem::unprotect_memory(inv_slot_wnd_vtable, sizeof(*inv_slot_wnd_vtable));
	inv_slot_wnd_vtable->HandleLButtonUp = InvSlotWnd_HandleLButtonUp;
	mem::reset_memory_protection(inv_slot_wnd_vtable);

	// Modify the Alt + Left Mouse click SetItem() related callback of CastSpellWnd.
	auto* spell_gems_wnd_vtable = Zeal::EqUI::CastSpellWnd::default_vtable;
	mem::unprotect_memory(spell_gems_wnd_vtable, sizeof(*spell_gems_wnd_vtable));
	spell_gems_wnd_vtable->WndNotification = CastSpellWnd_WndNotification;
	mem::reset_memory_protection(spell_gems_wnd_vtable);

	// Modify the Alt + Left Mouse click SetItem() related callback of SpellBookWnd.
	auto* spell_book_wnd_vtable = Zeal::EqUI::SpellBookWnd::default_vtable;
	mem::unprotect_memory(spell_book_wnd_vtable, sizeof(*spell_book_wnd_vtable));
	spell_book_wnd_vtable->WndNotification = SpellBookWnd_WndNotification;
	mem::reset_memory_protection(spell_book_wnd_vtable);

	// Not bothering to modify these windows (Retain default behavior using the default ItemDisplayWnd).
	// CBuffWindow: Alt + Left click toggles persistent one at a time, Right click is temporary.
	// CHotButtonWnd: Right click does a temporary pop-up.
}

ItemDisplay::~ItemDisplay()
{
}

// Notes:
//  - Besides CInvSlotWnd, the global ItemDisplayWnd is accessed in:
//   - CBuffWindow::HandleSpellInfoDisplay: InvSlotWnd sends 0x17 or 0x19 to WndNotification (probably)
//   - CCastSpellWnd::WndNotification responding to Alt+1 or 0x17 and 0x19. CSpellGemWnd::HandleLButtonUp
//   - CCastSpellWnd::HandleSpellInfoDisplay called by WndNotification
//   - CDisplay::NewUIProcessEscape
//   - CHotButtonWnd::WndNotification: Responding to 0x17 or 0x19.
//   - CSpellBookWnd::DisplaySpellInfo: Through WndNotification of Alt+1, 0x17, or 0x19.

// Deprecated patches:
// 
// Setting spell was toggling the item display window unlike items. These disable that.
// mem::write<BYTE>(0x4090AB, 0xEB);
// mem::write<BYTE>(0x40a4c4, 0xEB);

// There are three separate calls to SetItem() in the client and each of those feed in the
// Zeal::EqGame::Windows->ItemWnd to both SetItem() and an Activate() immediately after.
// The patches below disable the Activate() call afterwards.
// mem::set(0x421EBF, 0x90, 14); // Disables in CInvSlotWnd::HandleLButtonUp.
// mem::set(0x4229BE, 0x90, 14); // Disables in CInvSlot::HandleRButtonHeld().
// mem::set(0x4e81e0, 0x90, 14); // Disables in OP_ItemLinkResponse message from the server.

