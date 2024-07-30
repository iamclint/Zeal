#include "physics.h"
#include "Zeal.h"
#include "EqAddresses.h"

void ProcessPhysics(Zeal::EqStructures::Entity* ent, int missile, int effect)
{
	if (ent && ent->ActorInfo && missile==0 && ent==Zeal::EqGame::get_self())
	{
		static int prev_time = Zeal::EqGame::get_eq_time();
		int time_diff = Zeal::EqGame::get_eq_time() - prev_time;
		prev_time = Zeal::EqGame::get_eq_time();
		if (Zeal::EqGame::get_eq_time() - ent->ActorInfo->PhysicsTimer >= 16)
		{
			ZealService::get_instance()->hooks->hook_map["ProcessPhysics"]->original(ProcessPhysics)(ent, missile, effect);
		}
		//This frametime calculation is done inside process physics but since we are limiting how often its called we need to fix it up
		*(float*)0x7D01DC = (float)time_diff * 0.02f;
		return;
	}
	else
	{
		ZealService::get_instance()->hooks->hook_map["ProcessPhysics"]->original(ProcessPhysics)(ent, missile, effect);
	}
}

bool Physics::can_move(short spawn_id)
{
	bool move = false;
	if (move_timers.count(spawn_id) > 0)
	{
		move = Zeal::EqGame::get_eq_time() - move_timers[spawn_id] >= 16;
	}
	else
	{
		move_timers[spawn_id] = Zeal::EqGame::get_eq_time();
	}

	if (move)
		move_timers[spawn_id] = Zeal::EqGame::get_eq_time();

	return move;
}

int __fastcall MovePlayer(int t, int u, Zeal::EqStructures::Entity* ent)
{
	if (!ent)
		return 1;
	if (ZealService::get_instance()->physics->can_move(ent->SpawnId))
	{
		return ZealService::get_instance()->hooks->hook_map["MovePlayer"]->original(MovePlayer)(t, u, ent);
	}
	else
		return 1;
}

Physics::Physics(ZealService* zeal, IO_ini* ini)
{
	zeal->callbacks->AddGeneric([this]() { move_timers.clear(); }, callback_type::Zone);

	zeal->hooks->Add("ProcessPhysics", 0x54D964, ProcessPhysics, hook_type_detour);
	zeal->hooks->Add("MovePlayer", 0x504765, MovePlayer, hook_type_detour);
	//zeal->hooks->Add("GetTime", 0x54dbad, GetTime, hook_type_replace_call);
}

Physics::~Physics()
{
}