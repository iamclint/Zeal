#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "ZealSettings.h"
#
#include "bitmap_font.h"

class NamePlate
{
public:
	// The positive color indices must be kept in sync with the color options.
	enum class ColorIndex : int
	{
		UseConsider = -2,	// Special internal signal to use consider level.
		UseClient = -1,		// Internal signal to use the client default color.
		AFK = 0,
		LFG = 1,
		LD = 2,
		MyGuild= 3,
		Raid = 4,
		Group = 5,
		PVP = 6,
		Role = 7,
		OtherGuild = 8,
		Adventurer = 9,
		NpcCorpse = 10,
		PlayerCorpse = 11,
		Target = 18,
	};

	NamePlate(class ZealService* zeal, class IO_ini* ini);
	~NamePlate();

	// Tint (color) settings.
	ZealSetting<bool> setting_colors = { false, "Zeal", "NameplateColors", false };
	ZealSetting<bool> setting_con_colors = { false, "Zeal", "NameplateConColors", false };
	ZealSetting<bool> setting_target_color = { false, "Zeal", "NameplateTargetColor", false };
	ZealSetting<bool> setting_char_select = { false, "Zeal", "NameplateCharSelect", false };

	// Text settings.
	ZealSetting<bool> setting_hide_self = { false, "Zeal", "NameplateHideSelf", false };
	ZealSetting<bool> setting_x = { false, "Zeal", "NameplateX", false };
	ZealSetting<bool> setting_hide_raid_pets = { false, "Zeal", "NameplateHideRaidPets", false };
	ZealSetting<bool> setting_show_pet_owner_name = { false, "Zeal", "NameplateShowPetOwnerName", false };
	ZealSetting<bool> setting_target_marker = { false, "Zeal", "NameplateTargetMarker", false };
	ZealSetting<bool> setting_target_health = { false, "Zeal", "NameplateTargetHealth", false };
	ZealSetting<bool> setting_target_blink = { true, "Zeal", "NameplateTargetBlink", false };
	ZealSetting<bool> setting_attack_only = { false, "Zeal", "NameplateAttackOnly", false };
	ZealSetting<bool> setting_inline_guild = { false, "Zeal", "NameplateInlineGuild", false };
	
	ZealSetting<bool> setting_extended_nameplate = { true, "Zeal", "NameplateExtended", false,
		[this](bool& val) { mem::write<BYTE>(0x4B0B3D, val ? 0 : 1); }, true };

	// Advanced fonts
	ZealSetting<bool> setting_health_bars = { false, "Zeal", "NameplateHealthBars", false };
	ZealSetting<bool> setting_mana_bars = { false, "Zeal", "NameplateManaBars", false };
	ZealSetting<bool> setting_stamina_bars = { false, "Zeal", "NameplateManaBars", false };
	ZealSetting<bool> setting_zeal_fonts = { false, "Zeal", "NamePlateZealFonts", false,
		[this](bool val) { clean_ui(); } };
	ZealSetting<bool> setting_drop_shadow = { false, "Zeal", "NamePlateDropShadow", false,
		[this](bool val) { clean_ui(); } };
	ZealSetting<float> setting_shadow_offset_factor = { BitmapFontBase::kDefaultShadowOffsetFactor,
		"Zeal", "NamePlateShadowOffsetFactor", false };
	ZealSetting<std::string> setting_fontname = { std::string(BitmapFont::kDefaultFontName),
		"Zeal", "NamePlateFontname", false, [this](std::string val) { clean_ui(); } };

	std::vector<std::string> get_available_fonts() const;

	// Internal use only (public for use by callbacks).
	bool handle_SetNameSpriteTint(Zeal::EqStructures::Entity* entity);
	bool handle_SetNameSpriteState(void* this_display, Zeal::EqStructures::Entity* entity, int show);

private:
	struct NamePlateInfo {
		std::string text;
		DWORD color;
	};

	struct RenderInfo {
		NamePlateInfo* info;
		float distance;
		Vec2 screen_xy;
	};

	void parse_args(const std::vector<std::string>& args);
	ColorIndex get_color_index(const Zeal::EqStructures::Entity& entity);
	ColorIndex get_player_color_index(const Zeal::EqStructures::Entity& entity) const;
	ColorIndex get_pet_color_index(const Zeal::EqStructures::Entity& entity) const;
	std::string generate_nameplate_text(const Zeal::EqStructures::Entity& entity, int show) const;
	std::string generate_target_postamble(const Zeal::EqStructures::Entity& entity) const;
	bool is_nameplate_hidden_by_race(const Zeal::EqStructures::Entity& entity) const;

	void clean_ui();
	void render_ui();
	void load_sprite_font();

	std::unique_ptr<SpriteFont> sprite_font;
	std::unordered_map<struct Zeal::EqStructures::Entity*, NamePlateInfo> nameplate_info_map;
};
