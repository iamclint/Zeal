#include "auto_stand.h"
#include "Zeal.h"
#include "EqAddresses.h"

void AutoStand::handle_movement_binds(int cmd, bool key_down)
{
	if (!Zeal::EqGame::game_wants_input() && key_down)
	{
		if (!Zeal::EqGame::KeyMods->Alt && !Zeal::EqGame::KeyMods->Shift && !Zeal::EqGame::KeyMods->Ctrl)
		{
			if (Zeal::EqGame::Windows->Loot && Zeal::EqGame::Windows->Loot->IsOpen && Zeal::EqGame::Windows->Loot->IsVisible)
			{
				Zeal::EqGame::EqGameInternal::CLootWndDeactivate((int)Zeal::EqGame::Windows->Loot, 0);
				return;
			}
			else if (Zeal::EqGame::Windows->SpellBook && Zeal::EqGame::Windows->SpellBook->IsVisible)
			{
				//Zeal::EqGame::change_stance(Stance::Stand);
				//Zeal::EqGame::Windows->CSpellBook->IsVisible = false;
				return;
			}
			Zeal::EqGame::change_stance(Stance::Stand);
		}
	}
}

void AutoStand::handle_spellcast_binds(int cmd)
{
	if (!Zeal::EqGame::game_wants_input())
	{
		if (Zeal::EqGame::Windows->Loot && Zeal::EqGame::Windows->Loot->IsOpen && Zeal::EqGame::Windows->Loot->IsVisible)
		{
			return;
		}
		else if (Zeal::EqGame::Windows->SpellBook && Zeal::EqGame::Windows->SpellBook->IsVisible)
		{
			//Zeal::EqGame::change_stance(Stance::Stand);
			//Zeal::EqGame::Windows->CSpellBook->IsVisible = false;
			return;
		}
		Zeal::EqGame::change_stance(Stance::Stand);
	}
}

AutoStand::AutoStand(ZealService* zeal)
{

}
AutoStand::~AutoStand()
{

}