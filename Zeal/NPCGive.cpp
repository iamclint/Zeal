#include "NPCGive.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"


void __fastcall QtyPickupItem(int cquantitywnd, int unused)
{
    ZealService::get_instance()->give->HandleItemPickup();
    ZealService::get_instance()->hooks->hook_map["QtyPickupItem"]->original(QtyPickupItem)(cquantitywnd, unused);
}

static bool is_give_or_trade_window_active()
{
    return (Zeal::EqGame::Windows->Give && Zeal::EqGame::Windows->Give->IsVisible) ||
        (Zeal::EqGame::Windows->Trade && Zeal::EqGame::Windows->Trade->IsVisible);
}

static Zeal::EqUI::ContainerWnd* get_active_tradeskill_container()
{
    // Tradeskill combine containers are just a differently labeled "worldbag". To detect if one
    // is open, we just search the titles of all active tables to see if it contains any of
    // the substrings below.
    // Note: The EQITEMCONTAINERINFO::Combine field maps to the EqPacket::Combine_Struct::worldobjecttype
    // field, and that field is used to check versus race and class so it is another possible check.
    static std::vector<std::string> craftbags = { "Pottery Wheel", "Medicine Bag", "Mixing Bowl",
            "Oven", "Sewing Kit", "Forge", "Fletching Kit", "Brew Barrel", "Jeweler's Kit", "Kiln" };

    auto container_mgr = Zeal::EqGame::Windows->ContainerMgr;
    if (!container_mgr)
        return nullptr;

    for (int i = 0; i < 0x11; ++i)
    {
        auto wnd = container_mgr->pPCContainers[i];
        if (!wnd || !wnd->IsVisible || !wnd->pContainerInfo)  // Note: ItemInfo.Type != 1 for some reason.
            continue;
        const char* name = wnd->pLabel->Text.CastToCharPtr();
        if (!name)
            continue;
        if (std::find_if(craftbags.begin(), craftbags.end(),
            [name](const std::string& craft) {return std::string(name).find(craft) != std::string::npos; })
            != craftbags.end())
            return wnd;
    }
    return nullptr;
}

void NPCGive::HandleItemPickup()
{
    if (setting_enable_give.get() &&
        (is_give_or_trade_window_active() || get_active_tradeskill_container()))
        wait_cursor_item = true;
}

void NPCGive::tick()
{
    if (!wait_cursor_item)
        return;

    wait_cursor_item = false;  // Only make a single attempt whenever set.

    // Must be a non-container item on the cursor and still be in the game.
    if (!Zeal::EqGame::get_char_info() ||
        !Zeal::EqGame::get_char_info()->CursorItem || 
        Zeal::EqGame::get_char_info()->CursorItem->Type != 0 ||
        !Zeal::EqGame::is_in_game())
        return;

    if (ZealService::get_instance()->looting_hook->is_item_protected_from_selling(
        Zeal::EqGame::get_char_info()->CursorItem))
    {
        Zeal::EqGame::print_chat("Zeal disabled the single click move to trade or craft");
        return;
    }

    // First check for the NPC give and trade windows as highest priority.
    if (is_give_or_trade_window_active())
    {
        auto trade_target = *reinterpret_cast<Zeal::EqStructures::Entity**>(0x007f94c8);
        if (!trade_target || !Zeal::EqGame::Windows->Trade)
            return;   // Should have one, but just bail if not.

        const int trade_size = (trade_target->Type == Zeal::EqEnums::Player) ? 8 : 4;
        for (int i = 0; i < trade_size; i++) // Look for an empty slot.
        {
            if (!Zeal::EqGame::Windows->Trade->Item[i])
            {
                Zeal::EqGame::move_item(0, i + 0xbb8, 0, 1);  // Put in first free slot.
                break;
            }
        }
        return;
    }

    // Next check if there is a tradeskill container window open that can accept the item.
    auto wnd = get_active_tradeskill_container();
    if (!wnd || !wnd->pContainerInfo ||  // Note: Trade station ItemInfo.Type != 1 for some reason.
        wnd->pContainerInfo->Container.SizeCapacity < Zeal::EqGame::get_char_info()->CursorItem->Size)
        return;

    const int num_slots = wnd->pContainerInfo->Container.Capacity;
    for (int i = 0; i < num_slots; ++i)
    {
        auto invslot = wnd->pSlotWnds[i];
        if (invslot->invSlot && !invslot->invSlot->Item)
        {
            Zeal::EqGame::move_item(0, i + 4000, 0, 1);  // Put in free slot of world bag (4000).
            break;
        }
    }
}

NPCGive::NPCGive(ZealService* zeal, IO_ini* ini)
{
    zeal->callbacks->AddGeneric([this]() { tick();  });
    zeal->hooks->Add("QtyPickupItem", 0x42F65A, QtyPickupItem, hook_type_detour); //Hook in to end melody as well.
    zeal->commands_hook->Add("/singleclick", {},
        "Toggles on and off the single click auto-transfer of stackable items to open give, trade, or crafting windows.",
        [this](std::vector<std::string>& args) {
            setting_enable_give.toggle();
            Zeal::EqGame::print_chat("Single click give: %s", setting_enable_give.get() ? "ON" : "OFF");
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
}

NPCGive::~NPCGive()
{
}
