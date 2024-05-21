#include "physics.h"
#include "Zeal.h"
#include "EqAddresses.h"

bool Physics::process()
{
	if (GetTickCount64() - last_physic_calc >= 11)
	{
		last_physic_calc = GetTickCount64();
		did_physics = true;
		return true;
	}
	did_physics = false;
	return false;
}

bool Physics::movement()
{
	return did_physics;
}

void ProcessPhysics(Zeal::EqStructures::Entity* ent, int missile, int effect)
{
	if (ent == Zeal::EqGame::get_controlled())
	{
		if (ZealService::get_instance()->physics->process()) //roughly 30 fps
			ZealService::get_instance()->hooks->hook_map["ProcessPhysics"]->original(ProcessPhysics)(ent, missile, effect);
	}
	else
		ZealService::get_instance()->hooks->hook_map["ProcessPhysics"]->original(ProcessPhysics)(ent, missile, effect);
}

int __fastcall MovePlayer(int t, int u, Zeal::EqStructures::Entity* ent)
{
	if (ent == Zeal::EqGame::get_controlled())
	{
		if (ZealService::get_instance()->physics->movement())
			return ZealService::get_instance()->hooks->hook_map["MovePlayer"]->original(MovePlayer)(t, u, ent);
		else
			reinterpret_cast<void(__thiscall*)(int t, Zeal::EqStructures::Entity * ent)>(0x4db384)(t, ent); //do cam animations so camera movement is smooth still
		return 1;
	}
	return ZealService::get_instance()->hooks->hook_map["MovePlayer"]->original(MovePlayer)(t, u, ent);
}

//int GetTime()
//{
//	static int prev_time = ZealService::get_instance()->hooks->hook_map["GetTime"]->original(GetTime)();
//	int cur_time = ZealService::get_instance()->hooks->hook_map["GetTime"]->original(GetTime)();
//	int rval = cur_time;
//	if (ZealService::get_instance()->physics->movement())
//	{
//		Zeal::EqGame::print_chat("%i", cur_time - prev_time);
//		if (cur_time - prev_time == 0)
//			rval = cur_time + 1;
//		prev_time = cur_time;
//
//	}
//
//	return rval;
//}

Physics::Physics(ZealService* zeal, IO_ini* ini)
{
	zeal->hooks->Add("ProcessPhysics", 0x54D964, ProcessPhysics, hook_type_detour);
	zeal->hooks->Add("MovePlayer", 0x504765, MovePlayer, hook_type_detour);
	//zeal->hooks->Add("GetTime", 0x54dbad, GetTime, hook_type_replace_call);
}

Physics::~Physics()
{
}