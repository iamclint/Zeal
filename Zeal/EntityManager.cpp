#include "EntityManager.h"
#include "Zeal.h"
#include "EqAddresses.h"


void EntityManager::Add(struct Zeal::EqStructures::Entity* ent)
{
	if (ent->Type == Zeal::EqEnums::EntityTypes::Player)
	{
		player_map[ent->Name] = ent;
		//Zeal::EqGame::print_chat("Adding player: %s count: %i", ent->Name, player_map.size());
	}
}

void EntityManager::Remove(struct Zeal::EqStructures::Entity* ent)
{
	if (ent->Type==Zeal::EqEnums::EntityTypes::Player)
	{
		player_map.erase(ent->Name);
		//Zeal::EqGame::print_chat("Removing player: %s count: %i", ent->Name, player_map.size());
	}
}

EntityManager::EntityManager(class ZealService* zeal, class IO_ini* ini)
{
	zeal->callbacks->AddEntity([this](struct Zeal::EqStructures::Entity* ent) { Remove(ent); }, callback_type::EntityDespawn );
	zeal->callbacks->AddEntity([this](struct Zeal::EqStructures::Entity* ent) { Add(ent); }, callback_type::EntitySpawn);
}
EntityManager::~EntityManager()
{

}
