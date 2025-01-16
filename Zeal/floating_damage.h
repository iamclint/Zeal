#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "d3dx8/d3d8.h"
#include "bitmap_font.h"
#include "ZealSettings.h"

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
	static constexpr char kUseClientFontString[] = "Use client /fcd #";

	void add_damage(struct Zeal::EqStructures::Entity* source, struct  Zeal::EqStructures::Entity* target, short dmg, int heal, short spell_id);
	void callback_deferred();
	void callback_render();
	ZealSetting<bool> enabled = { true, "FloatingDamage", "Enabled", true };
	ZealSetting<bool> spell_icons = { true, "FloatingDamage", "Icons", true };
	ZealSetting<bool> spells = { true, "FloatingDamage", "Spells", true };
	ZealSetting<std::string> bitmap_font_filename = { std::string(kUseClientFontString),
		"FloatingDamage", "Font", true, [this](std::string val) { bitmap_font.reset(); } };
	bool set_font(std::string font_name);
	std::vector<std::string> get_available_fonts() const;
	void init_ui();
	void clean_ui();
	void draw_icon(int index, float x, float y, float opacity);
	int get_active_damage_count(Zeal::EqStructures::Entity* ent);
	FloatingDamage(class ZealService* zeal, class IO_ini* ini);
	~FloatingDamage();
private:

	bool add_texture(std::string path);
	std::vector<IDirect3DTexture8*> textures;
	IDirect3DTexture8* load_texture(std::string path);
	void load_bitmap_font();
	void render_spells();
	void render_text();  // Called in "render" for bitmap_font or "deferred" for CTextureFont.
	std::unique_ptr<BitmapFont> bitmap_font = nullptr;
	int font_size = 5;
	std::unordered_map<Zeal::EqStructures::Entity*, std::vector<DamageData>> damage_numbers;
};

