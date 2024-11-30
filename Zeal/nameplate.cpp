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
	uint8_t isInGroup = Zeal::EqGame::GroupInfo->is_in_group();
	auto raidSize = Zeal::EqGame::RaidInfo->MemberCount;
	Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
	Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
	Zeal::EqStructures::Entity** groupmembers = reinterpret_cast<Zeal::EqStructures::Entity**>(Zeal::EqGame::GroupInfo->EntityList);
	const Zeal::EqStructures::RaidMember* raidMembers = &(Zeal::EqGame::RaidInfo->MemberList[0]);
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
	Zeal::EqStructures::EQARGBCOLOR Targetcolor = options->GetColor(18); //0xFFFF80FF; //Target Color - Default Pink
	if (nameplateTargetColor) {
		if (spawn == target) {
			spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Targetcolor;
			return;
		}
	}
	if (spawn == target) //Leave blinking indicator on target
		return;
	switch (spawn->Type) {
	case Zeal::EqEnums::EntityTypes::Player: //Players
		if (nameplateColors) 
		{
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
				if (spawn == self)
				{
					self->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;
					return;
				}
				for (int i = 0; i < 5; i++)
				{
					Zeal::EqStructures::Entity* groupmember = groupmembers[i];
					if (spawn == groupmember)
					{
						if (!groupmember)
							continue;
						groupmember->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;
						return;
					}
					continue;
				}
			}

			if (raidSize > 1)  //RaidMember
			{
				if (spawn == self)
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
		if (nameplateconColors) { //Con Colors for Players if Nameplate Colors system off, Nameplate Con Colors on
			if (spawn != Zeal::EqGame::get_self()) {
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Zeal::EqGame::GetLevelCon(spawn);
				return;
			}
		}
		break;
	case Zeal::EqEnums::EntityTypes::NPC: //NPC
		if (nameplateColors && isInGroup)
		{
			if (spawn->PetOwnerSpawnId == self->SpawnId)//Self Pet
			{
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;//Self Pet to Groupcolor
				return;
			}
			for (int i = 0; i < 5; i++)//GroupMember Pet Loop
			{
				Zeal::EqStructures::Entity* groupmember = groupmembers[i];
				if (!groupmember)
					continue;
				if (spawn->PetOwnerSpawnId == groupmember->SpawnId)
				{
					spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;//GroupMember Pet to Groupcolor
					return;
				}
			}
		}
		if (nameplateColors && raidSize > 1)
		{
			if (spawn->PetOwnerSpawnId == self->SpawnId)//Self Pet
			{
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;//Self Pet to Raidcolor
				return;
			}
			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i) //Raid Member Pet Loop
			{
				const Zeal::EqStructures::RaidMember& member = raidMembers[i];
				if ((member.GroupNumber == 0xFFFFFFFF - 1) || (strlen(member.Name) == 0) || (strcmp(member.Name, Zeal::EqGame::get_self()->Name) == 0))
					continue;
				Zeal::EqStructures::Entity* raidMember = ZealService::get_instance()->entity_manager->Get(member.Name);
				if (!raidMember)
					continue;
				if (spawn->PetOwnerSpawnId == raidMember->SpawnId)
				{
					spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;//Raid Pet to Raidcolor
					return;
				}
			}
		}
		if (nameplateconColors) {
			if (spawn != Zeal::EqGame::get_self()) {//All NPC entities
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Zeal::EqGame::GetLevelCon(spawn); //Level Con Color for NPCs
				return;
			}
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

void ChangeDagStringSprite(Zeal::EqStructures::EQDAGINFO* dag, int fontTexture, const char* str)
{
	reinterpret_cast<int(__thiscall*)(void* _this_ptr, Zeal::EqStructures::EQDAGINFO* dag, int fontTexture, const char* text)>(0x4B0AA8)(*(void**)0x7F9510, dag, fontTexture, str);
}

std::string generateTargetNameplateString(const std::string& nameplateString) {
	ZealService* zeal = ZealService::get_instance();
	std::ostringstream ossShowNameLogic;
	std::ostringstream ossTargetNameplate;
	Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
	uint16_t showName = *(uint16_t*)0x7D01E4; // /showname command, 1 = first names, 2 = first/last names, 3 = first/last/guild names, 4 = everything
	bool hasLastName = false;
	bool hasGuild = false;
	int hpPercent = 0;
	std::string::size_type guildNamePosition = nameplateString.find("\n"); //Finds if nameplateString has 2nd line for Guild
	
	if ((strcmp(target->LastName, "") != 0)) //Does target have a last name
		hasLastName = true;
	if (guildNamePosition != std::string::npos) //Does target have a Guild
		hasGuild = true;
	if (target->Type == Zeal::EqEnums::EntityTypes::Player && target->HpCurrent > 0 && target->HpMax > 0)
		hpPercent = (target->HpCurrent * 100) / target->HpMax; //Get Percent Health of Target Player
	else
		hpPercent = target->HpCurrent; //Get Percent Health of Target NPC

	if (target->Type == Zeal::EqEnums::EntityTypes::Player) //Target Player ShowName Nameplate Logic
	{
		if (((showName == 2 && hasLastName == false ) || ((showName == 3 || showName == 4) && hasLastName == false && hasGuild == false)))// /show 2,3,4 + no_lastname + no guild
			ossShowNameLogic << nameplateString.substr(0, nameplateString.size() - 1);
		else if ((showName == 3 || showName == 4) && hasLastName == true && hasGuild == true)// /show 3,4 + lastname + guild
			ossShowNameLogic << nameplateString.substr(0, guildNamePosition);
		else if ((showName == 3 || showName == 4) && hasLastName == false && hasGuild == true)// /show 3,4 + no_lastname + guild
			ossShowNameLogic << nameplateString.substr(0, guildNamePosition - 1);
		else// /show 1,2,3,4 + no guild displayed
			ossShowNameLogic << nameplateString;
	}
	else //Target NPC Nameplate	
	{  
			ossShowNameLogic << nameplateString; //All other NPC Nameplate
	}

	if (zeal->nameplate->nameplateTargetMarker && zeal->nameplate->nameplateTargetHealth) 
	{
		if (hasGuild == false)
			ossTargetNameplate << "<" << ossShowNameLogic.str() << " " << hpPercent << "%>";
		if (hasGuild == true)
			ossTargetNameplate << "<" << ossShowNameLogic.str() << " " << hpPercent << "%>" << nameplateString.substr(guildNamePosition, nameplateString.size());
		return ossTargetNameplate.str();
	}

	if (zeal->nameplate->nameplateTargetMarker) 
	{
		if (hasGuild == false)
			ossTargetNameplate << "<" << ossShowNameLogic.str() << ">";
		if (hasGuild == true)
			ossTargetNameplate << "<" << ossShowNameLogic.str() << ">" << nameplateString.substr(guildNamePosition, nameplateString.size());
		return ossTargetNameplate.str();
	}

	if (zeal->nameplate->nameplateTargetHealth) 
	{
		if (hasGuild == false)
			ossTargetNameplate << ossShowNameLogic.str() << " " << hpPercent << "%";
		if (hasGuild == true)
			ossTargetNameplate << ossShowNameLogic.str() << " " << hpPercent << "%" << nameplateString.substr(guildNamePosition, nameplateString.size());
		return ossTargetNameplate.str();
	}

	return ossTargetNameplate.str();
}

void NamePlate::HandleState(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn)
{
	if (!spawn) { return; }
	if (!spawn->ActorInfo) { return; }
	if (!spawn->ActorInfo->DagHeadPoint) { return; }
	if (!spawn->ActorInfo->DagHeadPoint->StringSprite) { return; }
	DWORD fontTexture = *(DWORD*)(*(DWORD*)0x7F9510 + 0x2E08); //get the font texture
	Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
	Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
	const Zeal::EqStructures::RaidMember* raidMembers = &(Zeal::EqGame::RaidInfo->MemberList[0]);
	if (spawn == Zeal::EqGame::get_self() && (nameplateHideSelf || nameplateX))
	{
		if (nameplateHideSelf)
		{
			ChangeDagStringSprite(self->ActorInfo->DagHeadPoint, fontTexture, "");
			return;
		}
		if (nameplateX)
		{
			if (spawn->IsHidden == 0) //Visible
			{
				ChangeDagStringSprite(self->ActorInfo->DagHeadPoint, fontTexture, "X");
				SetNameSpriteTint(this_ptr, not_used, self);
				return;
			}
			if (spawn->IsHidden == 1) //Invisible
			{
				ChangeDagStringSprite(self->ActorInfo->DagHeadPoint, fontTexture, "(X)");
				SetNameSpriteTint(this_ptr, not_used, self);
				return;
			}
		}
	}
	if (nameplateHideRaidPets)
	{
		if (spawn->PetOwnerSpawnId == self->SpawnId)  //Self Pet
		{
			ChangeDagStringSprite(spawn->ActorInfo->DagHeadPoint, fontTexture, "");
			return;
		}
		for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i) //Raid Member loop
		{
			const Zeal::EqStructures::RaidMember& member = raidMembers[i];
			if ((member.GroupNumber == 0xFFFFFFFF - 1) || (strlen(member.Name) == 0) || (strcmp(member.Name, self->Name) == 0))
				continue;
			Zeal::EqStructures::Entity* raidMember = ZealService::get_instance()->entity_manager->Get(member.Name);
			if (!raidMember)
				continue;
			if (spawn->PetOwnerSpawnId == raidMember->SpawnId) //Raid Pet
			{
				ChangeDagStringSprite(spawn->ActorInfo->DagHeadPoint, fontTexture, "");
				return;
			}
		}
	}
	bool valid_string_sprite = (spawn->ActorInfo->DagHeadPoint->StringSprite->Unknown0000 == 0x51);
	if (spawn == target && (nameplateTargetMarker || nameplateTargetHealth)) { //Target Marker and Target Health
		std::string targetNameplate = valid_string_sprite ?
			spawn->ActorInfo->DagHeadPoint->StringSprite->Text : Zeal::EqGame::trim_name(spawn->Name);
		ChangeDagStringSprite(target->ActorInfo->DagHeadPoint, fontTexture, generateTargetNameplateString(targetNameplate).c_str());
		return;
	}

	// Handle cases of hidden, invalid nameplates that we want to show (targeted or corpses).
	if (!valid_string_sprite &&
		(spawn == target || spawn->Type == Zeal::EqEnums::EntityTypes::NPCCorpse ||
			spawn->Type == Zeal::EqEnums::EntityTypes::PlayerCorpse)) 
	{
		std::string spawnName = Zeal::EqGame::trim_name(spawn->Name);
		ChangeDagStringSprite(spawn->ActorInfo->DagHeadPoint, fontTexture, spawnName.c_str());
		SetNameSpriteTint(this_ptr, not_used, spawn);
		return;
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
