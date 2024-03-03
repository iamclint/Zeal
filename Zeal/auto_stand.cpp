#include "auto_stand.h"
#include "Zeal.h"
#include "EqAddresses.h"
void AutoStand::handle_binds(int cmd, bool key_down)
{
	if (!Zeal::EqGame::game_wants_input() && key_down)
	{
		if (!Zeal::EqGame::KeyMods->Alt && !Zeal::EqGame::KeyMods->Shift && !Zeal::EqGame::KeyMods->Ctrl)
		{
			if (Zeal::EqGame::Windows->CLoot && Zeal::EqGame::Windows->CLoot->IsOpen && Zeal::EqGame::Windows->CLoot->IsVisible)
			{
				Zeal::EqGame::EqGameInternal::CLootWndDeactivate((int)Zeal::EqGame::Windows->CLoot, 0);
				return;
			}
			else if (Zeal::EqGame::Windows->CSpellBook && Zeal::EqGame::Windows->CSpellBook->IsVisible)
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