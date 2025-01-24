#include "nameplate.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"

// Test cases:
// - Command line toggling of options and triggered options menu updates
// - Options menu toggling of options
// - Keybind toggling of options
// - Various states: AFK, LFG, role, anon
// - /showname command 0, 1, 2, 3 4
// - Options mixed with guilds, raids, and pets
// - Tab cycle targeting updates of text and tint

static float z_position_offset = 1.5f; // Static global to allow parse overrides during evaluation.

static void ChangeDagStringSprite(Zeal::EqStructures::EQDAGINFO* dag, int fontTexture, const char* str)
{
	reinterpret_cast<int(__thiscall*)(void* _this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, const char* text)>(0x4B0AA8)(*(void**)0x7F9510, dag, fontTexture, str);
}

static int __fastcall SetNameSpriteTint(void* this_display, void* not_used, Zeal::EqStructures::Entity* entity)
{
	if (ZealService::get_instance()->nameplate->handle_SetNameSpriteTint(entity))
		return 1;  // SetNameSpriteTint returns 1 if a tint was applied, 0 if not able to update.

	return ZealService::get_instance()->hooks->hook_map["SetNameSpriteTint"]->original(SetNameSpriteTint)(this_display, not_used, entity);
}

static int __fastcall SetNameSpriteState(void* this_display, void* unused_edx, Zeal::EqStructures::Entity* entity, int show)
{
	if (ZealService::get_instance()->nameplate->handle_SetNameSpriteState(this_display, entity, show))
		return 0;  // The callers of SetNameSpriteState do not check the result so just return 0.
	
	return ZealService::get_instance()->hooks->hook_map["SetNameSpriteState"]->original(SetNameSpriteState)(this_display, unused_edx, entity, show);
}

// Promotes a SetNameSpriteTint call to a SetNameSpriteState call (for faster target updates).
static int __fastcall SetNameSpriteTint_UpdateState(void* this_display, void* not_used, Zeal::EqStructures::Entity* entity)
{
	SetNameSpriteState(this_display, not_used, entity, 1);  // Calls SetNameSpriteTint internally.
	return 1;  // SetNameSpriteTint returns 1 if a tint was applied.
}


NamePlate::NamePlate(ZealService* zeal, IO_ini* ini)
{
	zeal->hooks->Add("SetNameSpriteState", 0x4B0BD9, SetNameSpriteState, hook_type_detour);
	zeal->hooks->Add("SetNameSpriteTint", 0x4B114D, SetNameSpriteTint, hook_type_detour);

	// Replace the tint only updates in RealRender_World with one that also updates the text
	// when there is a change in target. This processing happens shortly after the DoPassageOfTime()
	// processing where it normally happens, but that processing is gated by an update rate.
	zeal->hooks->Add("SetNameSpriteTint_UpdateState", 0x004aaff5, SetNameSpriteTint_UpdateState,
		hook_type_replace_call);  // Old target - New is null
	zeal->hooks->Add("SetNameSpriteTint_UpdateState", 0x004aafa5, SetNameSpriteTint_UpdateState,
		hook_type_replace_call);  // Old target - New not null
	zeal->hooks->Add("SetNameSpriteTint_UpdateState", 0x004aafba, SetNameSpriteTint_UpdateState,
		hook_type_replace_call);  // New target

	zeal->commands_hook->Add("/nameplate", {}, "Toggles nameplate options on/off.",
		[this](std::vector<std::string>& args) {
			parse_args(args);
			return true;
		});

	zeal->callbacks->AddGeneric([this]() { clean_ui(); }, callback_type::InitUI);  // Just release all resources.
	zeal->callbacks->AddGeneric([this]() { clean_ui(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { clean_ui(); }, callback_type::DXReset);  // Just release all resources.
	zeal->callbacks->AddGeneric([this]() { render_ui(); }, callback_type::RenderUI);
}

NamePlate::~NamePlate()
{
}



void NamePlate::parse_args(const std::vector<std::string>& args)
{
	static std::unordered_map<std::string, ZealSetting<bool>*> command_map = {
		{"colors", &setting_colors },
		{"concolors", &setting_con_colors },
		{"targetcolor", &setting_target_color },
		{"charselect", &setting_char_select },
		{"hideself", &setting_hide_self },
		{"x", &setting_x },
		{"hideraidpets", &setting_hide_raid_pets },
		{"showpetownername", &setting_show_pet_owner_name },
		{"targetmarker", &setting_target_marker },
		{"targethealth", &setting_target_health },
		{"targetblink", &setting_target_blink },
		{"attackonly", &setting_attack_only },
		{"inlineguild", &setting_inline_guild },
		{"zealfont", &setting_zeal_fonts },
		{"dropshadow", &setting_drop_shadow },
	};

	if (args.size() == 2 && args[1] == "dump") {
		if (sprite_font)
			sprite_font->dump();
		return;
	}
	if (args.size() == 3 && args[1] == "offset") {
		float offset;
		if (Zeal::String::tryParse(args[2], &offset)) {
			z_position_offset = max(-1.f, min(25.f, offset));
			return;
		}
	}

	if (args.size() == 2 && command_map.find(args[1]) != command_map.end())
	{
		auto setting = command_map[args[1]];
		setting->toggle();
		Zeal::EqGame::print_chat("Nameplate option %s set to %s", args[1].c_str(), setting->get() ? "Enabled" : "Disabled");
		if (ZealService::get_instance() && ZealService::get_instance()->ui
			&& ZealService::get_instance()->ui->options)
			ZealService::get_instance()->ui->options->UpdateOptionsNameplate();
		return;
	}

	Zeal::EqGame::print_chat("Usage: /nameplate option where option is one of");
	Zeal::EqGame::print_chat("tint:  colors, concolors, targetcolor, targetblink, attackonly, charselect");
	Zeal::EqGame::print_chat("text:  hideself, x, hideraidpets, showpetownername, targetmarker, targethealth, inlineguild");
	Zeal::EqGame::print_chat("font:  zealfont, dropshadow");
}

std::vector<std::string> NamePlate::get_available_fonts() const {
	return BitmapFont::get_available_fonts();  // Note that we customize the "default" one.
}

// Loads the sprite font for real-time text rendering to screen.
void NamePlate::load_sprite_font() {
	if (sprite_font || !setting_zeal_fonts.get())
		return;

	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	if (device == nullptr)
		return;

	std::string font_filename = setting_fontname.get();
	if (font_filename.empty() || font_filename == BitmapFont::kDefaultFontName)
		font_filename = "arial_bold_24";

	sprite_font = SpriteFont::create_sprite_font(*device, font_filename);
	if (!sprite_font)
		return;  // Let caller deal with the failure.
	sprite_font->set_drop_shadow(setting_drop_shadow.get());
	sprite_font->set_align_bottom(true);  // Bottom align for multi-line and font sizes.
}

void NamePlate::clean_ui()
{
	nameplate_info_map.clear();
	sprite_font.reset(); // Relying on spritefont destructor to be invoked to release resources.
}

// Approximation for the client behavior. Exact formula is unknown.
static float get_nameplate_z_offset(const Zeal::EqStructures::Entity& entity)
{
	return z_position_offset;

#if 0  // Adonis formula (before fix to bottom alignment).
	const float scale_factor = (entity.ActorInfo && entity.ActorInfo->ViewActor_) ?
		entity.ActorInfo->ViewActor_->ScaleFactor : 1.f;
	const float base_offset = entity.Height + entity.ModelHeightOffset;

	if (scale_factor > 3)
		return base_offset * 0.08f;
	if (scale_factor > 2)
		return base_offset * 0.1f;
	if (scale_factor > 1)
		return base_offset * 0.11f;
	if (scale_factor > 0.8)
		return base_offset * 0.12f;

	return base_offset * 0.14f;
#endif

}


void NamePlate::render_ui()
{
	if (!setting_zeal_fonts.get() || Zeal::EqGame::get_gamestate() != GAMESTATE_INGAME)
		return;

	if (!sprite_font)
		load_sprite_font();
	if (!sprite_font)
	{
		Zeal::EqGame::print_chat("Nameplate: Failed to load zeal fonts, disabling");
		setting_zeal_fonts.set(false, false);  // Fallback to native nameplates.
		if (ZealService::get_instance() && ZealService::get_instance()->ui
			&& ZealService::get_instance()->ui->options)
			ZealService::get_instance()->ui->options->UpdateOptionsNameplate();
		return;
	}

	// Go through world visible list. 
	const float kMaxDist = 400; // Quick testing of client extended nameplates was ~ 375.
	auto visible_entities = Zeal::EqGame::get_world_visible_actor_list(kMaxDist, false);
	auto self = Zeal::EqGame::get_self();
	if (self && *Zeal::EqGame::camera_view != Zeal::EqEnums::CameraView::FirstPerson &&
		!Zeal::EqGame::EqGameInternal::is_invisible(self, self))
		visible_entities.push_back(self);  // Add self nameplate.

	std::vector<RenderInfo> render_list;
	for (const auto& entity : visible_entities)
	{
		// Added Unknown0003 check due to some bad results with 0x05 at startup causing a crash.
		if (!entity || entity->StructType != 0x03 || !entity->ActorInfo || !entity->ActorInfo->DagHeadPoint)
			continue;
		auto it = nameplate_info_map.find(entity);
		if (it == nameplate_info_map.end())
			continue;

		NamePlateInfo& info = it->second;
		Vec3 position = entity->ActorInfo->DagHeadPoint->Position;
		position.z += get_nameplate_z_offset(*entity);
		sprite_font->queue_string(info.text.c_str(), position, true, info.color | 0xff000000);
	}
	sprite_font->flush_queue_to_screen();
}


// Helper function for selecting the player color.
NamePlate::ColorIndex NamePlate::get_player_color_index(const Zeal::EqStructures::Entity& entity) const
{
	if (entity.IsPlayerKill == 1)
		return ColorIndex::PVP;

	if (entity.IsAwayFromKeyboard == 1)
		return ColorIndex::AFK;

	if (entity.IsLinkDead == 1)
		return ColorIndex::LD;

	if (entity.ActorInfo && entity.ActorInfo->IsLookingForGroup)
		return ColorIndex::LFG;

	if (Zeal::EqGame::GroupInfo->is_in_group())
	{
		if (&entity == Zeal::EqGame::get_self())
			return ColorIndex::Group;

		for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; i++)
		{
			Zeal::EqStructures::Entity* groupmember = Zeal::EqGame::GroupInfo->EntityList[i];
			if (groupmember && &entity == groupmember)
				return ColorIndex::Group;
		}
	}

	if (Zeal::EqGame::RaidInfo->is_in_raid())
	{
		const char* spawn_name = entity.Name;
		for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i)
		{
			const Zeal::EqStructures::RaidMember& member = Zeal::EqGame::RaidInfo->MemberList[i];
			if ((strlen(member.Name) != 0) && (strcmp(member.Name, entity.Name) == 0))
				return ColorIndex::Raid;
		}
	}

	if (entity.AnonymousState == 2) // Roleplay
		return ColorIndex::Role;

	if (entity.GuildId == -1)  // Not in a guild.
		return ColorIndex::Adventurer;

	return (entity.GuildId == Zeal::EqGame::get_self()->GuildId) ? ColorIndex::MyGuild : ColorIndex::OtherGuild;
}

// Internal helper for retrieving the color for a pet (for raid or group).
NamePlate::ColorIndex NamePlate::get_pet_color_index(const Zeal::EqStructures::Entity& entity) const
{
	if (entity.PetOwnerSpawnId == Zeal::EqGame::get_self()->SpawnId) // Self Pet
		return ColorIndex::Group;  // Always a group member.

	if (Zeal::EqGame::GroupInfo->is_in_group())
	{
		for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; i++)
		{
			Zeal::EqStructures::Entity* groupmember = Zeal::EqGame::GroupInfo->EntityList[i];
			if (groupmember && entity.PetOwnerSpawnId == groupmember->SpawnId)
				return ColorIndex::Group;
		}
	}

	if (Zeal::EqGame::is_raid_pet(entity))
		return ColorIndex::Raid;

	return ColorIndex::UseClient;
}

NamePlate::ColorIndex NamePlate::get_color_index(const Zeal::EqStructures::Entity& entity)
{
	if (!entity.ActorInfo)
		return ColorIndex::UseClient;

	// Special handling for character select.
	if (Zeal::EqGame::get_gamestate() == GAMESTATE_CHARSELECT)
		return setting_char_select.get() ? ColorIndex::Adventurer : ColorIndex::UseClient;

	// Target setting overrides all other choices.
	if (&entity == Zeal::EqGame::get_target() && setting_target_color.get())
		return ColorIndex::Target;

	// Otherwise tint based on entity Type and other properties.
	auto color_index = ColorIndex::UseClient;
	switch (entity.Type) {
	case Zeal::EqEnums::EntityTypes::Player:
		if (setting_colors.get())
			color_index = get_player_color_index(entity);
		else if (setting_con_colors.get() && &entity != Zeal::EqGame::get_self())
			color_index = ColorIndex::UseConsider;
		break;
	case Zeal::EqEnums::EntityTypes::NPC:
		if (setting_colors.get() && entity.PetOwnerSpawnId)
			color_index = get_pet_color_index(entity);
		if (color_index == ColorIndex::UseClient && setting_con_colors.get())
			color_index = ColorIndex::UseConsider;
		break;
	case Zeal::EqEnums::EntityTypes::NPCCorpse:
		if (setting_colors.get())
			color_index = ColorIndex::NpcCorpse;
		break;
	case Zeal::EqEnums::EntityTypes::PlayerCorpse:
		if (setting_colors.get())
			color_index = ColorIndex::PlayerCorpse;
		break;
	default:
		break;
	}

	return color_index;
}

bool NamePlate::handle_SetNameSpriteTint(Zeal::EqStructures::Entity* entity)
{
	if (!entity || !entity->ActorInfo || !entity->ActorInfo->DagHeadPoint)
		return false;

	auto color_index = get_color_index(*entity);

	bool zeal_fonts = setting_zeal_fonts.get() && Zeal::EqGame::get_gamestate() != GAMESTATE_CHARSELECT;
	if (color_index == ColorIndex::UseClient && !zeal_fonts)
		return false;

	auto color = D3DCOLOR_XRGB(128, 255, 255);  // Approximately the default nameplate color.
	if (color_index == ColorIndex::UseConsider)
		color = Zeal::EqGame::GetLevelCon(entity);
	else if (color_index != ColorIndex::UseClient)
		color = ZealService::get_instance()->ui->options->GetColor(static_cast<int>(color_index));

	if (entity == Zeal::EqGame::get_target() && setting_target_blink.get())
	{
		// Share the flash speed slider with the target_ring so they aren't beating.
		float flash_speed = ZealService::get_instance()->target_ring->flash_speed.get();
		float fade_factor = Zeal::EqGame::get_target_blink_fade_factor(flash_speed,
			setting_attack_only.get());
		if (fade_factor < 1.0f) {
			BYTE faded_red = static_cast<BYTE>(((color >> 16) & 0xFF) * fade_factor);
			BYTE faded_green = static_cast<BYTE>(((color >> 8) & 0xFF) * fade_factor);
			BYTE faded_blue = static_cast<BYTE>((color & 0xFF) * fade_factor);
			color = D3DCOLOR_ARGB(0xff, faded_red, faded_green, faded_blue);
		}
	}

	if (zeal_fonts) {
		auto it = nameplate_info_map.find(entity);
		if (it != nameplate_info_map.end())
			it->second.color = color;
		return true;
	}

	if (!entity->ActorInfo->DagHeadPoint->StringSprite ||
		entity->ActorInfo->DagHeadPoint->StringSprite->MagicValue != Zeal::EqStructures::EQSTRINGSPRITE::kMagicValidValue)
		return false;
	entity->ActorInfo->DagHeadPoint->StringSprite->Color = color;
	return true;
}

// Implements the racial specific hidden nameplates. Logic copied from the client disassembly.
bool NamePlate::is_nameplate_hidden_by_race(const Zeal::EqStructures::Entity& entity) const
{
	if (entity.Type == Zeal::EqEnums::Player)  // Never hide the player by race.
		return false;

	// Zeal modification: Never hide corpse nameplates based on race.
	if (entity.Type == Zeal::EqEnums::NPCCorpse || entity.Type == Zeal::EqEnums::PlayerCorpse)
		return false;

	// Zeal modification: Never hide the current target nameplate (ie, skelly on the ground).
	if (&entity == Zeal::EqGame::get_target())
		return false;

	if (entity.Race == 0xf4)  // 0xf4 = "Ent"
		return true;

	auto animation = entity.ActorInfo->Animation;
	if (entity.Race == 0x3c)  // 0x3c = "Skeleton"
		return !entity.PetOwnerSpawnId && (animation == 0x10 || animation == 0x21 || animation == 0x26);

	// 0x1d = "Gargoyle", 0x34 = "Mimic", 0x118 = "Nightmare Gargoyle"
	if (entity.Race == 0x1d || entity.Race == 0x34 || entity.Race == 0x118)
		return (animation == 0x21 || animation == 0x26);

	return false;
}

// Handles the target health and target markers at the end of the name.
std::string NamePlate::generate_target_postamble(const Zeal::EqStructures::Entity& entity) const
{
	std::string text;

	if (setting_target_health.get())
	{
		if (entity.Type == Zeal::EqEnums::NPC)
		{
			int hp_percent = entity.HpCurrent;	// NPC value is stored as a percent.
			text += std::format(" {}%", hp_percent);
		}
		else if (entity.Type == Zeal::EqEnums::Player && entity.HpCurrent > 0 && entity.HpMax > 0)
		{
			int hp_percent = (entity.HpCurrent * 100) / entity.HpMax; // Calculate % health.
			text += std::format(" {}%", hp_percent);
		}
	}
	if (setting_target_marker.get())
		text += "<<";
	return text;
}


// Duplicates most of the client logic in SetNameSpriteState except for the unused return values.
std::string NamePlate::generate_nameplate_text(const Zeal::EqStructures::Entity& entity, int show) const
{
	// Handle some of the always disabled nameplates.
	if (is_nameplate_hidden_by_race(entity))
		return std::string(); // Returns empty text.

	// Handle character select formatted output when active.
	if (Zeal::EqGame::is_new_ui() && Zeal::EqGame::Windows->CharacterSelect &&
		Zeal::EqGame::Windows->CharacterSelect->Activated) {
		return std::format("{} [{} {}]\n{}",
			entity.Name,
			entity.Level,
			Zeal::EqGame::get_class_desc(entity.Class),
			Zeal::EqGame::get_full_zone_name(entity.ZoneId));
	}

	// Handle other reasons for disabled nameplates.
	const uint32_t show_name = Zeal::EqGame::get_showname();
	if ((show == 0) || (show_name < 1 && (entity.Type != Zeal::EqEnums::NPC)))
		return std::string();

	const bool is_self = (&entity == Zeal::EqGame::get_self());
	const bool is_target = (&entity == Zeal::EqGame::get_target());
	if (!is_target && ((is_self && setting_hide_self.get())
		 || (setting_hide_raid_pets.get() && Zeal::EqGame::is_raid_pet(entity))))
		return std::string();

	if (is_self && setting_x.get())
		return std::string((entity.IsHidden == 0x01) ? "(X)" : "X");

	if (entity.Race >= 0x8cd)  // Some sort of magic higher level races w/out name trimming.
		return std::string(entity.Name);

	std::string text;
	if (is_target && setting_target_marker.get())
		text += ">>";

	// Handle the simpler NPC and corpses first.
	if (entity.Type != Zeal::EqEnums::Player) {
		text += std::string(Zeal::EqGame::trim_name(entity.Name));
		if (is_target)
			text += generate_target_postamble(entity);
		if (setting_show_pet_owner_name.get() && Zeal::EqGame::is_player_pet(entity))
		{
			auto pet_owner = Zeal::EqGame::get_entity_by_id(entity.PetOwnerSpawnId);
			if (pet_owner && pet_owner != Zeal::EqGame::get_self())
				text += std::format("\n({}'s Pet)", Zeal::EqGame::trim_name(pet_owner->Name));
			else
				text += "\n(Pet)";  // Self-pet or missing owner.
		}
		return text;
	}

	if (entity.ActorInfo->IsTrader == 1 && Zeal::EqGame::get_self() &&
		Zeal::EqGame::get_self()->ZoneId == 0x97)
		text += "Trader ";  // String id 0x157f.

	else if (entity.AlternateAdvancementRank > 0 && entity.Gender != 2 && show_name > 3) {
		text += Zeal::EqGame::get_title_desc(entity.Class, entity.AlternateAdvancementRank, entity.Gender);
		text += " ";
	}

	// Finally work on the primary player name with embellishments.
	if (entity.IsHidden == 0x01)  // Client code only does () on normal invisibility.
		text += "(";
	text += Zeal::EqGame::trim_name(entity.Name);
	if (entity.IsHidden == 0x01)
		text += ")";

	if (show_name > 1 && entity.LastName[0]) {
		text += " ";
		text += Zeal::EqGame::trim_name(entity.LastName);
	}

	const bool is_anonymous = (entity.AnonymousState == 1) ? true : false;
	const bool show_guild = !is_anonymous && show_name > 2 && entity.GuildId != -1;
	const bool show_guild_newline = Zeal::EqGame::is_new_ui() && !setting_inline_guild.get();
	if (show_guild && !show_guild_newline)
		text += std::format(" <{}>", Zeal::EqGame::get_player_guild_name(entity.GuildId));

	if (entity.ActorInfo->IsLookingForGroup)
		text += " LFG";  // String id 0x301a.
	if (!entity.IsPlayerKill) {
		if (entity.IsAwayFromKeyboard)
			text += " AFK"; // String id 0x3017.
		if (entity.IsLinkDead)
			text += " LD"; // String id 0x8c0.
	}
	if (is_target)
		text += generate_target_postamble(entity);
	if (show_guild && show_guild_newline)
		text += std::format("\n<{}>", Zeal::EqGame::get_player_guild_name(entity.GuildId));

	return text;
}

// Returns true if it updated the nameplate state. False if the default code needs to run.
bool NamePlate::handle_SetNameSpriteState(void* this_display, Zeal::EqStructures::Entity* entity, int show)
{
	// Note: The this_display pointer should be equal to *Zeal::EqGame::Display.
	if (!entity || !entity->ActorInfo || !entity->ActorInfo->DagHeadPoint)
		return false;  // Note: Possibly change to true to avoid client handler from running.

	// Let the default client path handle things when the world display isn't active. That code
	// will handle any needed deallocations.
	int world_display_started = *(int*)((int)this_display + 0x2ca4);  // Set in CDisplay::StartWorldDisplay().
	int font_texture = *(int*)((int)this_display + 0x2e08);
	if (!world_display_started || !font_texture)
		return false;

	std::string text = generate_nameplate_text(*entity, show);
	const char* string_sprite_text = text.c_str();
	if (setting_zeal_fonts.get() && Zeal::EqGame::get_gamestate() != GAMESTATE_CHARSELECT) {
		if (!text.empty())
			nameplate_info_map[entity] = { .text = text, .color = D3DCOLOR_XRGB(255,255,255)};
		else {
			auto it = nameplate_info_map.find(entity);
			if (it != nameplate_info_map.end())
				nameplate_info_map.erase(it);
		}
		string_sprite_text = nullptr;  // This disables the client's sprite in call below.
	}

	ChangeDagStringSprite(entity->ActorInfo->DagHeadPoint, font_texture, string_sprite_text);

	if (!text.empty())
		SetNameSpriteTint(this_display, nullptr, entity);
	return true;

}
