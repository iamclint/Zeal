#include "looting.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"

//void __fastcall finalize_loot(int uk, int lootwnd_ptr)
//{
//	Zeal::EqStructures::Entity* corpse =  Zeal::EqGame::get_active_corpse();
//	if (ZealService::get_instance()->gui->Options->hidecorpse_looted && corpse)
//		corpse->ActorInfo->IsInvisible = 1;
//	ZealService::get_instance()->hooks->hook_map["FinalizeLoot"]->original(finalize_loot)(uk, lootwnd_ptr);
//}
void __fastcall release_loot(int uk, int lootwnd_ptr)
{
	Zeal::EqStructures::Entity* corpse = Zeal::EqGame::get_active_corpse();
	corpse->ActorInfo->IsInvisible = 1;
	ZealService::get_instance()->hooks->hook_map["ReleaseLoot"]->original(release_loot)(uk, lootwnd_ptr);
}

looting::~looting()
{
}

looting::looting(ZealService* zeal)
{
	zeal->hooks->Add("ReleaseLoot", Zeal::EqGame::EqGameInternal::fn_releaseloot, release_loot, hook_type_detour);
}
