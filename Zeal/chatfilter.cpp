#include "Zeal.h"
#include "chatfilter.h"
#include "EqUI.h"
#include "EqAddresses.h"
#include "EqFunctions.h"

#define RANDOM 0x1000000
#define LOOT  0x1000001
#define EF_LEN 2

#define ChannelMap0 0
#define ChannelMap40 0x28
#define FILTER_OFFSET 0x64

int32_t __fastcall ColorToChannelMap(int this_, int u, int filter)
{
	uint32_t channelMap;
	switch (filter)
	{
  case USERCOLOR_SAY:
  case USERCOLOR_ECHO_SAY:
    channelMap = 0;
    break;
  case USERCOLOR_TELL:
  case USERCOLOR_ECHO_TELL:
    channelMap = 1;
    break;
  case USERCOLOR_GROUP:
  case USERCOLOR_ECHO_GROUP:
    channelMap = 2;
    break;
  case USERCOLOR_GUILD:
  case USERCOLOR_ECHO_GUILD:
    channelMap = 4;
    break;
  case USERCOLOR_OOC:
  case USERCOLOR_ECHO_OOC:
    channelMap = 5;
    break;
  case USERCOLOR_AUCTION:
  case USERCOLOR_ECHO_AUCTION:
    channelMap = 6;
    break;
  case USERCOLOR_SHOUT:
  case USERCOLOR_ECHO_SHOUT:
    channelMap = 7;
    break;
  case USERCOLOR_EMOTE:
  case USERCOLOR_ECHO_EMOTE:
    channelMap = 8;
    break;
  case USERCOLOR_SPELLS:
    channelMap = 10;
    break;
  case USERCOLOR_YOU_HIT_OTHER:
    channelMap = 9;
    break;
  case USERCOLOR_OTHER_HIT_YOU:
    channelMap = 0x18;
    break;
  case USERCOLOR_YOU_MISS_OTHER:
    channelMap = 0x17;
    break;
  case USERCOLOR_OTHER_MISS_YOU:
    channelMap = 0x19;
    break;
  case USERCOLOR_SKILLS:
    channelMap = 0xb;
    break;
  case USERCOLOR_DISCIPLINES:
    channelMap = 0x1f;
    break;
  case USERCOLOR_YOUR_DEATH:
    channelMap = 0x1c;
    break;
  case USERCOLOR_OTHER_DEATH:
    channelMap = 0x1d;
    break;
  case USERCOLOR_OTHER_HIT_OTHER:
    channelMap = 0x1a;
    break;
  case USERCOLOR_OTHER_MISS_OTHER:
    channelMap = 0x1b;
    break;
  case USERCOLOR_NON_MELEE:
    channelMap = 0x28;
    break;
  case USERCOLOR_SPELL_WORN_OFF:
    channelMap = 0x27;
    break;
  case USERCOLOR_OTHERS_SPELLS:
    channelMap = 0x24;
    break;
  case USERCOLOR_SPELL_FAILURE:
    channelMap = 0x25;
    break;
  case USERCOLOR_CHAT_1:
  case USERCOLOR_ECHO_CHAT_1:
    channelMap = 0xc;
    break;
  case USERCOLOR_CHAT_2:
  case USERCOLOR_ECHO_CHAT_2:
    channelMap = 0xd;
    break;
  case USERCOLOR_CHAT_3:
  case USERCOLOR_ECHO_CHAT_3:
    channelMap = 0xe;
    break;
  case USERCOLOR_CHAT_4:
  case USERCOLOR_ECHO_CHAT_4:
    channelMap = 0xf;
    break;
  case USERCOLOR_CHAT_5:
  case USERCOLOR_ECHO_CHAT_5:
    channelMap = 0x10;
    break;
  case USERCOLOR_CHAT_6:
  case USERCOLOR_ECHO_CHAT_6:
    channelMap = 0x11;
    break;
  case USERCOLOR_CHAT_7:
  case USERCOLOR_ECHO_CHAT_7:
    channelMap = 0x12;
    break;
  case USERCOLOR_CHAT_8:
  case USERCOLOR_ECHO_CHAT_8:
    channelMap = 0x13;
    break;
  case 299:
  case USERCOLOR_ECHO_CHAT_9:
    channelMap = 0x14;
    break;
  case 300:
  case USERCOLOR_ECHO_CHAT_10:
    channelMap = 0x15;
    break;
  case USERCOLOR_MELEE_CRIT:
    channelMap = 0x1e;
    break;
  case USERCOLOR_SPELL_CRIT:
    channelMap = 0x26;
    break;
  case USERCOLOR_TOO_FAR_AWAY:
    channelMap = 0x20;
    break;
  case USERCOLOR_NPC_RAMAGE:
    channelMap = 0x21;
    break;
  case USERCOLOR_NPC_FURRY:
    channelMap = 0x22;
    break;
  case USERCOLOR_NPC_ENRAGE:
    channelMap = 0x23;
    break;
  case 0x147:
    channelMap = 3;
    break;
  default:  // The dreaded 'Other' Category
    channelMap = 0x16;
    break;
  case USERCOLOR_RANDOM:
    channelMap = RANDOM;
    break;
  case USERCOLOR_LOOT:
    channelMap = LOOT;
    break;
  }
  return channelMap;
}

void __fastcall ClearChannelMaps(Zeal::EqUI::CChatManager* cman, int u, int window)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

    if (window != (int)cman->ChatWindows[0])
    {
        for (int i = 0; i < EF_LEN; i++)
        {
            if (cf->extendedChannelMaps[i] == window)
            {
                cf->extendedChannelMaps[i] = (int)cman->ChatWindows[0];
            }
        }
        for (int i = 0; i <= ChannelMap40; i++)
        {
            if (cman->ChannelMapWnd[i] == window)
            {
                cman->ChannelMapWnd[i] = (int)cman->ChatWindows[0];
            }
        }
    }
}

void __fastcall ClearChannelMap(Zeal::EqUI::CChatManager* cman, int u, int filter)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    if (filter == (RANDOM - FILTER_OFFSET) || filter == (LOOT - FILTER_OFFSET))
    {
        int index = filter - (0x1000000 - FILTER_OFFSET);
        cf->extendedChannelMaps[index] = (int)cman->ChatWindows[0];
    }
    else if (filter <= ChannelMap40 and filter >= ChannelMap0)
    {
        cman->ChannelMapWnd[filter] = (int)cman->ChatWindows[0];
    }
}

//Return a Window handle for a filter. We'll need to store our own references here.
int  __fastcall GetChannelMap(Zeal::EqUI::CChatManager* cman, int u, int filter)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    int windowHandle = 0;

    if (filter == (RANDOM - FILTER_OFFSET) || filter == (LOOT - FILTER_OFFSET))
    {
        int index = filter - (0x1000000 - FILTER_OFFSET);
        return cf->extendedChannelMaps[index];

    }
    else if ((filter >= ChannelMap0) && (filter <= ChannelMap40)) {
        windowHandle = cman->ChannelMapWnd[filter];
    } 
    return windowHandle;
}

void __fastcall SetChannelMap(Zeal::EqUI::CChatManager* cman, int u, int filter, int window)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    
    if (filter == (RANDOM - FILTER_OFFSET) || filter == (LOOT- FILTER_OFFSET))
    {
        int index = filter - (0x1000000 - FILTER_OFFSET);
        cf->extendedChannelMaps[index] = window;
    }
    else if ((filter >= ChannelMap0) && (filter <= ChannelMap40)) {
        cman->ChannelMapWnd[filter] = window;
    }
}

__declspec (naked) void FilterConditional(void)
{
    __asm {
        cmp ebx, RANDOM
        je validFilter
        cmp ebx, LOOT
        je validFilter
        cmp ebx, 8Ch
        jg invalidFilter
    validFilter:
        mov eax, 414123h
        jmp eax
    invalidFilter:
        mov eax, 41426Fh
        jmp eax
    }
}

int SelectWindow(int this_, int ChannelMap)
{
    _asm //Save ECX
    {
        push ecx
    }

    int window = 0;

    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

    if (ChannelMap >= ChannelMap0 && ChannelMap <= ChannelMap40)
    {
        window = this_ + 0x90 + ChannelMap * 4;
    }
    else if (ChannelMap == RANDOM || ChannelMap == LOOT)
    {
        int index = ChannelMap - 0x1000000;
        window = (int)&cf->extendedChannelMaps[index];
    }

    _asm
    {
        pop ecx
    }
    return window;
}

int32_t __fastcall AddMenu(int this_, int u, Zeal::EqUI::ContextMenu* menu)
{
	Zeal::EqUI::CXSTR rand("Random");
	menu->AddMenuItem(rand, RANDOM);

	Zeal::EqUI::CXSTR loot("Loot");
	menu->AddMenuItem(loot, LOOT);

	return ZealService::get_instance()->hooks->hook_map["AddMenu"]->original(AddMenu)(this_, u, menu);
}

void chatfilter::LoadSettings()
{
    std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
    IO_ini ui_ini(ini_name);
    std::string cmap = "ChannelMap";
    int num = 41;

    for (int i = 0; i < EF_LEN; i++)
    {
        std::string this_cmap = cmap + std::to_string(num + i);
        if (!ui_ini.exists("ChatManager", this_cmap))
        {
            ui_ini.setValue<int>("ChatManager", this_cmap, 0);
        }
        else
        {
            extendedChannelMaps[i] = ui_ini.getValue<int>("ChatManager", this_cmap);
        }
    }
}

int __fastcall CChatManager(Zeal::EqUI::CChatManager* cman, int u)
{
    int  retVal = ZealService::get_instance()->hooks->hook_map["CChatManager"]->original(CChatManager)(cman, u);

    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    cf->LoadSettings();

    for (int i = 0; i < EF_LEN; i++)
    {
        int window_index = cf->extendedChannelMaps[i];
        cf->extendedChannelMaps[i] = (int)cman->ChatWindows[window_index];
    }
    return retVal;
}

void __fastcall Deactivate(Zeal::EqUI::CChatManager* cman, int u)
{
    std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
    IO_ini ui_ini(ini_name);
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    std::string cmap = "ChannelMap";
    int num = 41;

    for (int i = 0; i < EF_LEN; i++)
    {
        int window_pos = 0;
        for (int j = 0; j < cman->MaxChatWindows; j++)
        {
            if ((int)cman->ChatWindows[j] == cf->extendedChannelMaps[i])
            {
                std::string this_cmap = cmap + std::to_string(num + i);
                ui_ini.setValue<int>("ChatManager", this_cmap, j);
                break;
            }
        }
    }

    ZealService::get_instance()->hooks->hook_map["Deactivate"]->original(Deactivate)(cman, u);
}


chatfilter::chatfilter(ZealService* zeal, IO_ini* ini)
{  
    zeal->hooks->Add("CChatManager", 0x4100e2, CChatManager, hook_type_detour);
    zeal->hooks->Add("Deactivate", 0x410871, Deactivate, hook_type_detour);
	zeal->hooks->Add("AddMenu", 0x4120DD, AddMenu, hook_type_replace_call);
	zeal->hooks->Add("GetChannelMap", 0x41161D, GetChannelMap, hook_type_detour);
    zeal->hooks->Add("SetChannelMap", 0x4113F1, SetChannelMap, hook_type_detour);
    zeal->hooks->Add("ClearChannelMap", 0x41140C, ClearChannelMap, hook_type_detour);
    zeal->hooks->Add("ClearChannelMaps", 0x411638, ClearChannelMaps, hook_type_detour);
    zeal->hooks->Add("ColorToChannelMap", 0x411173, ColorToChannelMap, hook_type_replace_call);

    //ChatWindow::WndNotification Conditional Patch
    mem::write<byte[2]>(0x414117, { 0x8d, 0x05 });  //lea eax
    mem::write<int>(0x414119, (int)FilterConditional); 
    mem::write<byte[2]>(0x41411D, { 0xFF, 0xE0 });  //jmp eax
    mem::write<byte[4]>(0x41411F, { 0x90, 0x90, 0x90, 0x90 });

    //ChatManager::AddText replace direct dereference
    mem::write<byte>(0x411178, 0x50);    //push eax (ChannelMap)
    mem::write<byte>(0x411179, 0x56);    //push esi (this)
    mem::write<byte[2]>(0x41117A, { 0x8d, 0x05 }); //lea eax
    mem::write<int>(0x41117C, (int)SelectWindow);
    mem::write<byte[2]>(0x411180, { 0xFF, 0xD0 });  //call eax
    mem::write<byte[2]>(0x411182, { 0x89, 0xC6 });  //mov esi, eax
    mem::write<byte[3]>(0x411184, { 0x83, 0xFE, 0x00 }); //cmp esi, 0x0
}

chatfilter::~chatfilter()
{
}