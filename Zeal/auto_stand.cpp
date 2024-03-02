#include "auto_stand.h"
#include "Zeal.h"
#include "EqAddresses.h"
void AutoStand::handle_binds(int cmd, bool key_down)
{
	if (!Zeal::EqGame::game_wants_input())
	{
		if (!Zeal::EqGame::KeyMods->Alt && !Zeal::EqGame::KeyMods->Shift && !Zeal::EqGame::KeyMods->Ctrl)
		{
			if (Zeal::EqGame::LootWnd && Zeal::EqGame::LootWnd->IsOpen && Zeal::EqGame::LootWnd->IsVisible)
			{
				Zeal::EqGame::EqGameInternal::CLootWndDeactivate((int)Zeal::EqGame::LootWnd, 0);
				return;
			}
			else if (Zeal::EqGame::SpellBookWnd && Zeal::EqGame::SpellBookWnd->IsVisible)
				return;
			
			if (Zeal::EqGame::get_self() && Zeal::EqGame::get_self()->StandingState == Zeal::EqEnums::Stance::Sitting)
				Zeal::EqGame::change_stance(Stance::Stand);
		}
	}
}

AutoStand::AutoStand(ZealService* zeal)
{

}
AutoStand::~AutoStand()
{

}