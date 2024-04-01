#include "melody.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
#include "StringUtil.h"

void Melody::start()
{
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_controlled();
    if (songs.size() && Zeal::EqGame::is_in_game() && self && self->CharInfo && !self->CharInfo->StunnedState)
    {
        current_index = -1;
        active = true;
        Zeal::EqGame::print_chat("You begin playing a melody.");
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
    if (reason == 0)
        ZealService::get_instance()->melody->end();
    else
        ZealService::get_instance()->melody->current_index--;
    ZealService::get_instance()->hooks->hook_map["StopCast"]->original(StopCast)(t, u, reason, spell_id);
}

void Melody::stop_cast()
{
    if (current_index>=0)
        ZealService::get_instance()->hooks->hook_map["StopCast"]->original(StopCast)((int)Zeal::EqGame::get_char_info(), 0, 0, Zeal::EqGame::get_char_info()->MemorizedSpell[songs[current_index]]);
}

void Melody::tick()
{
    if (!Zeal::EqGame::is_in_game())
    {
        end();
        return;
    }
    if (active && Zeal::EqGame::get_char_info())
    {
        Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
        static ULONGLONG casting_timestamp = GetTickCount64();
        if (!Zeal::EqGame::Windows->Casting->IsVisible && GetTickCount64()- casting_timestamp > 150)
        {
            if (Zeal::EqGame::get_char_info()->StunnedState)
                end();

            if (self->StandingState != Stance::Stand)
                return;

            stop_cast();
            current_index++;
            if (current_index >= songs.size() || current_index<0)
                current_index = 0;

            int current_gem = songs[current_index];
            if (Zeal::EqGame::get_char_info()->MemorizedSpell[current_gem] != -1)
                Zeal::EqGame::get_char_info()->cast(current_gem, Zeal::EqGame::get_char_info()->MemorizedSpell[current_gem], 0, 0);
        }
        else if (Zeal::EqGame::Windows->Casting->IsVisible)
            casting_timestamp = GetTickCount64();
    }
}

Melody::Melody(ZealService* zeal, IO_ini* ini)
{
    zeal->callbacks->add_callback([this]() { tick();  });
    zeal->callbacks->add_callback([this]() { end(); }, callback_fn::CharacterSelect);
    zeal->hooks->Add("StopCast", 0x4cb510, StopCast, hook_type_detour); //add extra prints for new loot types
    zeal->commands_hook->add("/melody", { },
        [this](std::vector<std::string>& args) {

            if (active)
            {
                end();
                return true;
            }
            if (Zeal::EqGame::get_char_info()->Class != 8)
            {
                Zeal::EqGame::print_chat("Only bards can keep a melody.");
                songs.clear();
                return true;
            }

            
            
            songs.clear();
            for (int i = 1; i < args.size(); i++) //start at argument 1 because 0 is the command itself
            {
                int current_gem = -1;
                if (StringUtil::tryParse(args[i], &current_gem) && current_gem<9 && current_gem>0)
                {
                    current_gem--; //base 0
                    if (Zeal::EqGame::get_char_info()->MemorizedSpell[current_gem] != -1)
                    {
                        songs.push_back(current_gem);
                    }
                    else
                    {
                        Zeal::EqGame::print_chat("Error spell gem %i is empty", current_gem+1);
                        songs.clear();
                        return true;
                    }
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
