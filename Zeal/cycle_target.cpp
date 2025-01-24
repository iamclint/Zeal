#include "cycle_target.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

static size_t last_index = -1;
static std::vector<Zeal::EqStructures::Entity*> near_ents;
bool compareDist(Zeal::EqStructures::Entity* i1, Zeal::EqStructures::Entity* i2)
{
	return (i1->Position.Dist(Zeal::EqGame::get_self()->Position) < i2->Position.Dist(Zeal::EqGame::get_self()->Position));
}

void AddIndex()
{
	last_index++;
	if (last_index >= near_ents.size())
		last_index = 0;
}
Zeal::EqStructures::Entity* CycleTarget::get_next_ent(float dist, byte type)
{
	static ULONGLONG last_press = 0;
	
	
	std::vector<Zeal::EqStructures::Entity*> visible_ents = Zeal::EqGame::get_world_visible_actor_list(dist, true);
	if (GetTickCount64() - last_press > 3000) //if you haven't pressed the cycle key in 3 seconds reset the index so it selected the nearest
		last_index = -1;
	last_press = GetTickCount64();


	if (!visible_ents.size())
		return 0;
	near_ents.clear();
	for (auto& ent : visible_ents)
	{
		if (ent->StructType != 0x03)
			continue;
		if (ent->Type == type && ent->Level > 0)
		{
			if (ent->PetOwnerSpawnId)
			{
				Zeal::EqStructures::Entity* owner = Zeal::EqGame::get_entity_by_id(ent->PetOwnerSpawnId);
				if ((owner && (owner->Type == Zeal::EqEnums::EntityTypes::NPC || owner->Type == Zeal::EqEnums::EntityTypes::NPCCorpse)) || !owner)
					near_ents.push_back(ent);
			}
			else
				near_ents.push_back(ent);
		}
	}
	if (!near_ents.size())
		return 0;

	std::sort(near_ents.begin(), near_ents.end(), compareDist);
	AddIndex();
	
	if (near_ents[last_index] && Zeal::EqGame::get_target() && Zeal::EqGame::get_target() == near_ents[last_index])
		AddIndex();

	if (near_ents[last_index])
		return near_ents[last_index];
	else
		return 0;

}

Zeal::EqStructures::Entity* CycleTarget::get_nearest_ent(float dist, byte type)
{
	static ULONGLONG last_press = 0;
	std::vector<Zeal::EqStructures::Entity*> visible_ents;
	if (type>0)
		visible_ents = Zeal::EqGame::get_world_visible_actor_list(dist, true);
	else
		visible_ents = Zeal::EqGame::get_world_visible_actor_list(dist, false);
	if (!visible_ents.size())
		return 0;
	near_ents.clear();
	for (auto& ent : visible_ents)
	{
		if (ent->StructType != 0x03)
			continue;
		if (ent->Type == type && ent->Level > 0)
		{
			if (ent->PetOwnerSpawnId)
			{
				Zeal::EqStructures::Entity* owner = Zeal::EqGame::get_entity_by_id(ent->PetOwnerSpawnId);
				if ((owner && (owner->Type == Zeal::EqEnums::EntityTypes::NPC || owner->Type == Zeal::EqEnums::EntityTypes::NPCCorpse)) || !owner)
					near_ents.push_back(ent);
			}
			else
				near_ents.push_back(ent);
		}
	}
	if (!near_ents.size())
		return 0;

	std::sort(near_ents.begin(), near_ents.end(), compareDist);
	AddIndex();

	if (near_ents.front())
		return near_ents.front();
	else
		return 0;

}

CycleTarget::~CycleTarget()
{
	//hook->remove(); //hooks being removed from dllmain
}

CycleTarget::CycleTarget(ZealService* zeal)
{
	//originally this used a hook and replaced target nearest but now uses a bind
	//	hook = zeal->hooks->Add("NearestEnt", Zeal::EqGame::EqGameInternal::fn_targetnearestnpc, get_nearest_ent, hook_type_detour, 6);
}
