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

	// TODO: Add render callback hooks for the new bitmap fonts.
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
		{"targetmarker", &setting_target_marker },
		{"targethealth", &setting_target_health },
		{"inlineguild", &setting_inline_guild },
	};

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
	Zeal::EqGame::print_chat("tint:  colors, concolors, targetcolor, charselect");
	Zeal::EqGame::print_chat("text:  hideself, x, hideraidpets, targetmarker, targethealth, inlineguild");
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

	// Special handling for the current target since we want to leave the blinking indicator on it.
	if (&entity == Zeal::EqGame::get_target())
		return setting_target_color.get() ? ColorIndex::Target : ColorIndex::UseClient;

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
	if (color_index == ColorIndex::UseClient)
		return false;

	auto color = (color_index == ColorIndex::UseConsider) ? Zeal::EqGame::GetLevelCon(entity) :
		ZealService::get_instance()->ui->options->GetColor(static_cast<int>(color_index));

	// TODO: If bitmap font nameplates, store the color rendering and skip the update.
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

	if (entity.Race == 0xf4)
		return true;

	auto animation = entity.ActorInfo->Animation;
	if (entity.Race == 0x3c)
		return !entity.PetOwnerSpawnId && (animation == 0x10 || animation == 0x21 || animation == 0x26);

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
		return std::string((entity.IsHidden) ? "(X)" : "X");

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
	if (entity.IsHidden)
		text += "(";
	text += Zeal::EqGame::trim_name(entity.Name);
	if (entity.IsHidden)
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

	// TODO: If bitmap font nameplates, store the text for rendering and blank the StringSprite.
	std::string text = generate_nameplate_text(*entity, show);
	const char* sprite_text = text.empty() ? nullptr : text.c_str();
	ChangeDagStringSprite(entity->ActorInfo->DagHeadPoint, font_texture, sprite_text);
	if (!text.empty())
		SetNameSpriteTint(this_display, nullptr, entity);
	return true;

}
