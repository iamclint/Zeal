#include "EntityManager.h"
#include "Zeal.h"
#include "EqAddresses.h"


void EntityManager::Add(struct Zeal::EqStructures::Entity* ent)
{
	entity_map[ent->Name] = ent;
	entity_id_map[ent->SpawnId] = ent;

	Zeal::EqStructures::Entity* self = Get(0);
	if (self && self->SpawnId != 0)
	{
		entity_id_map[self->SpawnId] = self;
		entity_id_map.erase(0);
	}
}

void EntityManager::Remove(struct Zeal::EqStructures::Entity* ent)
{
	// The EQ game renames the ent->Name field (like _corpse0 suffix), so
	// ent->Name is not a reliable key to locate the hash entity to remove.
	// Removes are infrequent operations, so we can afford a linear search.
	for (auto it = entity_map.begin(); it != entity_map.end(); ++it)
	{
		if (it->second == ent) {
			entity_id_map.erase(it->second->SpawnId);
			entity_map.erase(it);  // Ignoring updated iterator.
			return;
		}
	}
}

Zeal::EqStructures::Entity* EntityManager::Get(std::string  name) const {
	if (!Zeal::EqGame::is_in_game())
		return nullptr;
	auto it = entity_map.find(name);
	return (it == entity_map.end()) ? nullptr : it->second;
}
Zeal::EqStructures::Entity* EntityManager::Get(WORD id) const
{
	if (!Zeal::EqGame::is_in_game())
		return nullptr;
	auto it = entity_id_map.find(id);
	return (it == entity_id_map.end()) ? nullptr : it->second;
}

// Local Dump() helper that confirms there isn't a mismatch of a hashmap key/value pair.
static bool is_valid_entity(const std::string& name, struct Zeal::EqStructures::Entity* entity) {
	Zeal::EqStructures::Entity* current = Zeal::EqGame::get_entity_list();

	while (current != nullptr) {
		if (current == entity) {
			if (name != current->Name) {
				Zeal::EqGame::print_chat("MISMATCH: entity name: %s, manager name: %s", current->Name, name.c_str());
				return false;
			}
			return true;
		}
		current = current->Next;
	}

	return false;
}

// Local Dump() helper that just counts up the number of EQ game entities.
static int get_num_game_entities() {
	int count = 0;
	Zeal::EqStructures::Entity* current = Zeal::EqGame::get_entity_list();
	while (current != nullptr) {
		count++;
		current = current->Next;
	}
	return count;
}

void EntityManager::Dump() const {

	Zeal::EqGame::print_chat("Num EQ entities: %i, Num in manager: %i, Num in spawn_id: %i",
		get_num_game_entities(), entity_map.size(), entity_id_map.size());

	for (const auto& [key, value] : entity_map)
	{
		// First do an integrity check to make sure the entity address is valid.
		if (!is_valid_entity(key, value))
			Zeal::EqGame::print_chat("INVALID: name: %s, entity: 0x%08x",
				key.c_str(), reinterpret_cast<uint32_t>(value));
		else if (value->Type == Zeal::EqEnums::EntityTypes::Player)
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
