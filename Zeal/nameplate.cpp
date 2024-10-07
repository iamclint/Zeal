#include "nameplate.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"


//struct CachedNameData
//{
//	int unk1;
//	int unk2;
//	int name_len;
//	int unk3;
//	int fontptr;
//	char name[30];
//};

//int DeferCachedNameTagTextW(int font_size, CachedNameData* cnd, int a1, int a2, int a3, int a4, double a5, int a6)
//{
//	//if (GetAsyncKeyState(VK_END))
//	//{
//	//	Zeal::EqGame::print_chat("font: [%i] name: [%s] a1: [%i] a2: [%i] a3: [%i] a4: [%i] a5: [%f] a6: [%i]", font_size, cnd->name, a1, a2, a3, a4, a5, a6);
//	//}
//	//if (Zeal::EqGame::get_self())
//	//{
//	//	std::string name = cnd->name;
//	//	name = Zeal::String::split(name, " ").front();
//	//	std::string me = Zeal::EqGame::get_self()->Name;
//	//	me = Zeal::String::trim_and_reduce_spaces(me);
//	//	if (name == me)
//	//	{
//	//		cnd->unk1 = 0;
//	//	}
//	//	return ZealService::get_instance()->hooks->hook_map["DeferCachedNameTagTextW"]->original(DeferCachedNameTagTextW)(font_size, cnd, a1, a2, a3, a4, a5, a6);
//	//}
//	//else
//	//{
//		return ZealService::get_instance()->hooks->hook_map["DeferCachedNameTagTextW"]->original(DeferCachedNameTagTextW)(font_size, cnd, a1, a2, a3, a4, a5, a6);
//	//}
//}

void NamePlate::HandleTint(Zeal::EqStructures::Entity* spawn)
{
	if (!spawn) { return; }
	if (!spawn->ActorInfo) { return; }
	if (!spawn->ActorInfo->DagHeadPoint) { return; }
	if (!spawn->ActorInfo->DagHeadPoint->StringSprite) { return; }
	ui_options* options = ZealService::get_instance()->ui->options.get();
	switch (spawn->Type) {
	case 0: //Players
		if (nameplateColors) {
			Zeal::EqStructures::EQARGBCOLOR LDcolor = options->GetColor(2); //0xFFFF0000; //LinkDead - Red
			Zeal::EqStructures::EQARGBCOLOR AFKcolor = options->GetColor(0); //0xFFFF8000; //AFK - Orange
			Zeal::EqStructures::EQARGBCOLOR LFGcolor =  options->GetColor(1); //0x00CFFF00; //LFG - Yellow
			Zeal::EqStructures::EQARGBCOLOR Groupcolor = options->GetColor(5); //0x0500FF32; //Group Member - Light Green
			Zeal::EqStructures::EQARGBCOLOR Raidcolor =  options->GetColor(4); //0xFFFFFFFF; //Raid Member - White Light Purple
			Zeal::EqStructures::EQARGBCOLOR Guildcolor = options->GetColor(3); //0x0000FFFF; //Guild Member - Cyan
			//int maxGroupMembers = 5;
			//int maxRaidMembers = 72;
			uint8_t isInGroup = *(uint8_t*)0x7912B0;
			uint8_t groupSize = *(uint8_t*)0x7912B0 + *(uint8_t*)0x7912B1 + *(uint8_t*)0x7912B2 + *(uint8_t*)0x7912B3 + *(uint8_t*)0x7912B4;
			uint16_t raidSize = *(uint16_t*)0x794F9C;
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			Zeal::EqStructures::Entity** groupmembers = reinterpret_cast<Zeal::EqStructures::Entity**>(Zeal::EqGame::GroupList);
			Zeal::EqStructures::RaidMember* raidMembers = reinterpret_cast<Zeal::EqStructures::RaidMember*>(Zeal::EqGame::RaidMemberList);

			if (spawn->IsLinkDead == 1) { //LinkDead
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = LDcolor;
				return;
			}
			else if (spawn->IsAwayFromKeyboard == 1) {//AFK
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = AFKcolor;
				return;
			}
			else if (spawn == Zeal::EqGame::get_target()) //Leave blinking indicator on target
				return;
			else if (spawn->ActorInfo->IsLookingForGroup == 1) { //LFG
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = LFGcolor;
				return;
			}
			if (groupmembers) {
				switch (isInGroup) {
					case 0:
						break;
					case 1:
						for (int i = 0; i < groupSize; ++i) //Group Member loop
						{
							Zeal::EqStructures::Entity* groupmember = groupmembers[i];
							if (spawn == Zeal::EqGame::get_self()) {
								self->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;
								return;
							}
							else if (spawn != Zeal::EqGame::get_self()) {
								if (spawn == groupmember) {
									spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Groupcolor;
									return;
								}
								else if (spawn != groupmember) {
									if (raidSize > 1) { //RaidMember
										for (int i = 0; i < raidSize; ++i) //Raid Member loop
										{
											Zeal::EqStructures::RaidMember member = raidMembers[i];
											if ((member.GroupNumber == 0xFFFFFFFF - 1) || (strlen(member.Name) == 0) || (strcmp(member.Name, Zeal::EqGame::get_self()->Name) == 0))
												continue;
											Zeal::EqStructures::Entity* raidMember = ZealService::get_instance()->entity_manager->Get(member.Name);
											if (!raidMember)
												continue;
											if (spawn == raidMember)
												raidMember->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;
										}
									}
									else if (spawn->GuildId == 0xFFFF || spawn->GuildId != Zeal::EqGame::get_self()->GuildId) { 
										//Separates those Not in a Guild or different Guild than Player
									}
									else if (spawn->GuildId == Zeal::EqGame::get_self()->GuildId) {//Guild Member
										spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Guildcolor;
									}
									return;
								}
							}
						}
						break;
				}
			}
			if (raidMembers) {
				if (raidSize == 1)
					break;
				if (raidSize > 1)
				{
					for (int i = 0; i < raidSize; ++i) //Raid Member loop
					{
						Zeal::EqStructures::RaidMember member = raidMembers[i];
						if ((member.GroupNumber == 0xFFFFFFFF - 1) || (strlen(member.Name) == 0) || (strcmp(member.Name, Zeal::EqGame::get_self()->Name) == 0))
							continue;
						Zeal::EqStructures::Entity* raidMember = ZealService::get_instance()->entity_manager->Get(member.Name);
						if (!raidMember)
							continue;
						if (spawn == raidMember) {
							//self->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;
							raidMember->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;
							return;
						}
						else if (spawn == self) {
							self->ActorInfo->DagHeadPoint->StringSprite->Color = Raidcolor;
							return;
						}
					}
				}
			}			
			//If not in a Guild - keep default color  //If spawn is same Guild as Player
			if (spawn->GuildId != 0xFFFF && spawn->GuildId == Zeal::EqGame::get_self()->GuildId) {//Guild Member
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Guildcolor;
				return;
			}
			return;
		}
		break;
	case 1: //NPC
		if (nameplateconColors) {
			if (spawn == Zeal::EqGame::get_target()) //Leave blinking indicator on target
				return;
			if (spawn != Zeal::EqGame::get_self()) //All NPC entities
				spawn->ActorInfo->DagHeadPoint->StringSprite->Color = Zeal::EqGame::GetLevelCon(spawn); //Level Con Color for NPCs
		}
		break;
	case 2: //NPC Corpse
		break;
	case 3: //Player Corpse
		break;
	default:
		break;
	}
}

int __fastcall SetNameSpriteTint(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn)
{	
	int result = ZealService::get_instance()->hooks->hook_map["SetNameSpriteTint"]->original(SetNameSpriteTint)(this_ptr, not_used, spawn);
	ZealService::get_instance()->nameplate->HandleTint(spawn);
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

NamePlate::NamePlate(ZealService* zeal, IO_ini* ini)
{
	//HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");

	//remove unused hook for now
	//if (eqfx) 
		//zeal->hooks->Add("DeferCachedNameTagTextW", (DWORD)eqfx + 0x70A00, DeferCachedNameTagTextW, hook_type_detour);

	zeal->hooks->Add("SetNameSpriteTint", 0x4B114D, SetNameSpriteTint, hook_type_detour);
	
	if (!ini->exists("Zeal", "NameplateColors")) 
		ini->setValue<bool>("Zeal", "NameplateColors", false);
	if (!ini->exists("Zeal", "NameplateConColors"))
		ini->setValue<bool>("Zeal", "NameplateConColors", false);

	nameplateColors = ini->getValue<bool>("Zeal", "NameplateColors");
	nameplateconColors = ini->getValue<bool>("Zeal", "NameplateConColors");

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
}

NamePlate::~NamePlate()
{
}
