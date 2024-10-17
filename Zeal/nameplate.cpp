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
	Zeal::EqStructures::EQARGBCOLOR BlueConcolor = options->GetColor(12); //BlueCon - Default DarkBlue is ligher than CON_BLUE
	Zeal::EqStructures::EQARGBCOLOR GreenConcolor = options->GetColor(13); //CON_GREEN
	Zeal::EqStructures::EQARGBCOLOR LightBlueConcolor = options->GetColor(14); //CON_LIGHTBLUE
	Zeal::EqStructures::EQARGBCOLOR WhiteConcolor = options->GetColor(15); //CON_WHITE
	Zeal::EqStructures::EQARGBCOLOR YellowConcolor = options->GetColor(16); //CON_YELLOW
	Zeal::EqStructures::EQARGBCOLOR RedConcolor = options->GetColor(17); //CON_RED
	switch (spawn->Type) {
	case 0: //Players
		if (nameplateColors) 
		{
			uint8_t isInGroup = *(uint8_t*)0x7912B0;
			uint16_t raidSize = *(uint16_t*)0x794F9C;
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			Zeal::EqStructures::Entity** groupmembers = reinterpret_cast<Zeal::EqStructures::Entity**>(Zeal::EqGame::GroupList);
			Zeal::EqStructures::RaidMember* raidMembers = reinterpret_cast<Zeal::EqStructures::RaidMember*>(Zeal::EqGame::RaidMemberList);

			if (spawn == Zeal::EqGame::get_target()) //Leave blinking indicator on target
				return;

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
				for (int i = 0; i < 72; ++i) //Raid Member loop
				{
					Zeal::EqStructures::RaidMember member = raidMembers[i];
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
	case 1: //NPC
		if (nameplateconColors) {
			if (spawn == Zeal::EqGame::get_target()) //Leave blinking indicator on target
				return;
			if (Zeal::EqGame::GetLevelCon(spawn) == CON_GREEN) { //Changes NPC Green Con to user selected Color
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = GreenConcolor;
				return;
			}
			if (Zeal::EqGame::GetLevelCon(spawn) == CON_LIGHTBLUE) { //Changes NPC Light Blue Con to user selected Color
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = LightBlueConcolor;
				return;
			}
			if (Zeal::EqGame::GetLevelCon(spawn) == CON_BLUE){ //Changes NPC DarkBlue Con to user selected Color
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = BlueConcolor;
				return;
			}
			if (Zeal::EqGame::GetLevelCon(spawn) == CON_WHITE) { //Changes NPC White Con to user selected Color
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = WhiteConcolor;
				return;
			}
			if (Zeal::EqGame::GetLevelCon(spawn) == CON_YELLOW) { //Changes NPC Yellow Con to user selected Color
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = YellowConcolor;
				return;
			}
			if (Zeal::EqGame::GetLevelCon(spawn) == CON_RED) { //Changes NPC Red Con to user selected Color
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = RedConcolor;
				return;
			}
			if (spawn != Zeal::EqGame::get_self()) //All NPC entities
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Zeal::EqGame::GetLevelCon(spawn); //Level Con Color for NPCs
		}
		break;
	case 2: //NPC Corpse
		if (nameplateColors) {
			spawn->ActorInfo->DagHeadPoint->StringSprite->Color = NpcCorpsecolor;
			return;
		}
		break;
	case 3: //Player Corpse
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
	ZealService::get_instance()->nameplate->HandleTint(spawn);
	if (ZealService::get_instance()->nameplate->nameplateColors && Zeal::EqGame::get_gamestate() == GAMESTATE_CHARSELECT && Zeal::EqGame::get_self())
		Zeal::EqGame::get_self()->ActorInfo->DagHeadPoint->StringSprite->Color = 0xFF00FF32; //Green indication Namecolors on at Character Select
	return result;
}

char* trim_name(char* spawnName)
{
	return reinterpret_cast<char* (__thiscall*)(int CEverquest_ptr, char* spawnName)>(0x537D39)(*(int*)0x809478, spawnName);
}

void NamePlate::HandleState(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn)
{
	if (!spawn) { return; }
	if (!spawn->ActorInfo) { return; }
	if (!spawn->ActorInfo->DagHeadPoint) { return; }
	if (!spawn->ActorInfo->DagHeadPoint->StringSprite) { return; }
	DWORD fontTexture = *(DWORD*)(*(DWORD*)0x7F9510 + 0x2E08); //get the font texture
	Zeal::EqStructures::RaidMember* raidMembers = reinterpret_cast<Zeal::EqStructures::RaidMember*>(Zeal::EqGame::RaidMemberList);
	if (spawn == Zeal::EqGame::get_self())
	{
		if (nameplateSelf)
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
	if (nameplateRaidPets)
	{
		if (spawn->PetOwnerSpawnId == Zeal::EqGame::get_self()->SpawnId)
		{
			reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, spawn->ActorInfo->DagHeadPoint, fontTexture, (char*)"");
			SetNameSpriteTint(this_ptr, not_used, spawn);
			return;
		}
		for (int i = 0; i < 72; ++i) //Raid Member loop
		{
			Zeal::EqStructures::RaidMember member = raidMembers[i];
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
	if ((spawn->Type == 2 || spawn->Type == 3) && spawn->Race == 60) { //Skeleton Corpse - Nameplate fix
		reinterpret_cast<int(__thiscall*)(void* this_ptr, Zeal::EqStructures::EQDAGINFO * dag, int fontTexture, char* text)>(0x4B0AA8)(this_ptr, spawn->ActorInfo->DagHeadPoint, fontTexture, trim_name(spawn->Name));
		SetNameSpriteTint(this_ptr, not_used, spawn);
		return;
	}

}

int __fastcall SetNameSpriteState(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn, bool show)
{
	int result = ZealService::get_instance()->hooks->hook_map["SetNameSpriteState"]->original(SetNameSpriteState)(this_ptr, not_used, spawn, show);
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

void NamePlate::self_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateSelf", _enabled);
	Zeal::EqGame::print_chat("Nameplate for Self is %s", _enabled ? "Disabled" : "Enabled");
	nameplateSelf = _enabled;
}

void NamePlate::x_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateX", _enabled);
	Zeal::EqGame::print_chat("Nameplate for Self set to X is %s", _enabled ? "Enabled" : "Disabled");
	nameplateX = _enabled;
}

void NamePlate::raidpets_set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateRaidPets", _enabled);
	Zeal::EqGame::print_chat("Nameplates for RaidPets are %s", _enabled ? "Disabled" : "Enabled");
	nameplateRaidPets = _enabled;
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
	if (!ini->exists("Zeal", "NameplateSelf"))
		ini->setValue<bool>("Zeal", "NameplateSelf", false);
	if (!ini->exists("Zeal", "NameplateX"))
		ini->setValue<bool>("Zeal", "NameplateX", false);
	if (!ini->exists("Zeal", "NameplateRaidPets"))
		ini->setValue<bool>("Zeal", "NameplateRaidPets", false);

	nameplateColors = ini->getValue<bool>("Zeal", "NameplateColors");
	nameplateconColors = ini->getValue<bool>("Zeal", "NameplateConColors");
	nameplateSelf = ini->getValue<bool>("Zeal", "NameplateSelf");
	nameplateX = ini->getValue<bool>("Zeal", "NameplateX");
	nameplateRaidPets = ini->getValue<bool>("Zeal", "NameplateRaidPets");

	zeal->commands_hook->Add("/nameplatecolors", {}, "Toggles Nameplate Colors",
		[this](std::vector<std::string>& args) {
			colors_set_enabled(!ZealService::get_instance()->nameplate->colors_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplateconcolors", {}, "Toggles Nameplate Colors",
		[this](std::vector<std::string>& args) {
			con_colors_set_enabled(!ZealService::get_instance()->nameplate->con_colors_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplateself", {}, "Toggles Nameplate Colors",
		[this](std::vector<std::string>& args) {
			colors_set_enabled(!ZealService::get_instance()->nameplate->self_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplatex", {}, "Toggles Nameplate Colors",
		[this](std::vector<std::string>& args) {
			con_colors_set_enabled(!ZealService::get_instance()->nameplate->x_is_enabled());
			return true;
		});
	zeal->commands_hook->Add("/nameplateraidpets", {}, "Toggles Nameplate Colors",
		[this](std::vector<std::string>& args) {
			colors_set_enabled(!ZealService::get_instance()->nameplate->raidpets_is_enabled());
			return true;
		});
}

NamePlate::~NamePlate()
{
}
