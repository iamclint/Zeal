#pragma once
#include "hook_wrapper.h"
#include "EqStructures.h"
#include <stdint.h>
#include <unordered_map>
class EntityManager
{
public:
	std::unordered_map<const char*, struct Zeal::EqStructures::Entity*> player_map;
	EntityManager(class ZealService* zeal, class IO_ini* ini);
	~EntityManager();
	void Add(struct Zeal::EqStructures::Entity*);
	void Remove(struct Zeal::EqStructures::Entity*);
};

