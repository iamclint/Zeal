#pragma once
#include "hook_wrapper.h"
#include "EqStructures.h"
#include <stdint.h>
#include <unordered_map>
#include <string>
#include <vector>

class EntityManager
{
public:
	EntityManager(class ZealService* zeal);
	~EntityManager();
	void Add(struct Zeal::EqStructures::Entity*);
	void Remove(struct Zeal::EqStructures::Entity*);
	Zeal::EqStructures::Entity* Get(std::string name) const;  // Returns nullptr if not found.
	Zeal::EqStructures::Entity* Get(WORD id) const;  // Note: Equivalent to EqGame::get_entity_by_id()
	std::vector<std::string> GetPlayerPartialMatches(const std::string& start_of_name) const;
	void Dump() const;

private:
	std::unordered_map<std::string, struct Zeal::EqStructures::Entity*> entity_map;
};

