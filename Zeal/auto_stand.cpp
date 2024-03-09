#include "auto_stand.h"
#include "Zeal.h"
#include "EqAddresses.h"

static void CloseSpellbook(void)
{
	Zeal::EqGame::change_stance(Stance::Stand);
	Zeal::EqGame::Windows->SpellBook->IsVisible = false;
}

void AutoStand::handle_movement_binds(int cmd, bool key_down)
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
						case 5: // default should be turn page left (not implemented)
							if (!spellbook_left_autostand)
								return;
							else
								CloseSpellbook();
							break;
						case 6: // default to turn page right (not implemented)
							if (!spellbook_right_autostand)
								return;
							else
								CloseSpellbook();
							break;
						default:{}break;
					}

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
				// spellbook handler here
				// left and right arrows dont turn pages on oldui by default
				// so we probably wont add support for other keys
			}

			// not in a window, handle things normally. (why did this check get removed?)
			switch (Zeal::EqGame::get_self()->StandingState)
			{
				case Zeal::EqEnums::Stance::Sitting:
					Zeal::EqGame::change_stance(Stance::Stand);
					break;
				default: { return; }
			}
		}
	}
}

void AutoStand::handle_spellcast_binds(int cmd)
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
			// need spellbook handler
		}

		// not in a window, handle things normally
		switch (Zeal::EqGame::get_self()->StandingState)
		{
			case Zeal::EqEnums::Stance::Sitting:
				Zeal::EqGame::change_stance(Stance::Stand);
				break;
			default: { return; }
		}
	}
}

void AutoStand::load_settings(IO_ini* ini)
{
	if (!ini->exists("Zeal", "LeftTurnSpellbookAutostand"))
		ini->setValue<bool>("Zeal", "LeftTurnSpellbookAutostand", false);
	if (!ini->exists("Zeal", "RightTurnSpellbookAutostand"))
		ini->setValue<bool>("Zeal", "RightTurnSpellbookAutostand", false);

	spellbook_left_autostand = ini->getValue<bool>("Zeal", "LeftTurnSpellbookAutostand");
	spellbook_right_autostand = ini->getValue<bool>("Zeal", "RightTurnSpellbookAutostand");
}

AutoStand::AutoStand(ZealService* zeal, class IO_ini* ini)
{
	load_settings(ini);
}
AutoStand::~AutoStand()
{

}