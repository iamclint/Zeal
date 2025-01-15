#include "player_movement.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"

static void CloseSpellbook(void)
{
	Zeal::EqGame::get_self()->ChangeStance(Stance::Stand);
	Zeal::EqGame::Windows->SpellBook->IsVisible = false;
}

void PlayerMovement::handle_movement_binds(int cmd, bool key_down)
{
	if (!Zeal::EqGame::game_wants_input() && key_down)
	{
		if (!Zeal::EqGame::KeyMods->Alt && !Zeal::EqGame::KeyMods->Shift && !Zeal::EqGame::KeyMods->Ctrl)
		{

			if (Zeal::EqGame::is_new_ui())
			{
				if (Zeal::EqGame::Windows->Loot && Zeal::EqGame::Windows->Loot->IsOpen && Zeal::EqGame::Windows->Loot->IsVisible)
				{
					Zeal::EqGame::EqGameInternal::CLootWndDeactivate((int)Zeal::EqGame::Windows->Loot, 0);
					return;
				}
				else if (Zeal::EqGame::Windows->SpellBook && Zeal::EqGame::Windows->SpellBook->IsVisible)
				{
					switch (cmd) {
						case 3:
							CloseSpellbook();
							break;
						case 5: 
						case 6: 
						case 211:
						case 212:
							if (!SpellBookAutoStand.get()) { return; }
								CloseSpellbook();
							break;
						default: { return; }
					}
				}
			}
			else
			{
				if (Zeal::EqGame::get_active_corpse())
				{
					// how do we close corpse without closing the window?
					return;
				}
				else if (Zeal::EqGame::OldUI::spellbook_window_open())
				{
					// left and right arrows dont turn pages on oldui by default
					// so I'm not sure if we'll add support for other keys
					if (cmd == 4) { return; }
					else if (cmd == 5 && !SpellBookAutoStand.get())  { return; }
					else if (cmd == 6 && !SpellBookAutoStand.get()) { return; }
				}
			}

			switch (Zeal::EqGame::get_controlled()->StandingState)
			{
				case Zeal::EqEnums::Stance::Sitting:
					Zeal::EqGame::get_controlled()->ChangeStance(Stance::Stand);
					break;
				default: { return; }
			}
		}
	}
}

void PlayerMovement::handle_spellcast_binds(int cmd)
{
	if (!Zeal::EqGame::game_wants_input())
	{
		if (Zeal::EqGame::is_new_ui())
		{
			if (Zeal::EqGame::Windows->Loot && Zeal::EqGame::Windows->Loot->IsOpen && Zeal::EqGame::Windows->Loot->IsVisible)
			{
				return;
			}
			else if (Zeal::EqGame::Windows->SpellBook && Zeal::EqGame::Windows->SpellBook->IsVisible)
			{
				return;
			}
		}
		else
		{
			if (Zeal::EqGame::get_active_corpse())
			{
				// how do we close corpse without closing the window?
				return;
			}
			else if (Zeal::EqGame::OldUI::spellbook_window_open())
			{
				return;
			}
		}

		if (ZealService::get_instance()->movement->CastAutoStand.get() && Zeal::EqGame::get_self()->StandingState == Zeal::EqEnums::Stance::Sitting)
			Zeal::EqGame::get_self()->ChangeStance(Stance::Stand);
	}
}

void PlayerMovement::set_strafe(strafe_direction dir)
{
	//slightly revised so the game properly sets speed based on encumber and handles the stance checks
	if (dir == strafe_direction::None || !Zeal::EqGame::can_move())
	{
		current_strafe = strafe_direction::None;
		*Zeal::EqGame::strafe_direction = 0;
		*Zeal::EqGame::strafe_speed = 0;
		if (orig_reset_strafe[0] != 0)
			mem::copy(0x53f424, orig_reset_strafe, 7);
	}
	else
	{
		if (orig_reset_strafe[0] == 0)
			mem::set(0x53f424, 0x90, 7, orig_reset_strafe);
		else if (*(BYTE*)0x53f424 != 0x90)
			mem::set(0x53f424, 0x90, 7);


		if (dir == strafe_direction::Right)
		{
			current_strafe = strafe_direction::Right;
			*Zeal::EqGame::strafe_direction = 0x2;
		}
		else
		{
			current_strafe = strafe_direction::Left;
			*Zeal::EqGame::strafe_direction = 0x1;
		}
	}
}

void PlayerMovement::callback_main()
{
	if (current_strafe != strafe_direction::None)
	{
		Zeal::EqStructures::Entity* controlled_player = Zeal::EqGame::get_controlled();
		float encumberance = Zeal::EqGame::encum_factor();
		*Zeal::EqGame::strafe_speed = encumberance + encumberance;
		if (controlled_player->IsSneaking || controlled_player->StandingState == Stance::Duck)
			*Zeal::EqGame::strafe_speed *= .5f;
		if (controlled_player->ActorInfo && controlled_player->ActorInfo->MovementSpeedModifier < 0)
			*Zeal::EqGame::strafe_speed *= .5f;
		if (controlled_player->ActorInfo && controlled_player->ActorInfo->Entity0198 != 0)
			*Zeal::EqGame::strafe_speed *= .25f;
		if (controlled_player->ActorInfo && controlled_player->ActorInfo->MovementSpeedModifier < -1000.0f)
		{
			*Zeal::EqGame::strafe_direction = 0;
			*Zeal::EqGame::strafe_speed = 0;
		}
		if (controlled_player->StandingState != Stance::Duck && controlled_player->StandingState != Stance::Stand)
		{
			*Zeal::EqGame::strafe_direction = 0;
			*Zeal::EqGame::strafe_speed = 0;
		}
	}
}

//void PlayerMovement::load_settings()
//{
//	if (!ini_handle->exists("Zeal", "SpellbookAutostand"))
//		ini_handle->setValue<bool>("Zeal", "SpellbookAutostand", false);
//	//if (!ini_handle->exists("Zeal", "LeftStrafeSpellbookAutostand"))
//	//	ini_handle->setValue<bool>("Zeal", "LeftStrafeSpellbookAutostand", true);
//	//if (!ini_handle->exists("Zeal", "RightStrafeSpellbookAutostand"))
//	//	ini_handle->setValue<bool>("Zeal", "RightStrafeSpellbookAutostand", true);
//
//	SpellBookAutoStand = ini_handle->getValue<bool>("Zeal", "SpellbookAutostand");
//	/*spellbook_right_autostand = ini_handle->getValue<bool>("Zeal", "RightTurnSpellbookAutostand");
//	spellbook_left_strafe_autostand = ini_handle->getValue<bool>("Zeal", "LeftStrafeSpellbookAutostand");
//	spellbook_right_strafe_autostand = ini_handle->getValue<bool>("Zeal", "RightStrafeSpellbookAutostand");*/
//}
//

int __fastcall CastSpell(void* this_ptr, void* not_used, unsigned char a1, short a2, Zeal::EqStructures::EQITEMINFO** a3, short a4)
{
	if (ZealService::get_instance()->movement->CastAutoStand.get() && Zeal::EqGame::get_self() && Zeal::EqGame::get_self()->StandingState == Zeal::EqEnums::Stance::Sitting)
			Zeal::EqGame::get_self()->ChangeStance(Stance::Stand);
	return ZealService::get_instance()->hooks->hook_map["CastSpell"]->original(CastSpell)(this_ptr, not_used, a1, a2, a3, a4);
}

PlayerMovement::PlayerMovement(ZealService* zeal, class Binds* binds, class IO_ini* ini)
{
	ini_handle = ini;
	//zeal->hooks->Add("CastSpell", 0x004C483B, CastSpell, hook_type_detour);
	
	// ISSUE: Mapping LEFT/RIGHT arrow keys to strafe on TAKP2.1 client fails to function.
	binds->replace_cmd(211, [this](int state) {
		if (!state && current_strafe == strafe_direction::Left)
		{
			set_strafe(strafe_direction::None);
		}
		else if (state)
		{
			handle_movement_binds(211, state);
			set_strafe(strafe_direction::Left);
		}
		return false;
	}); // strafe left
	binds->replace_cmd(212, [this](int state) {
		if (!state && current_strafe == strafe_direction::Right)
		{
			set_strafe(strafe_direction::None);
		}
		else if (state)
		{
			handle_movement_binds(212, state);
			set_strafe(strafe_direction::Right);
		}
		return false;
	}); // strafe right

	zeal->commands_hook->Add("/autostand", {}, "Autostands when you cast.",
		[this](std::vector<std::string>& args) {
			if (args.size() == 1 || args.size() > 2)
			{
				Zeal::EqGame::print_chat("usage: /autostand spellbook");
				return true;
			}
			if (args.size() > 1) {
				std::ostringstream oss;
				if (Zeal::String::compare_insensitive(args[1], "spellbook"))
				{
					SpellBookAutoStand.toggle();
					std::string is_enabled = SpellBookAutoStand.get() ? "enabled" : "disabled";
					oss << "[Autostand] spellbook autostand has been " << is_enabled << "." << std::endl;
					Zeal::EqGame::print_chat(oss.str());
					return true;
				}
			}
			return false;
		}
	);
	zeal->callbacks->AddGeneric([this]() { callback_main(); }, callback_type::MainLoop);
}