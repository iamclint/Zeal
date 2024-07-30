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

void NPCGive::HandleItemPickup()
{
    if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->Give && Zeal::EqGame::Windows->Give->IsVisible)
        wait_cursor_item = true;
}

void NPCGive::tick()
{
    if (!Zeal::EqGame::is_in_game())
    {
        wait_cursor_item = false;
        return;
    }
    if (!Zeal::EqGame::Windows || !Zeal::EqGame::Windows->Give || !Zeal::EqGame::Windows->Give->IsVisible)
    {
        wait_cursor_item = false;
        return;
    }
    if (wait_cursor_item && Zeal::EqGame::get_char_info()->CursorItem)
    {
        int trade_size = 4;
        int ptr1 = *(int*)0x7f94c8;
        if (*(BYTE*)(ptr1 + 0xa8) == 0)
            trade_size = 8;
        for (int i = 0; i < trade_size; i++) //look for an empty slot
        {
            if (!Zeal::EqGame::Windows->Trade->Item[i])
            {
                Zeal::EqGame::move_item(0, i + 0xbb8, 0, 1); //move item
                break;
            }
        }
        wait_cursor_item = false;
    }
}

NPCGive::NPCGive(ZealService* zeal, IO_ini* ini)
{
    zeal->callbacks->AddGeneric([this]() { tick();  });
    zeal->hooks->Add("QtyPickupItem", 0x42F65A, QtyPickupItem, hook_type_detour); //Hook in to end melody as well.
    zeal->commands_hook->Add("/npcgive", { "/ngv" }, "Toggles on and off the ctrl+click item picking up going directly to your give window if its open.",
        [this](std::vector<std::string>& args) {

            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
}

NPCGive::~NPCGive()
{
}
