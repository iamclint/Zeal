#include "autofire.h"
#include "Zeal.h"
//
//bool AutoFire::HandleDoAttack(Zeal::EqStructures::Entity* player, uint8_t type, uint8_t p2, Zeal::EqStructures::Entity* target)
//{
//    Zeal::EqStructures::_EQITEMINFO* ranged_item = Zeal::EqGame::get_char_info()->Inventory.Ranged;
//    if (!autofire || !ranged_item)
//        return false;
//    static bool disabled_auto = false;
//    if (type == 13 || type == 14)
//    {
//        ZealService::get_instance()->eqstr_hook->str_noprint[124] = true; //don't print too far away messaging
//        if (!Zeal::EqGame::CanIHitTarget(0.f))
//        {
//            if (player->Position.Dist2D(target->Position) > 30.f )
//            {
//                if (!do_autofire)
//                {
//                    was_autoattacking = *(BYTE*)0x7f6ffe;
//                    *(BYTE*)0x7f6ffe = 0; //disable auto attack
//                    do_autofire = true;
//                }
//                else
//                {
//                    do_autofire = false;
//                }
//                ZealService::get_instance()->eqstr_hook->str_noprint[124] = false;
//                return true;
//            }
//        }
//        else
//        {
//            do_autofire = false;
//        }
//    }
//    return false;
//}

void AutoFire::Main()
{
    if (!Zeal::EqGame::get_target() || *(BYTE*)0x7f6ffe)
    {
        SetAutoFire(false);
        return;
    }
    static ULONGLONG last_print_time = GetTickCount64();
    if (autofire)
    {
        if (*(BYTE*)0x7cd844)
        {
            int range = 0;
            if (Zeal::EqGame::get_char_info()->Inventory.Ranged && Zeal::EqGame::get_char_info()->Inventory.Ranged->Common.Skill == 0x5)
            {
                range += Zeal::EqGame::get_char_info()->Inventory.Ranged->Common.Range;
                if (Zeal::EqGame::get_char_info()->Inventory.Ammo)
                    range += Zeal::EqGame::get_char_info()->Inventory.Ammo->Common.Range;
            }
            else if (!Zeal::EqGame::get_char_info()->Inventory.Ranged && Zeal::EqGame::get_char_info()->Inventory.Ammo->Common.Range)
            {
                range += Zeal::EqGame::get_char_info()->Inventory.Ammo->Common.Range;
            }
            if (GetTickCount64() - last_print_time<500)
            {
                ZealService::get_instance()->eqstr_hook->str_noprint[124] = true;
            }
            else
            {
                last_print_time = GetTickCount64();
            }

            if (Zeal::EqGame::CanIHitTarget(static_cast<float>(range)))
            {
                *(BYTE*)0x7cd844 = 0;
                Zeal::EqGame::do_attack(11, 0);
            }

            ZealService::get_instance()->eqstr_hook->str_noprint[124] = false;
        }
    }
}

//bool __fastcall DoAttack(Zeal::EqStructures::Entity* player, int unused, uint8_t type, uint8_t p2, Zeal::EqStructures::Entity* target)
//{
//    if (player && target && ZealService::get_instance()->autofire->HandleDoAttack(player, type, p2, target))
//        return true;
//        
//    return ZealService::get_instance()->hooks->hook_map["DoAttack"]->original(DoAttack)(player, unused, type, p2, target);
//}
void AutoFire::SetAutoFire(bool enabled)
{
    if (autofire && !enabled) 
        Zeal::EqGame::print_chat("Autofire disabled"); 
    else if (enabled)
    {
        Zeal::EqGame::print_chat("Autofire enabled.");
        *(BYTE*)0x7f6ffe = 0; //disable auto attack
    }
    autofire = enabled;
    do_autofire = false;
}

AutoFire::AutoFire(ZealService* zeal, IO_ini* ini)
{
  //  zeal->hooks->Add("DoAttack", 0x50A0F8, DoAttack, hook_type_detour);
   // zeal->callbacks->add_generic([this]() { SetAutoFire(false);  }, callback_type::Zone);
    zeal->callbacks->add_generic([this]() { SetAutoFire(false); }, callback_type::CharacterSelect);
    zeal->callbacks->add_generic([this]() { SetAutoFire(false);  }, callback_type::EndMainLoop);
    zeal->callbacks->add_generic([this]() { Main();  }, callback_type::MainLoop);
    zeal->callbacks->add_packet([this](UINT opcode, char* buffer, UINT size) {
        if (opcode == 0x4161 || opcode == 0x4151)
        {
         
        }
        return false;
        //Zeal::EqGame::print_chat("Opcode: 0x%x Size: %i Buffer: %s", opcode, size, byteArrayToHexString(buffer, size).c_str());
        //return false;
    }, callback_type::SendMessage_);
    zeal->commands_hook->add("/autofire", { "/af" },
        [this](std::vector<std::string>& args) {
            SetAutoFire(!autofire);
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
}

AutoFire::~AutoFire()
{
}
