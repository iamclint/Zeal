#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"

struct DamageData
{
	ULONGLONG start_time;
	ULONGLONG last_tick;
	float y_offset;
	float x_offset;
	bool needs_removed;
	float opacity;
	void tick();
	bool is_my_damage;
	bool is_spell;
	std::string str_dmg;
	int damage;
	int heal;
	DamageData(int dmg, bool _is_my_damage_dealt, bool _is_spell, int _heal);
};

class FloatingDamage
{
public:
	void add_damage(int* dmg, int heal);
	void callback_deferred();
	void set_enabled(bool enable);
	bool enabled;
	FloatingDamage(class ZealService* zeal, class IO_ini* ini);
	~FloatingDamage();
private:
	int font_size = 5;
	std::unordered_map<Zeal::EqStructures::Entity*, std::vector<DamageData>> damage_numbers;
};

