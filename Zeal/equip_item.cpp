#include "equip_item.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include "hook_wrapper.h"
#include <algorithm>
#include <vector>

static bool ClickInventoryWindowSlot(int invSlot, unsigned int flags = 0) {
	if (!Zeal::EqGame::Windows->Inventory) {
		return false;
	}
	if (invSlot < 1 || invSlot > 22) {
		return false;
	}
	// Note: Don't need to check for visible for Inventory UI. The elements are clickable even when not shown.
	Zeal::EqUI::BasicWnd* btn = Zeal::EqGame::Windows->Inventory->GetChildItem("InvSlot" + std::to_string(invSlot));
	if (btn) {
		Zeal::EqUI::CXPoint point = btn->GetScreenCenterPoint();
		btn->LeftClickDown(point.x, point.y, flags);
		btn->LeftClickUp(point.x, point.y, flags);
		return true;
	}
	return false;
}

// Hooks the RButtonUp on InvSlot UI elements. Attempts to equip the item in the slot if valid.
static int __fastcall InvSlotWnd_HandleRButtonUp(Zeal::EqUI::InvSlotWnd* wnd, int unused_edx, int mouse_x, int mouse_y, unsigned int flags)
{
	// Call-through to the original handler.
	int res = wnd->HandleRButtonUp(mouse_x, mouse_y, flags);

	// Try to do right-click-equip logic
	if (wnd && wnd->invSlot && wnd->invSlot->Item) {

		static int PRIORITY_ORDER[21] = {
			12, 13, 10,           // Primary, Secondary, Range
			16, 17, 1, 6, 11, 18, // Chest, Legs, Head, Arms, Hands, Feet
			5, 7, 4, 2, 19, 8, 9, // Shoulders, Back, Neck, Face, Waist, WristLeft, WristRight,
			0, 3, 14, 15, 20      // EarLeft, EarRight, RingLeft, RingRight, Ammo
		};

		// Slot ID for bagged items is 250 + (bag_i*10) + (contents_i) = [250...329]
		int src_slot_id = wnd->SlotID;
		if (src_slot_id < 250 || src_slot_id > 329) {
			return res; // Item is not in an inventory bag.
		}

		int src_container_i = (src_slot_id - 250) / 10;
		if (src_container_i < 0 || src_container_i > 7) {
			return res; // Shouldn't happen. Ensure bag is 0..7
		}

		Zeal::EqUI::CXWndManager* wnd_mgr = Zeal::EqGame::get_wnd_manager();
		if (!wnd_mgr) {
			return res;
		}

		Zeal::EqStructures::EQCHARINFO* c = Zeal::EqGame::get_char_info();
		if (!c || c->CursorItem || c->CursorCopper || c->CursorGold || c->CursorPlatinum || c->CursorSilver) {
			return res; // Fast-fail. We are holding something.

		}

		Zeal::EqStructures::EQITEMINFO* container = c->InventoryPackItem[src_container_i];
		if (!container || container->Type != 1) {
			return res; // Can't locate source bag info.
		}

		Zeal::EqStructures::EQITEMINFO* item = (Zeal::EqStructures::EQITEMINFO*)wnd->invSlot->Item;
		if (item->Type != 0) {
			return res; // Item is not a common item.
		}

		bool can_class_race_equip = Zeal::EqGame::can_use_item(c, item);
		if (!can_class_race_equip) {
			return res; // Item is not usable by our race/class/deity
		}

		// ------------------------------------------
		// -- Find what slots will accept the item --
		// ------------------------------------------

		WORD src_item_id = item->ID;
		std::vector<std::pair<int, WORD>> dst_slots; // Pairs of {invSlot,itemID} for the valid destination slots

		// First look for empty slots, prioritize those.
		for (int i : PRIORITY_ORDER) {
			if (!c->InventoryItem[i] && Zeal::EqGame::can_item_equip_in_slot(c, item, i + 1)) {
				dst_slots.push_back(std::make_pair<int, WORD>(i + 1, 0));
			}
		}

		// Looks for any valid slot if no empty ones were available.
		if (dst_slots.empty()) {
			for (int i : PRIORITY_ORDER) {
				if (c->InventoryItem[i] && Zeal::EqGame::can_item_equip_in_slot(c, item, i + 1)) {
					if (c->InventoryItem[i]->Type != 0) {
						continue; // Equipped item isn't a normal item? Skip it.
					}
					if (c->InventoryItem[i]->ID == src_item_id) {
						if (item->Common.IsStackable) {
							if (c->InventoryItem[i]->Common.StackCount >= 20) {
								continue; // Equipepd item is already a full stack of the same item. Skip it.
							}
						}
						else if (item->Common.Charges == c->InventoryItem[i]->Common.Charges) {
							continue; // Equiped item is the same item. Skip it.
						}
					}
					else if (container->Container.SizeCapacity < c->InventoryItem[i]->Size) {
						continue; // Equipped item won't fit into this bag when we swap. Skip it.
					}
					dst_slots.push_back(std::make_pair(i + 1, c->InventoryItem[i]->ID));
				}
			}
			if (dst_slots.empty()) {
				return res; // Couldn't find any slot to equip the item in.
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

		int dst_inv_slot = dst_slots[i].first;
		WORD dst_item_id = dst_slots[i].second;

		// --------------------
		// -- Begin Swapping --
		// --------------------

		// Pre-swap:
		// - Force the keyboard modifiers 'SHIFT'.
		// - This ensure we pick up stacked items when sending LeftClick events.
		wnd_mgr->ShiftKeyState = 1;
		wnd_mgr->ControlKeyState = 0;
		wnd_mgr->AltKeyState = 0;
		unsigned int click_flags = 0x20001;

		// (1) Pickup the bagged item.
		wnd->LeftClickDown(mouse_x, mouse_y, click_flags);
		wnd->LeftClickUp(mouse_x, mouse_y, click_flags);
		// Now we should be holding the item to swap
		if (c->CursorItem == item) {
			// (2) Equip it in the destination slot.
			ClickInventoryWindowSlot(dst_inv_slot, click_flags);
			// Now we should be holding the swapped-out item instead (if any).
			if (dst_item_id && c->CursorItem && c->CursorItem->ID == dst_item_id) {
				// (3) Put the swapped-out item in the bag slot we just emptied.
				wnd->LeftClickDown(mouse_x, mouse_y, click_flags);
				wnd->LeftClickUp(mouse_x, mouse_y, click_flags);
			}
		}

		// Done. Restore the keyboard flags.
		wnd_mgr->ShiftKeyState = shift;
		wnd_mgr->ControlKeyState = ctrl;
		wnd_mgr->AltKeyState = alt;
	}

	return res;
}

EquipItem::EquipItem(ZealService* zeal, IO_ini* ini)
{
	// Modify the Alt + Left Mouse click SetItem() related callback of CInvSlotWnd.
	auto* inv_slot_wnd_vtable = Zeal::EqUI::InvSlotWnd::default_vtable;
	mem::unprotect_memory(inv_slot_wnd_vtable, sizeof(*inv_slot_wnd_vtable));
	inv_slot_wnd_vtable->HandleRButtonUp = InvSlotWnd_HandleRButtonUp;
	mem::reset_memory_protection(inv_slot_wnd_vtable);
}

EquipItem::~EquipItem()
{
}