#include "melody.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
#include "StringUtil.h"

void Melody::start()
{
    if (songs.size())
    {
        current_index = -1;
        active = true;
        Zeal::EqGame::print_chat("You begin playing a melody.");
        //Zeal::EqGame::get_char_info()->cast(songs.front(), Zeal::EqGame::get_char_info()->MemorizedSpell[songs.front()], 0, 0);
    }
}
void Melody::end()
{
    if (active)
    {
        current_index = 0;
        active = false;
        songs.clear();
        Zeal::EqGame::print_chat("Your melody has ended.");
    }
}

void __fastcall StopCast(int t, int u, BYTE reason, short spell_id)
{
    ZealService::get_instance()->melody->end();
    ZealService::get_instance()->hooks->hook_map["StopCast"]->original(StopCast)(t, u, reason, spell_id);
}
void Melody::stop_cast()
{
    ZealService::get_instance()->hooks->hook_map["StopCast"]->original(StopCast)((int)Zeal::EqGame::get_char_info(), 0, 0, Zeal::EqGame::get_char_info()->MemorizedSpell[songs[current_index]]);
}

void Melody::tick()
{
    if (!Zeal::EqGame::is_in_game())
    {
        end();
        return;
    }
    if (active)
    {
        static ULONGLONG casting_timestamp = GetTickCount64();
        if (!Zeal::EqGame::Windows->Casting->IsVisible && GetTickCount64()- casting_timestamp >500)
        {
            stop_cast();
            current_index++;
            if (current_index >= songs.size())
                current_index = 0;
            Zeal::EqGame::get_char_info()->cast(songs[current_index], Zeal::EqGame::get_char_info()->MemorizedSpell[songs[current_index]], 0, 0);
        }
        else if (Zeal::EqGame::Windows->Casting->IsVisible)
            casting_timestamp = GetTickCount64();

    }

}

Melody::Melody(ZealService* zeal, IO_ini* ini)
{
    zeal->main_loop_hook->add_callback([this]() { tick();  });
    zeal->hooks->Add("StopCast", 0x4cb510, StopCast, hook_type_detour); //add extra prints for new loot types
    zeal->commands_hook->add("/melody", { },
        [this](std::vector<std::string>& args) {
            if (args.size() > 6)
            {
                Zeal::EqGame::print_chat("A melody can only consist of 5 songs");
                return true;
            }
            songs.clear();
            for (int i = 1; i < args.size(); i++) //start at argument 1 because 0 is the command itself
            {
                int current_gem = -1;
                if (StringUtil::tryParse(args[i], &current_gem) && current_gem<9 && current_gem>0)
                {
                    songs.push_back(current_gem - 1); //make it base 0
                }
                else
                {
                    Zeal::EqGame::print_chat("There was an error trying to parse your melody list");
                    songs.clear();
                    return true;
                }
            }
            if (songs.size())
            {
                start();
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    


}

Melody::~Melody()
{
}

