#include "equip_item.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include "hook_wrapper.h"
#include <algorithm>
#include <vector>

static void __fastcall CInvSlot_HandleRButtonUp(Zeal::EqUI::InvSlot* inv_slot, int unused_edx, int x, int y)
{
	if (ZealService::get_instance()->equip_item_hook->HandleRButtonUp(inv_slot)) {
		return;
	}
	ZealService::get_instance()->hooks->hook_map["CInvSlot_HandleRButtonUp"]->original(CInvSlot_HandleRButtonUp)(inv_slot, unused_edx, x, y);
}

bool EquipItem::HandleRButtonUp(Zeal::EqUI::InvSlot* src_inv_slot)
{
	if (!Enabled.get() || !src_inv_slot) {
		return false;
	}

	Zeal::EqUI::InvSlotWnd* src_wnd = src_inv_slot->invSlotWnd;
	if (!src_wnd) {
		return false;
	}

	// Slot ID for bagged items is 250 + (bag_i*10) + (contents_i) = [250...329]
	int src_slot_id = src_wnd->SlotID;
	if (src_slot_id < 250 || src_slot_id > 329) {
		return false; // Item is not in an inventory bag.
	}

	int src_container_i = (src_slot_id - 250) / 10;
	if (src_container_i < 0 || src_container_i > 7) {
		return false; // Shouldn't happen. Ensure bag is 0..7
	}

	Zeal::EqUI::CXWndManager* wnd_mgr = Zeal::EqGame::get_wnd_manager();
	if (!wnd_mgr) {
		return false;
	}

	Zeal::EqStructures::EQCHARINFO* c = Zeal::EqGame::get_char_info();
	if (!c || c->CursorItem || c->CursorCopper || c->CursorGold || c->CursorPlatinum || c->CursorSilver) {
		return false; // Fast-fail. We are holding something.
	}

	Zeal::EqStructures::EQITEMINFO* src_container = c->InventoryPackItem[src_container_i];
	if (!src_container || src_container->Type != 1) {
		return false; // Can't locate source bag info.
	}

	Zeal::EqStructures::EQITEMINFO* src_item = (Zeal::EqStructures::EQITEMINFO*)src_inv_slot->Item;
	if (src_item->Type != 0) {
		return false; // Item is not a common item.
	}

	bool can_class_race_equip = Zeal::EqGame::can_use_item(c, src_item);
	if (!can_class_race_equip) {
		return false; // Item is not usable by our race/class/deity
	}

	// ------------------------------------------
	// -- Find what slots will accept the item --
	// ------------------------------------------

	WORD src_item_id = src_item->ID;
	std::vector<std::pair<int, WORD>> dst_slots; // Pairs of {invSlot,itemID} for the valid destination slots

	// First look for empty slots, prioritize those.
	for (int i : EQUIP_PRIORITY_ORDER) {
		if (!c->InventoryItem[i] && Zeal::EqGame::can_item_equip_in_slot(c, src_item, i + 1)) {
			dst_slots.push_back(std::make_pair<int, WORD>(i + 1, 0));
		}
	}

	// Looks for any valid slot if no empty ones were available.
	if (dst_slots.empty()) {
		for (int i : EQUIP_PRIORITY_ORDER) {
			if (c->InventoryItem[i] && Zeal::EqGame::can_item_equip_in_slot(c, src_item, i + 1)) {
				if (c->InventoryItem[i]->Type != 0) {
					continue; // Equipped item isn't a normal item? Skip it.
				}
				if (c->InventoryItem[i]->ID == src_item_id) {
					if (src_item->Common.IsStackable) {
						if (c->InventoryItem[i]->Common.StackCount >= 20) {
							continue; // Equipepd item is already a full stack of the same item. Skip it.
						}
					}
					else if (src_item->Common.Charges == c->InventoryItem[i]->Common.Charges) {
						continue; // Equiped item is the same item. Skip it.
					}
				}
				else if (src_container->Container.SizeCapacity < c->InventoryItem[i]->Size) {
					continue; // Equipped item won't fit into this bag when we swap. Skip it.
				}
				dst_slots.push_back(std::make_pair(i + 1, c->InventoryItem[i]->ID));
			}
		}
		if (dst_slots.empty()) {
			return false; // Couldn't find any slot to equip the item in.
		}
	}

	// -----------------------------------------
	// -- Preparing to Swap, Picking the Slot --
	// -----------------------------------------

	// Get the currently held keyboard modifiers
	BYTE shift = wnd_mgr->ShiftKeyState;
	BYTE ctrl = wnd_mgr->ControlKeyState;
	BYTE alt = wnd_mgr->AltKeyState;

	// User controlled behavior:
	// Based on what keys are held (nothing, Shift, Ctrl, Shift+Ctrl), use the 1st, 2nd, 3rd, or 4th equippable slot, respectively
	int i = (shift ? 1 : 0) + (ctrl ? 2 : 0);
	if (i >= dst_slots.size()) {
		i = dst_slots.size() - 1;
	}

	int dst_inv_slot_id = dst_slots[i].first;
	WORD dst_item_id = dst_slots[i].second;
	Zeal::EqUI::InvSlot* dst_inv_slot = GetInventorySlot(dst_inv_slot_id);
	if (!dst_inv_slot) {
		return false; // Destination slot not found.
	}

	// --------------------
	// -- Begin Swapping --
	// --------------------

	// Pre-swap:
	// - Force the keyboard modifiers 'SHIFT'.
	// - This ensure we pick up stacked items when sending LeftClick events.
	wnd_mgr->ShiftKeyState = 1;
	wnd_mgr->ControlKeyState = 0;
	wnd_mgr->AltKeyState = 0;

	// (1) Pickup the bagged item.
	src_inv_slot->HandleLButtonUp();
	// Now we should be holding the item to swap on our Cursor
	if (c->CursorItem == src_item) {
		// (2) Equip it in the destination slot.
		dst_inv_slot->HandleLButtonUp();
		// Now we should be holding the swapped-out item instead (if there was one equipped).
		if (dst_item_id && c->CursorItem && c->CursorItem->ID == dst_item_id) {
			// (3) Put the swapped-out item in the bag slot we just emptied.
			src_inv_slot->HandleLButtonUp();
		}
	}

	// Done. Restore the keyboard flags.
	wnd_mgr->ShiftKeyState = shift;
	wnd_mgr->ControlKeyState = ctrl;
	wnd_mgr->AltKeyState = alt;
	return true;
}

Zeal::EqUI::InvSlot* EquipItem::GetInventorySlot(int inv_slot_id) {
	if (inv_slot_id < 1 || inv_slot_id > 22) {
		return nullptr; // Not an equippable inventory slot
	}
	Zeal::EqUI::CInvSlotMgr* inv_slot_mgr = Zeal::EqGame::Windows->InvSlotMgr;
	if (!inv_slot_mgr) {
		return nullptr;
	}
	Zeal::EqUI::InvSlot* inv_slot = inv_slot_mgr->FindInvSlot(inv_slot_id);
	if (!inv_slot) {
		return nullptr; // Slot Unavailable
	}
	Zeal::EqUI::InvSlotWnd* wnd = inv_slot->invSlotWnd;
	if (!wnd || wnd->SlotID != inv_slot_id) {
		return nullptr; // Invalid window
	}
	return inv_slot;
}

EquipItem::EquipItem(ZealService* zeal)
{
	if (!Zeal::EqGame::is_new_ui()) {
		return;
	}
	ZealService::get_instance()->hooks->Add("CInvSlot_HandleRButtonUp", 0x422804, CInvSlot_HandleRButtonUp, hook_type_detour);
}

EquipItem::~EquipItem()
{
}

