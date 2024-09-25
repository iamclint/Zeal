#include "EntityManager.h"
#include "Zeal.h"
#include "EqAddresses.h"


void EntityManager::Add(struct Zeal::EqStructures::Entity* ent)
{
	entity_map[ent->Name] = ent;
	entity_id_map[ent->SpawnId] = ent;
}

void EntityManager::Remove(struct Zeal::EqStructures::Entity* ent)
{
	entity_map.erase(ent->Name);
	entity_id_map.erase(ent->SpawnId);
}

Zeal::EqStructures::Entity* EntityManager::Get(std::string  name) const {
	auto it = entity_map.find(name);
	return (it == entity_map.end()) ? nullptr : it->second;
}
Zeal::EqStructures::Entity* EntityManager::Get(WORD id) const
{
	auto it = entity_id_map.find(id);
	return (it == entity_id_map.end()) ? nullptr : it->second;
}

void EntityManager::Dump() const {
	for (const auto& [key, value] : entity_map)
	{
		if (value->Type==Zeal::EqEnums::EntityTypes::Player)
			Zeal::EqGame::print_chat("name: %s, entity: 0x%08x", key.c_str(), reinterpret_cast<uint32_t>(value));
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
