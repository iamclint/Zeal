#include "nameplate.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"

bool nameplatecolorsEnabled = true;
bool nameplatconcolorsEnabled = true;

struct CachedNameData
{
	int unk1;
	int unk2;
	int name_len;
	int unk3;
	int fontptr;
	char name[30];
};

int DeferCachedNameTagTextW(int font_size, CachedNameData* cnd, int a1, int a2, int a3, int a4, double a5, int a6)
{
	//if (GetAsyncKeyState(VK_END))
	//{
	//	Zeal::EqGame::print_chat("font: [%i] name: [%s] a1: [%i] a2: [%i] a3: [%i] a4: [%i] a5: [%f] a6: [%i]", font_size, cnd->name, a1, a2, a3, a4, a5, a6);
	//}
	//if (Zeal::EqGame::get_self())
	//{
	//	std::string name = cnd->name;
	//	name = Zeal::String::split(name, " ").front();
	//	std::string me = Zeal::EqGame::get_self()->Name;
	//	me = Zeal::String::trim_and_reduce_spaces(me);
	//	if (name == me)
	//	{
	//		cnd->unk1 = 0;
	//	}
	//	return ZealService::get_instance()->hooks->hook_map["DeferCachedNameTagTextW"]->original(DeferCachedNameTagTextW)(font_size, cnd, a1, a2, a3, a4, a5, a6);
	//}
	//else
	//{
		return ZealService::get_instance()->hooks->hook_map["DeferCachedNameTagTextW"]->original(DeferCachedNameTagTextW)(font_size, cnd, a1, a2, a3, a4, a5, a6);
	//}
}

int __fastcall SetNameSpriteTint(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn)
{	
	int result = ZealService::get_instance()->hooks->hook_map["SetNameSpriteTint"]->original(SetNameSpriteTint)(this_ptr, not_used, spawn);
	if (spawn == NULL) { return result; }
	if (spawn->ActorInfo->DagHeadPoint == NULL) { return result; }
	if (spawn->ActorInfo->DagHeadPoint->StringSprite == NULL) { return result; }
	switch (spawn->Type) {
		case 0: //Players
			if (nameplatecolorsEnabled) {
				int maxGroupMembers = 5;
				int maxRaidMembers = 72;
				Zeal::EqStructures::Entity** groupmembers = reinterpret_cast<Zeal::EqStructures::Entity**>(Zeal::EqGame::GroupList);
				Zeal::EqStructures::RaidMember* raidMembers = reinterpret_cast<Zeal::EqStructures::RaidMember*>(Zeal::EqGame::RaidMemberList);
				if (spawn == Zeal::EqGame::get_target()) //Leave blinking indicator on target
					return result;
				if (raidMembers){
					for (int i = 0; i < maxRaidMembers; ++i) //Raid Member loop
					{
						Zeal::EqStructures::RaidMember member = raidMembers[i];
						if ((member.GroupNumber == 0xFFFFFFFE) || (strlen(member.Name) == 0) || (strcmp(member.Name, Zeal::EqGame::get_self()->Name) == 0))
							continue;
						Zeal::EqStructures::Entity* raidMember = ZealService::get_instance()->entity_manager->Get(member.Name);
						if (!raidMember)
							continue;
						if (spawn == raidMember)
							raidMember->ActorInfo->DagHeadPoint->StringSprite->Color = 0xFF000000; //Raid Member-Black
					}
				}
				if (spawn->GuildId == Zeal::EqGame::get_self()->GuildId) //Guild Member
					spawn->ActorInfo->DagHeadPoint->StringSprite->Color = 0xFF00FF80; //Guild Member-Greenish Blue
					//spawn->ActorInfo->DagHeadPoint->StringSprite->Color = 0xFF80FF80; //Guild Member-White Green
				if (groupmembers) {
					for (int i = 0; i < maxGroupMembers; ++i) //Group Member loop
					{
						Zeal::EqStructures::Entity* groupmember = groupmembers[i];
						if (spawn == groupmember)
							spawn->ActorInfo->DagHeadPoint->StringSprite->Color = 0x0500FF32; //Group Member-Light Green 
						continue;
					}
				}
				if (spawn->IsLinkDead == 1) //LinkDead
					spawn->ActorInfo->DagHeadPoint->StringSprite->Color = 0xFFFF0000; //LinkDead - Red
				else if (spawn->IsAwayFromKeyboard == 1) //AFK
					spawn->ActorInfo->DagHeadPoint->StringSprite->Color = 0xFFFF8000; //AFK - Orange	
			}
			break;
		case 1: //NPC
			if (nameplatconcolorsEnabled) {
				if (spawn == Zeal::EqGame::get_target()) //Leave blinking indicator on target
					return result;
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
	return result;
}

void NamePlate::colors_set_enabled(bool _enabled)
{
	_enabled = !nameplatecolorsEnabled;
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateColors", _enabled);
	Zeal::EqGame::print_chat("NameplateColors are %s", _enabled ? "Enabled" : "Disabled");
	nameplatecolorsEnabled = !nameplatecolorsEnabled;
}

void NamePlate::con_colors_set_enabled(bool _enabled)
{
	_enabled = !nameplatconcolorsEnabled;
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "NameplateConColors", _enabled);
	Zeal::EqGame::print_chat("NameplateConColors are %s", _enabled ? "Enabled" : "Disabled");
	nameplatconcolorsEnabled = !nameplatconcolorsEnabled;
}

NamePlate::NamePlate(ZealService* zeal, IO_ini* ini)
{
	HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");
	if (eqfx)
		zeal->hooks->Add("DeferCachedNameTagTextW", (DWORD)eqfx + 0x70A00, DeferCachedNameTagTextW, hook_type_detour);
		if (nameplatecolorsEnabled)
		zeal->hooks->Add("SetNameSpriteTint", 0x4B114D, SetNameSpriteTint, hook_type_detour);
	
	if (!ini->exists("Zeal", "NameplateColors")) 
		ini->setValue<bool>("Zeal", "NameplateColors", false);
	if (ini->exists("Zeal", "NameplateColors"))
		nameplatecolorsEnabled = ini->getValue<bool>("Zeal", "NameplateColors");
	if (!ini->exists("Zeal", "NameplateConColors"))
		ini->setValue<bool>("Zeal", "NameplateConColors", false);
	if (ini->exists("Zeal", "NameplateConColors"))
		nameplatecolorsEnabled = ini->getValue<bool>("Zeal", "NameplateConColors");

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
