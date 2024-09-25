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
	Zeal::EqStructures::Entity* Get(std::string name) const;  // Returns nullptr if not found.
	Zeal::EqStructures::Entity* Get(WORD id) const;
	void Dump() const;

private:
	std::unordered_map<std::string, struct Zeal::EqStructures::Entity*> entity_map;
	std::unordered_map<WORD, struct Zeal::EqStructures::Entity*> entity_id_map;
};

