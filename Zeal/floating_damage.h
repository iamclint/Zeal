#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "d3dx8/d3d8.h"
struct DamageData
{
	ULONGLONG start_time;
	ULONGLONG last_tick;
	float y_offset;
	float x_offset;
	bool needs_removed;
	float opacity;
	void tick(int active_number_count);
	bool is_my_damage;
	bool is_spell;
	std::string str_dmg;
	int damage;
	int heal;
	Zeal::EqStructures::SPELL* spell;
	DamageData(int dmg, bool _is_my_damage_dealt, bool _is_spell, int _heal, Zeal::EqStructures::SPELL* sp = nullptr);
};

class FloatingDamage
{
public:
	void add_damage(struct Zeal::EqStructures::Entity* source, struct  Zeal::EqStructures::Entity* target, short dmg, int heal, short spell_id);
	void callback_deferred();
	void callback_render();
	void set_enabled(bool enable);
	bool enabled;
	void init_ui();
	void draw_icon(int index, float x, float y, float opacity);
	int get_active_damage_count(Zeal::EqStructures::Entity* ent);
	FloatingDamage(class ZealService* zeal, class IO_ini* ini);
	~FloatingDamage();
private:
	bool add_texture(std::string path);
	std::vector<IDirect3DTexture8*> textures;
	IDirect3DTexture8* load_texture(std::string path);
	int font_size = 5;
	std::unordered_map<Zeal::EqStructures::Entity*, std::vector<DamageData>> damage_numbers;
};

