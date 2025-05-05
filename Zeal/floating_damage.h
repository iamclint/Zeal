#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqPackets.h"
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
	std::string str_dmg;
	Zeal::EqStructures::SPELL* spell;
	D3DCOLOR color;
	bool highlight;

	// Damage is positive, heals are negative. Percent adds a % suffix.
	DamageData(int dmg, bool percent, Zeal::EqStructures::SPELL* spell, D3DCOLOR color, bool highlight);
};

class FloatingDamage
{
public:
	static constexpr char kUseClientFontString[] = "Use client /fcd #";

	void add_damage(Zeal::EqStructures::Entity* source, Zeal::EqStructures::Entity* target, WORD type,
		short spell_id, short damage, char output_text);
	void handle_hp_update_packet(const Zeal::Packets::SpawnHPUpdate_Struct* packet);
	void callback_deferred();
	void callback_render();
	ZealSetting<bool> enabled = { true, "FloatingDamage", "Enabled", true };
	ZealSetting<bool> hide_with_gui = { false, "FloatingDamage", "HideWithGui", true };
	ZealSetting<bool> spell_icons = { true, "FloatingDamage", "Icons", true };
	ZealSetting<bool> show_spells = { true, "FloatingDamage", "Spells", true };
	ZealSetting<bool> show_melee = { true, "FloatingDamage", "Melee", true };
	ZealSetting<bool> show_self = { true, "FloatingDamage", "Self", true };
	ZealSetting<bool> show_pets = { true, "FloatingDamage", "Pets", true };
	ZealSetting<bool> show_others = { true, "FloatingDamage", "Others", true };
	ZealSetting<bool> show_npcs = { true, "FloatingDamage", "Npcs", true };
	ZealSetting<bool> show_hp_updates = { true, "FloatingDamage", "ShowHpUpdates", true };
	ZealSetting<int> big_hit_threshold = { 100, "FloatingDamage", "BigHitThreshold", true };
	ZealSetting<std::string> bitmap_font_filename = { std::string(kUseClientFontString),
		"FloatingDamage", "Font", true, [this](std::string val) { bitmap_font.reset(); } };
	std::vector<std::string> get_available_fonts() const;
	void init_ui();
	void clean_ui();
	void draw_icon(int index, float x, float y, float opacity);
	int get_active_damage_count(Zeal::EqStructures::Entity* ent);
	FloatingDamage(class ZealService* zeal, class IO_ini* ini);
	~FloatingDamage();
private:

	bool is_visible() const;
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

