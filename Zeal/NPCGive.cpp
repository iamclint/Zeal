#include "NPCGive.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"

// Stacked items go through this path which includes the call to CInvSlot::SliderComplete()
// that will create a new item pointer when a stack is split (like using ctrl on a stack).
// That stack split puts the item on the cursor immediately w/out a call to MoveItem. If 
// not split, the MoveItem() call happens to move the existing item to the cursor.
void __fastcall QtyPickupItem(int cquantitywnd, int unused)
{
    auto char_info = Zeal::EqGame::get_char_info();
    bool empty_cursor = char_info && char_info->CursorItem == nullptr;
    ZealService::get_instance()->hooks->hook_map["QtyPickupItem"]->original(QtyPickupItem)(cquantitywnd, unused);

    if (char_info && empty_cursor && char_info->CursorItem)
        ZealService::get_instance()->give->HandleItemInCursor();
}

// Intercept the MoveItem call to detect the movement of items to the cursor in order to possibly arm the
// /singleclick logic to auto-move from the cursor to the destination (trade, crafting).
static void __fastcall CInvSlotMgrMoveItem(void* mgr, int unused_edx, int from_slot, int to_slot,
    int print_error, int unknown)
{
    // The MoveItem is a generic call and we only want to trigger moving items from the cursor and
    // only from the results of a HandleLButtonUp / SliderComplete parent call. Additionally, the
    // right click to equip in equip_item.cpp is calling the HandleLButtonUp multiple times in a
    // row to move so we need to co-exist with that. Those calls are atomic, so the end result
    // should be an item moving off the cursor which will clear the trigger item state. The 
    // msg_disarm and the DropItemIntoTrade calls all set print_error == 0, so we filter for that.
    // Also, we confirm that the from_slot is not the cursor and the to_slot is the cusor.
    // HandleItemPickup() does additional filtering.
    ZealService::get_instance()->give->ClearItem();
    if (from_slot != 0 && to_slot == 0 && print_error == 1 && unknown == 1)
        ZealService::get_instance()->give->HandleItemPickup(from_slot);

    ZealService::get_instance()->hooks->hook_map["CInvSlotMgrMoveItem"]->original(CInvSlotMgrMoveItem)(mgr, unused_edx, from_slot, to_slot, print_error, unknown);
}

static bool is_give_or_trade_window_active()
{
    return (Zeal::EqGame::Windows->Give && Zeal::EqGame::Windows->Give->IsVisible) ||
        (Zeal::EqGame::Windows->Trade && Zeal::EqGame::Windows->Trade->IsVisible);
}

static Zeal::EqUI::ContainerWnd* get_active_tradeskill_container(int bag_index)
{
    // Scan the open container windows for tradeskill ones.
    static int kFirstCombineType = 9;  // Types < 7 are bags.

    // If a world container is open, focus on that only.
    auto container_mgr = Zeal::EqGame::Windows->ContainerMgr;
    if (!container_mgr)
        return nullptr;

    if (container_mgr->pWorldItems)
    {
        // Return the world window if open, combinable, and visible.
        if (container_mgr->pWorldItems->Container.IsOpen && 
            container_mgr->pWorldItems->Container.Combine >= kFirstCombineType)
            for (int i = 0; i < 0x11; ++i)
            {
                auto wnd = container_mgr->pPCContainers[i];
                if (wnd && (wnd->pContainerInfo == container_mgr->pWorldItems))
                    return (wnd->IsVisible) ? wnd : nullptr;
            }
        return nullptr;
    }

    // World container isn't open. Check for the /singleclick bag <target #>
    if (bag_index < 1 || bag_index > 8)
        return nullptr;

    auto char_info = Zeal::EqGame::get_char_info();
    if (!char_info)
        return nullptr;
    auto item_info = char_info->InventoryPackItem[bag_index - 1];

    Zeal::EqUI::ContainerWnd* inventory_bag = nullptr;
    for (int i = 0; i < 0x11; ++i)
    {
        auto wnd = container_mgr->pPCContainers[i];
        if (wnd && wnd->IsVisible && wnd->pContainerInfo &&
            wnd->pContainerInfo == item_info &&
            wnd->pContainerInfo->Container.IsOpen &&
            wnd->pContainerInfo->Container.Combine >= kFirstCombineType)
            return wnd;
    }
    return nullptr;
}

void NPCGive::ClearItem()
{
    wait_cursor_item = nullptr;
}

void NPCGive::HandleItemInCursor()
{
    ClearItem();  // Default to resetting the item.

    // Bail out if the cursor has no item or is holding money.
    auto char_info = Zeal::EqGame::get_char_info();
    if (!char_info || !char_info->CursorItem || char_info->CursorCopper || char_info->CursorGold
        || char_info->CursorPlatinum || char_info->CursorSilver)
        return;

    // Bail out if not enabled, key isn't held, or there's no target option.
    if (!setting_enable_give.get() || !Zeal::EqGame::is_new_ui() ||
        !(Zeal::EqGame::KeyMods->Shift || Zeal::EqGame::KeyMods->Ctrl) ||
        !(is_give_or_trade_window_active() || get_active_tradeskill_container(bag_index)))
        return;

    wait_cursor_item = char_info->CursorItem;
}

void NPCGive::HandleItemPickup(int from_slot)
{
    ClearItem();  // Default to resetting the item.

    if (from_slot == 0)
        return;  // Bail out if the cursor is the source.

    // Bail out if the cursor has an item on it or we are holding money.
    auto char_info = Zeal::EqGame::get_char_info();
    if (!char_info || char_info->CursorItem || char_info->CursorCopper || char_info->CursorGold
        || char_info->CursorPlatinum || char_info->CursorSilver)
        return;

    // Bail out if not enabled, key isn't held, or there's no target option.
    if (!setting_enable_give.get() || !Zeal::EqGame::is_new_ui() ||
        !(Zeal::EqGame::KeyMods->Shift || Zeal::EqGame::KeyMods->Ctrl) ||
        !(is_give_or_trade_window_active() || get_active_tradeskill_container(bag_index)))
        return;

    // Cache item if the from_slot is a general inventory slot.
    if (from_slot >= 22 && from_slot <= 29)
    {
        wait_cursor_item = char_info->InventoryPackItem[from_slot - 22];
        return;
    }

    // Cache item if the from_slot is a general bag slot.
    // Slot ID for bagged items is 250 + (bag_index*10) + (bag_slot) = [250...329]
    if (from_slot >= 250 && from_slot < 330)
    {
        int bag_index = (from_slot - 250) / 10;
        int bag_slot = (from_slot - 250) % 10;
        auto bag = char_info->InventoryPackItem[bag_index];
        if (!bag || bag->Type != 1 || bag_slot >= bag->Container.Capacity)
            return;  // Error finding the source bag for the from_slot.
        wait_cursor_item = bag->Container.Item[bag_slot];
    }
}

void NPCGive::tick()
{
    if (!wait_cursor_item)
        return;

    auto target_cursor_item = wait_cursor_item;
    ClearItem();  // Only make a single attempt whenever set.

    // Must be a non-container item on the cursor and still be in the game.
    auto char_info = Zeal::EqGame::get_char_info();
    if (!char_info || char_info->CursorItem != target_cursor_item ||
        char_info->CursorItem->Type != 0 || char_info->CursorItem->NoDrop == 0 ||  // Extra filtering.
        !Zeal::EqGame::is_in_game())
        return;

    if (ZealService::get_instance()->looting_hook->is_item_protected_from_selling(
        char_info->CursorItem))
    {
        Zeal::EqGame::print_chat("Zeal disabled the single click move to trade or craft");
        return;
    }

    // If the give/trade window is open, that is the only possible destination.
    if (is_give_or_trade_window_active())
    {
        auto trade_target = *reinterpret_cast<Zeal::EqStructures::Entity**>(0x007f94c8);
        if (!trade_target || !Zeal::EqGame::Windows->Trade) 
            return;   // Needs a clear target and the trade window to be available for xfer.

        // TODO: We are disabling singleclick into player trade windows until more logic is added.
        // The client path goes through DropItemIntoTrade() that calls methods like CanDrop() and
        // UpdateTradeDisplay().
        if (trade_target->Type != Zeal::EqEnums::NPC)  // No trading to corpses either.
            return;

        if ((trade_target->Type == Zeal::EqEnums::NPC &&
            (!Zeal::EqGame::Windows->Give || !Zeal::EqGame::Windows->Give->IsVisible))
            || (trade_target->Type == Zeal::EqEnums::Player && !Zeal::EqGame::Windows->Trade->IsVisible))
            return;  // Need clear destination with expected window.

        const int trade_size = (trade_target->Type == Zeal::EqEnums::Player) ? 8 : 4;
        for (int i = 0; i < trade_size; i++) // Look for an empty slot.
        {   // Both Give and Trade windows use the Trade window slots.
            if (!Zeal::EqGame::Windows->Trade->GiveItems[i])
            {
                Zeal::EqGame::move_item(0, i + 0xbb8, 0, 1);  // Put in first free slot.
                // TODO: For players need to call UpdateTradeDisplay() and reset accept status.
                // Possibly also should be adjusting the cursor visibility based on success.
                break;
            }
        }
        return;
    }

    // Next check if there is a tradeskill container window open that can accept the item.
    auto wnd = get_active_tradeskill_container(bag_index);
    if (!wnd || !wnd->pContainerInfo ||
        wnd->pContainerInfo->Container.SizeCapacity < char_info->CursorItem->Size)
        return;

    const int num_slots = wnd->pContainerInfo->Container.Capacity;
    for (int i = 0; i < num_slots; ++i)
    {
        auto invslot = wnd->pSlotWnds[i];
        if (invslot->invSlot && !invslot->invSlot->Item)
        {
            Zeal::EqGame::move_item(0, invslot->SlotID, 0, 1);
            break;
        }
    }
}

NPCGive::NPCGive(ZealService* zeal)
{
    zeal->callbacks->AddGeneric([this]() { tick();  });
    zeal->hooks->Add("QtyPickupItem", 0x42F65A, QtyPickupItem, hook_type_detour);
    zeal->hooks->Add("CInvSlotMgrMoveItem", 0x00422b1c, CInvSlotMgrMoveItem, hook_type_detour);
    zeal->commands_hook->Add("/singleclick", {},
        "Toggles on and off the single click auto-transfer of stackable items to open give, trade, or crafting windows.",
        [this](std::vector<std::string>& args) {
            int value = 0;
            if (args.size() == 1) {
                setting_enable_give.toggle();
                Zeal::EqGame::print_chat("Single click give: %s", setting_enable_give.get() ? "ON" : "OFF");
            }
            else if (args.size() == 3 && args[1] == "bag" && Zeal::String::tryParse(args[2], &value))
            {
                bag_index = value;
                Zeal::EqGame::print_chat("Single click bag index set to %d", bag_index);
            }
            else {
                Zeal::EqGame::print_chat("Usage: /singleclick to toggle, /singleclick bag # (1-8, 0 disables)");
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
}

NPCGive::~NPCGive()
{
}
