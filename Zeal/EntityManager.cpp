#include "EntityManager.h"
#include "Zeal.h"
#include "EqAddresses.h"


void EntityManager::Add(struct Zeal::EqStructures::Entity* ent)
{
	if (ent->Type == Zeal::EqEnums::EntityTypes::Player)
	{
		player_map[ent->Name] = ent;
	}
	else if (ent->PetOwnerSpawnId)
	{
		pet_map[ent->Name] = ent;
	}
	else if (ent->Type == Zeal::EqEnums::EntityTypes::NPC)
	{
		npc_map[ent->Name] = ent;
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

Zeal::EqStructures::Entity* EntityManager::GetPet(std::string name) const {
	auto it = pet_map.find(name);
	return (it == pet_map.end()) ? nullptr : it->second;
}
Zeal::EqStructures::Entity* EntityManager::GetNPC(std::string name) const {
	auto it = npc_map.find(name);
	return (it == npc_map.end()) ? nullptr : it->second;
}
Zeal::EqStructures::Entity* EntityManager::GetPlayer(std::string  name) const {
	auto it = player_map.find(name);
	return (it == player_map.end()) ? nullptr : it->second;
}

void EntityManager::Dump() const {
	for (const auto& [key, value] : player_map)
		Zeal::EqGame::print_chat("name: %s, entity: 0x%08x", key.c_str(), reinterpret_cast<uint32_t>(value));
}

EntityManager::EntityManager(class ZealService* zeal, class IO_ini* ini)
{
	zeal->callbacks->AddEntity([this](struct Zeal::EqStructures::Entity* ent) { Remove(ent); }, callback_type::EntityDespawn );
	zeal->callbacks->AddEntity([this](struct Zeal::EqStructures::Entity* ent) { Add(ent); }, callback_type::EntitySpawn);
}
EntityManager::~EntityManager()
{

}
