#pragma once
#include "hook_wrapper.h"
#include "EqStructures.h"
#include <stdint.h>
#include <unordered_map>
#include <string>

class EntityManager
{
public:
	EntityManager(class ZealService* zeal, class IO_ini* ini);
	~EntityManager();
	void Add(struct Zeal::EqStructures::Entity*);
	void Remove(struct Zeal::EqStructures::Entity*);
	Zeal::EqStructures::Entity* GetPlayer(std::string name) const;  // Returns nullptr if not found.
	Zeal::EqStructures::Entity* GetPet(std::string name) const;
	Zeal::EqStructures::Entity* GetNPC(std::string name) const;
	void Dump() const;

private:
	std::unordered_map<std::string, struct Zeal::EqStructures::Entity*> player_map;
	std::unordered_map<std::string, struct Zeal::EqStructures::Entity*> pet_map;
	std::unordered_map<std::string, struct Zeal::EqStructures::Entity*> npc_map;
};

