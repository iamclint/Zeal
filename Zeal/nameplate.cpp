#include "nameplate.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"

void NamePlate::HandleTint(Zeal::EqStructures::Entity* spawn)
{
	if (!spawn) { return; }
	if (!spawn->ActorInfo) { return; }
	if (!spawn->ActorInfo->DagHeadPoint) { return; }
	if (!spawn->ActorInfo->DagHeadPoint->StringSprite) { return; }
	ui_options* options = ZealService::get_instance()->ui->options.get();
	Zeal::EqStructures::EQARGBCOLOR LDcolor = options->GetColor(2); //0xFFFF0000; //LinkDead - Red
	Zeal::EqStructures::EQARGBCOLOR AFKcolor = options->GetColor(0); //0xFFFF8000; //AFK - Orange
	Zeal::EqStructures::EQARGBCOLOR PVPcolor = options->GetColor(6); //0xFFFF0000; //PVP - Red
	Zeal::EqStructures::EQARGBCOLOR LFGcolor = options->GetColor(1); //0xFFCFFF00; //LFG - Yellow
	Zeal::EqStructures::EQARGBCOLOR Groupcolor = options->GetColor(5); //0xFF00FF32; //Group Member - Light Green
	Zeal::EqStructures::EQARGBCOLOR Raidcolor = options->GetColor(4); //0xFFFFFFFF; //Raid Member - White Light Purple
	Zeal::EqStructures::EQARGBCOLOR Rolecolor = options->GetColor(7); //0xFF85489C; //Roleplay - Purple
	Zeal::EqStructures::EQARGBCOLOR MyGuildcolor = options->GetColor(3); //0xFFFF8080; //MyGuild Member - White Red
	Zeal::EqStructures::EQARGBCOLOR OtherGuildscolor = options->GetColor(8); //0xFFFFFF80; //OtherGuild Member - White Yellow
	Zeal::EqStructures::EQARGBCOLOR Adventurercolor = options->GetColor(9); //0xFF3D6BDC; //Not in Guild Member - Default Blue
	Zeal::EqStructures::EQARGBCOLOR NpcCorpsecolor = options->GetColor(10); //0xFF000000; //Npc Corpse - Black
	Zeal::EqStructures::EQARGBCOLOR PlayersCorpsecolor = options->GetColor(11); //0xFFFFFFFF; //Players Corpse - White Light Purple
	Zeal::EqStructures::EQARGBCOLOR Targetcolor = options->GetColor(18); //0xFFFFFFFF; //Players Corpse - White Light Purple
	if (nameplateTargetColor) {
		if (spawn == Zeal::EqGame::get_target()) {
			spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Targetcolor;
			return;
		}
	}
	if (spawn == Zeal::EqGame::get_target()) //Leave blinking indicator on target
		return;
	switch (spawn->Type) {
	case Zeal::EqEnums::EntityTypes::Player: //Players
		if (nameplateColors) 
		{
			uint8_t isInGroup = Zeal::EqGame::GroupInfo->is_in_group();
			auto raidSize = Zeal::EqGame::RaidInfo->MemberCount;
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			Zeal::EqStructures::Entity** groupmembers = reinterpret_cast<Zeal::EqStructures::Entity**>(Zeal::EqGame::GroupInfo->EntityList);
			const Zeal::EqStructures::RaidMember* raidMembers = &(Zeal::EqGame::RaidInfo->MemberList[0]);

			if (spawn->IsLinkDead == 1) { //LinkDead
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = LDcolor;
				return;
			}

			if (spawn->IsAwayFromKeyboard == 1) {//AFK
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = AFKcolor;
				return;
			}

			if (spawn->IsPlayerKill == 1){//PVP
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = PVPcolor;
				return;
			}

			if (spawn->ActorInfo->IsLookingForGroup == 1) { //LFG
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = LFGcolor;
				return;
			}

			if (isInGroup)
			{
				if (self == spawn)
				{
					self->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;
					return;
				}
				for (int i = 0; i < 5; i++)
				{
					Zeal::EqStructures::Entity* groupmember = groupmembers[i];
					if (groupmember == spawn)
					{
						spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;
						return;
					}
				}
			}

			if (raidSize > 1)  //RaidMember
			{
				if (self == spawn)
				{
					self->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;
					return;
				}
				for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i) //Raid Member loop
				{
					const Zeal::EqStructures::RaidMember& member = raidMembers[i];
					if ((member.GroupNumber == 0xFFFFFFFF - 1) || (strlen(member.Name) == 0) || (strcmp(member.Name, Zeal::EqGame::get_self()->Name) == 0))
						continue;
					Zeal::EqStructures::Entity* raidMember = ZealService::get_instance()->entity_manager->Get(member.Name);
					if (!raidMember)
						continue;
					if (spawn == raidMember)
					{
						raidMember->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;
						return;
					}
				}
			}

			if (spawn->AnonymousState == 2) {//Roleplay
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Rolecolor;
				return;
			}

			//If spawn is same Guild as Player
			if (spawn->GuildId == Zeal::EqGame::get_self()->GuildId && spawn->GuildId != 0xFFFF) {//MyGuild Member
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = MyGuildcolor;
				return;
			}
			//If spawn is in different Guild than Player
			if (spawn->GuildId != Zeal::EqGame::get_self()->GuildId && spawn->GuildId != 0xFFFF) {//OtherGuild Member
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = OtherGuildscolor;
				return;
			}
			//If not in a Guild - Adventurer has default color
			if (spawn->GuildId == 0xFFFF) { //Not in Guild Adventurer   
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Adventurercolor;
				return;
			}

		}
		break;
	case Zeal::EqEnums::EntityTypes::NPC: //NPC
		if (nameplateconColors) {
			if (spawn != Zeal::EqGame::get_self()) //All NPC entities
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Zeal::EqGame::GetLevelCon(spawn); //Level Con Color for NPCs
		}
		break;
	case Zeal::EqEnums::EntityTypes::NPCCorpse: //NPC Corpse
		if (nameplateColors) {
			spawn->ActorInfo->DagHeadPoint->StringSprite->Color = NpcCorpsecolor;
			return;
		}
		break;
	case Zeal::EqEnums::EntityTypes::PlayerCorpse: //Player Corpse
		if (nameplateColors) {
			spawn->ActorInfo->DagHeadPoint->StringSprite->Color = PlayersCorpsecolor;
			return;
		}
		break;
	default:
		break;
	}
}

int __fastcall SetNameSpriteTint(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn)
{
	int result = ZealService::get_instance()->hooks->hook_map["SetNameSpriteTint"]->original(SetNameSpriteTint)(this_ptr, not_used, spawn);
	if (ZealService::get_instance()->nameplate->nameplateCharSelect == false && Zeal::EqGame::get_gamestate() == GAMESTATE_CHARSELECT)
		return result;
	ZealService::get_instance()->nameplate->HandleTint(spawn);
	if (ZealService::get_instance()->nameplate->nameplateColors && Zeal::EqGame::get_gamestate() == GAMESTATE_CHARSELECT && Zeal::EqGame::get_self())
		Zeal::EqGame::get_self()->ActorInfo->DagHeadPoint->StringSprite->Color = 0xFF00FF32; //Green indication Namecolors on at Character Select
	return result;
}

void NamePlate::HandleState(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn)
{
	if (!spawn) { return; }
	if (!spawn->ActorInfo) { return; }
	if (!spawn->ActorInfo->DagHeadPoint) { return; }
	if (!spawn->ActorInfo->DagHeadPoint->StringSprite) { return; }
	DWORD fontTexture = *(DWORD*)(*(DWORD*)0x7F9510 + 0x2E08); //get the font texture
	const Zeal::EqStructures::RaidMember* raidMembers = &(Zeal::EqGame::RaidInfo->MemberList[0]);
	uint16_t showName = *(uint16_t*)0x7D01E4; // /showname command, 1 = first names, 2 = first/last names, 3 = first/last/guild names, 4 = everything
	uint8_t showPCNames = *(uint8_t*)0x63D6C8; //Options -> Display -> Show PC Names, 0 = off, 1 = on
	uint8_t showNPCNames = *(uint8_t*)0x63D6CC; //Options -> Display -> Show NPC Names, 0 = off, 1 = on
	if (spawn == Zeal::EqGame::get_self() && (nameplateHideSelf || nameplateX))
	{
		if (nameplateHideSelf)
		{
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_self()->ActorInfo->DagHeadPoint, fontTexture, (char*)"");
			SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_self());
			return;
		}
		if (nameplateX)
		{
			if (spawn->IsHidden == 0) //Visible
			{
				reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_self()->ActorInfo->DagHeadPoint, fontTexture, (char*)"X");
				SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_self());
				return;
			}
			if (spawn->IsHidden == 1) //Invisible
			{
				reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_self()->ActorInfo->DagHeadPoint, fontTexture, (char*)"(X)");
				SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_self());
				return;
			}
		}
	}
	if (nameplateHideRaidPets)
	{
		if (spawn->PetOwnerSpawnId == Zeal::EqGame::get_self()->SpawnId)
		{
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, spawn->ActorInfo->DagHeadPoint, fontTexture, (char*)"");
			SetNameSpriteTint(this_ptr, not_used, spawn);
			return;
		}
		for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i) //Raid Member loop
		{
			const Zeal::EqStructures::RaidMember& member = raidMembers[i];
			if ((member.GroupNumber == 0xFFFFFFFF - 1) || (strlen(member.Name) == 0) || (strcmp(member.Name, Zeal::EqGame::get_self()->Name) == 0))
				continue;
			Zeal::EqStructures::Entity* raidMember = ZealService::get_instance()->entity_manager->Get(member.Name);
			if (!raidMember)
				continue;
			if (spawn->PetOwnerSpawnId == raidMember->SpawnId)
			{
				reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, spawn->ActorInfo->DagHeadPoint, fontTexture, (char*)"");
				SetNameSpriteTint(this_ptr, not_used, spawn);
				return;
			}
		}
	}
	if (spawn == Zeal::EqGame::get_target() && (nameplateTargetMarker || nameplateTargetHealth)) {
		char targetNameplate[60];
		char targetFirstLineNameplate[40];
		char targetGuildNameplate[20];
		//strncpy_s(targetNameplate, sizeof(targetNameplate), spawn->ActorInfo->DagHeadPoint->StringSprite->Text, _TRUNCATE);
		int hpPercent = 0;
		if (spawn->Type == Zeal::EqEnums::EntityTypes::Player) { //PC Nameplate
			//Get Percent Health of Player
			if (spawn->HpCurrent > 0 && spawn->HpMax > 0)
				hpPercent = (spawn->HpCurrent * 100) / spawn->HpMax;
			//No Guild Nameplate for 2nd line
			if (spawn->GuildId == 0xFFFF)
				strncpy_s(targetGuildNameplate, sizeof(targetGuildNameplate), (char*)"", _TRUNCATE);
			//Guild Nameplate for 2nd line
			if (spawn->GuildId != 0xFFFF)
				_snprintf_s(targetGuildNameplate, sizeof(targetGuildNameplate), _TRUNCATE, "\n<%s>", Zeal::EqGame::get_guildName_from_guildId(spawn->GuildId));
			//AA Title and First and Last name in First Line of Nameplate
			if (showName == 4 && spawn->CharInfo->LastName && spawn->AlternateAdvancementRank > 0 && spawn->Gender != 2) //Illusions to Non-humanoid races remove AA_title, Gender = 2 means under illusion
				_snprintf_s(targetFirstLineNameplate, sizeof(targetFirstLineNameplate), _TRUNCATE, "%s %s %s", (char*)Zeal::EqGame::get_aa_title_name(spawn->Class, spawn->AlternateAdvancementRank, spawn->Gender), Zeal::EqGame::trim_name(spawn->CharInfo->Name), Zeal::EqGame::trim_name(spawn->CharInfo->LastName));
			//AA Title and First name in First Line of Nameplate
			if (showName == 4 && strcmp(spawn->CharInfo->LastName, "") == 0 && spawn->AlternateAdvancementRank > 0 && spawn->Gender != 2) //Illusions to Non-humanoid races remove AA_title, Gender = 2 means under illusion
				_snprintf_s(targetFirstLineNameplate, sizeof(targetFirstLineNameplate), _TRUNCATE, "%s %s", (char*)Zeal::EqGame::get_aa_title_name(spawn->Class, spawn->AlternateAdvancementRank, spawn->Gender), Zeal::EqGame::trim_name(spawn->CharInfo->Name));
			//First and Last name in First Line of Nameplate
			if ((showName == 2 && spawn->CharInfo->LastName) || (showName == 3 && spawn->CharInfo->LastName) || (showName == 4 && spawn->CharInfo->LastName && (spawn->AlternateAdvancementRank == 0 || (spawn->AlternateAdvancementRank > 0 && spawn->Gender == 2))))
				_snprintf_s(targetFirstLineNameplate, sizeof(targetFirstLineNameplate), _TRUNCATE, "%s %s", Zeal::EqGame::trim_name(spawn->CharInfo->Name), Zeal::EqGame::trim_name(spawn->CharInfo->LastName));
			//First name only in First Line of Nameplate
			if (showName == 1 || (showName == 2 && strcmp(spawn->CharInfo->LastName, "") == 0) || (showName == 3 && strcmp(spawn->CharInfo->LastName, "") == 0) || (showName == 4 && strcmp(spawn->CharInfo->LastName, "") == 0 && (spawn->AlternateAdvancementRank == 0 || (spawn->AlternateAdvancementRank > 0 && spawn->Gender == 2))))
				strncpy_s(targetFirstLineNameplate, sizeof(targetFirstLineNameplate), Zeal::EqGame::trim_name(spawn->CharInfo->Name), _TRUNCATE);
		}
		else { //NPC Nameplate
			//Get Percent Health of NPC
			hpPercent = spawn->HpCurrent;
			//Prevents broken string bug on Skeleton Nameplate, Allows Target Nameplate to play nice with Skeleton Nameplate fix code below
			if (spawn->Type == 60 && spawn->StandingState == Zeal::EqEnums::Stance::Feigned) 
				_snprintf_s(targetFirstLineNameplate, sizeof(targetFirstLineNameplate), (size_t)-0, "%s", Zeal::EqGame::trim_name(spawn->Name));
			else
				_snprintf_s(targetFirstLineNameplate, sizeof(targetFirstLineNameplate), _TRUNCATE, "%s", Zeal::EqGame::trim_name(spawn->Name));
		}
		//Below accounts for /showname 4, Two lines on Nameplate, Guild on 2nd line, "AA_Title First_Name Last_Name \n <Guild>"
		//Below also accounts for /showname 3, Two lines on Nameplate, Guild on 2nd line, "First_Name Last_Name \n <Guild>"
		if ((showName == 4 || showName == 3) && spawn->Type == Zeal::EqEnums::EntityTypes::Player) {
			if (nameplateTargetMarker && nameplateTargetHealth) {
				_snprintf_s(targetNameplate, sizeof(targetNameplate), _TRUNCATE, ">%s %i%%<%s", targetFirstLineNameplate, hpPercent, targetGuildNameplate);
				reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_target()->ActorInfo->DagHeadPoint, fontTexture, targetNameplate);
				SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_target());
				return;
			}
			if (nameplateTargetMarker) {
				_snprintf_s(targetNameplate, sizeof(targetNameplate), _TRUNCATE, ">%s<%s", targetFirstLineNameplate, targetGuildNameplate);
				reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_target()->ActorInfo->DagHeadPoint, fontTexture, targetNameplate);
				SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_target());
				return;
			}
			if (nameplateTargetHealth) {
				_snprintf_s(targetNameplate, sizeof(targetNameplate), _TRUNCATE, "%s %i%%%s", targetFirstLineNameplate, hpPercent, targetGuildNameplate);
				reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_target()->ActorInfo->DagHeadPoint, fontTexture, targetNameplate);
				SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_target());
				return;
			}
		}
		//Below accounts for /showname 1 and /showname 2 with only one line on Nameplate, no Guild line. "First_Name" and "First_Name Last_name"
		if (nameplateTargetMarker && nameplateTargetHealth && (spawn->Type == Zeal::EqEnums::EntityTypes::Player || spawn->Type == Zeal::EqEnums::EntityTypes::NPC)) {
			_snprintf_s(targetNameplate, sizeof(targetNameplate), _TRUNCATE, ">%s %i%%<", targetFirstLineNameplate, hpPercent);
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_target()->ActorInfo->DagHeadPoint, fontTexture, targetNameplate);
			SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_target());
			return;
		}
		if (nameplateTargetMarker && (spawn->Type == Zeal::EqEnums::EntityTypes::Player || spawn->Type == Zeal::EqEnums::EntityTypes::NPC)) {
			_snprintf_s(targetNameplate, sizeof(targetNameplate), _TRUNCATE, ">%s<", targetFirstLineNameplate);
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_target()->ActorInfo->DagHeadPoint, fontTexture, targetNameplate);
			SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_target());
			return;
		}
		if (nameplateTargetHealth && (spawn->Type == Zeal::EqEnums::EntityTypes::Player || spawn->Type == Zeal::EqEnums::EntityTypes::NPC)) {
			_snprintf_s(targetNameplate, sizeof(targetNameplate), _TRUNCATE, "%s %i%%", targetFirstLineNameplate, hpPercent);
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, Zeal::EqGame::get_target()->ActorInfo->DagHeadPoint, fontTexture, targetNameplate);
			SetNameSpriteTint(this_ptr, not_used, Zeal::EqGame::get_target());
			return;
		}
	}
	if (spawn->Race == 60) { //Race = 60 is Skeletons. Below is Skeleton Nameplate fix code
		//Skeleton Feigned - Nameplate fix - Skeletons Feigned at their spawn point now show a Nameplate
		if (spawn->Type == Zeal::EqEnums::EntityTypes::NPC && spawn->StandingState != Zeal::EqEnums::Stance::Standing) { //Skeleton Feigned Death at Spawn Point Nameplate fix
			char skeletonNameplate[30];
			_snprintf_s(skeletonNameplate, sizeof(skeletonNameplate), _TRUNCATE, "%s", Zeal::EqGame::trim_name(spawn->Name));
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO* dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, spawn->ActorInfo->DagHeadPoint, fontTexture, skeletonNameplate);
			SetNameSpriteTint(this_ptr, not_used, spawn);
			return;
		}
		//Skeleton Corpse - Nameplate fix
		if (spawn->Type == Zeal::EqEnums::EntityTypes::NPCCorpse || spawn->Type == Zeal::EqEnums::EntityTypes::PlayerCorpse) {
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, spawn->ActorInfo->DagHeadPoint, fontTexture, Zeal::EqGame::trim_name(spawn->Name));
			SetNameSpriteTint(this_ptr, not_used, spawn);
			return;
		}	
	}
}

int __fastcall SetNameSpriteState(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn, bool show)
{
	int result = ZealService::get_instance()->hooks->hook_map["SetNameSpriteState"]->original(SetNameSpriteState)(this_ptr, not_used, spawn, show);
	if (ZealService::get_instance()->nameplate->nameplateCharSelect == false && Zeal::EqGame::get_gamestate() == GAMESTATE_CHARSELECT && Zeal::EqGame::get_self())
		return result;
	ZealService::get_instance()->nameplate->HandleState(this_ptr, not_used, spawn);
	return result;
}

void NamePlate::colors_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateColors", _enabled);
	Zeal::EqGame::print_chat("NameplateColors are %s", _enabled ? "Enabled" : "Disabled");
	nameplateColors = _enabled;
}

void NamePlate::con_colors_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateConColors", _enabled);
	Zeal::EqGame::print_chat("NameplateConColors are %s", _enabled ? "Enabled" : "Disabled");
	nameplateconColors = _enabled;
}

void NamePlate::hide_self_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateHideSelf", _enabled);
	Zeal::EqGame::print_chat("Hidden Nameplate for Self is %s", _enabled ? "Enabled" : "Disabled");
	nameplateHideSelf = _enabled;
}

void NamePlate::x_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateX", _enabled);
	Zeal::EqGame::print_chat("Show Nameplate for Self set to X is %s", _enabled ? "Enabled" : "Disabled");
	nameplateX = _enabled;
}

void NamePlate::hide_raidpets_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateHideRaidPets", _enabled);
	Zeal::EqGame::print_chat("Hidden Nameplates for RaidPets are %s", _enabled ? "Enabled" : "Disabled");
	nameplateHideRaidPets = _enabled;
}

void NamePlate::charselect_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateCharSelect", _enabled);
	Zeal::EqGame::print_chat("Show Nameplate Options at Character Selection Screen is %s", _enabled ? "Enabled" : "Disabled");
	nameplateCharSelect = _enabled;
}

void NamePlate::target_color_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateTargetColor", _enabled);
	Zeal::EqGame::print_chat("Show Target Nameplate Color is %s", _enabled ? "Enabled" : "Disabled");
	nameplateTargetColor = _enabled;
}

void NamePlate::target_marker_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateTargetMarker", _enabled);
	Zeal::EqGame::print_chat("Show Target Nameplate Marker is %s", _enabled ? "Enabled" : "Disabled");
	nameplateTargetMarker = _enabled;
}

void NamePlate::target_health_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateTargetHealth", _enabled);
	Zeal::EqGame::print_chat("Show Target Nameplate Health is %s", _enabled ? "Enabled" : "Disabled");
	nameplateTargetHealth = _enabled;
}

NamePlate::NamePlate(ZealService* zeal, IO_ini* ini)
{
	//HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");

	//remove unused hook for now
	//if (eqfx) 
		//zeal->hooks->Add("DeferCachedNameTagTextW", (DWORD)eqfx + 0x70A00, DeferCachedNameTagTextW, hook_type_detour);

	zeal->hooks->Add("SetNameSpriteState", 0x4B0BD9, SetNameSpriteState, hook_type_detour);
	zeal->hooks->Add("SetNameSpriteTint", 0x4B114D, SetNameSpriteTint, hook_type_detour);
	
	if (!ini->exists("Zeal", "NameplateColors")) 
		ini->setValue<bool>("Zeal", "NameplateColors", false);
	if (!ini->exists("Zeal", "NameplateConColors"))
		ini->setValue<bool>("Zeal", "NameplateConColors", false);
	if (!ini->exists("Zeal", "NameplateHideSelf"))
		ini->setValue<bool>("Zeal", "NameplateHideSelf", false);
	if (!ini->exists("Zeal", "NameplateX"))
		ini->setValue<bool>("Zeal", "NameplateX", false);
	if (!ini->exists("Zeal", "NameplateHideRaidPets"))
		ini->setValue<bool>("Zeal", "NameplateHideRaidPets", false);
	if (!ini->exists("Zeal", "NameplateCharSelect"))
		ini->setValue<bool>("Zeal", "NameplateCharSelect", false);
	if (!ini->exists("Zeal", "NameplateTargetColor"))
		ini->setValue<bool>("Zeal", "NameplateTargetColor", false);
	if (!ini->exists("Zeal", "NameplateTargetMarker"))
		ini->setValue<bool>("Zeal", "NameplateTargetMarker", false);
	if (!ini->exists("Zeal", "NameplateTargetHealth"))
		ini->setValue<bool>("Zeal", "NameplateTargetHealth", false);

	nameplateColors = ini->getValue<bool>("Zeal", "NameplateColors");
	nameplateconColors = ini->getValue<bool>("Zeal", "NameplateConColors");
	nameplateHideSelf = ini->getValue<bool>("Zeal", "NameplateHideSelf");
	nameplateX = ini->getValue<bool>("Zeal", "NameplateX");
	nameplateHideRaidPets = ini->getValue<bool>("Zeal", "NameplateHideRaidPets");
	nameplateCharSelect = ini->getValue<bool>("Zeal", "NameplateCharSelect");
	nameplateTargetColor = ini->getValue<bool>("Zeal", "NameplateTargetColor");
	nameplateTargetMarker = ini->getValue<bool>("Zeal", "NameplateTargetMarker");
	nameplateTargetHealth = ini->getValue<bool>("Zeal", "NameplateTargetHealth");

	zeal->commands_hook->Add("/nameplatecolors", {}, "Toggles Nameplate Colors",
		[this](std::vector<std::string>& args) {
			colors_set_enabled(!ZealService::get_instance()->nameplate->colors_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplateconcolors", {}, "Toggles Nameplate Con Colors",
		[this](std::vector<std::string>& args) {
			con_colors_set_enabled(!ZealService::get_instance()->nameplate->con_colors_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatehideself", {}, "Toggles Nameplate Self",
		[this](std::vector<std::string>& args) {
			hide_self_set_enabled(!ZealService::get_instance()->nameplate->hide_self_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatex", {}, "Toggles Nameplate Self as X",
		[this](std::vector<std::string>& args) {
			x_set_enabled(!ZealService::get_instance()->nameplate->x_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatehideraidpets", {}, "Toggles Nameplate for Raid Pets",
		[this](std::vector<std::string>& args) {
			hide_raidpets_set_enabled(!ZealService::get_instance()->nameplate->hide_raidpets_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatecharselect", {}, "Toggles Nameplate Options Shown at Character Selection Screen",
		[this](std::vector<std::string>& args) {
			charselect_set_enabled(!ZealService::get_instance()->nameplate->charselect_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatetargetcolor", {}, "Toggles Target Nameplate Color",
		[this](std::vector<std::string>& args) {
			target_color_set_enabled(!ZealService::get_instance()->nameplate->target_color_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatetargetmarker", {}, "Toggles Target Nameplate Marker",
		[this](std::vector<std::string>& args) {
			target_marker_set_enabled(!ZealService::get_instance()->nameplate->target_marker_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatetargethealth", {}, "Toggles Target Nameplate Health",
		[this](std::vector<std::string>& args) {
			target_health_set_enabled(!ZealService::get_instance()->nameplate->target_health_is_enabled());
			return true;
		});
}

NamePlate::~NamePlate()
{
}
