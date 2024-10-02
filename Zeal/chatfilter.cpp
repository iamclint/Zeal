#include "Zeal.h"
#include "chatfilter.h"
#include "EqUI.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include <regex>
//Standard ChannelMaps and filter offset
#define ChannelMap0 0
#define ChannelMap40 0x28
#define FILTER_OFFSET 0x64

bool chatfilter::isExtendedCM(int channelMap, int applyOffset)
{
    for (auto& ec : Extended_ChannelMaps)
    {
        if ((channelMap + applyOffset) == ec.channelMap)
        {
            return true;
        }
    }
    return false;
}

bool chatfilter::isStandardCM(int channelMap, int applyOffset)
{
    channelMap = channelMap + applyOffset;
    if (channelMap >= ChannelMap0 && channelMap <= ChannelMap40)
    {
        return true;
    }
    return false;
}

void __fastcall ClearChannelMaps(Zeal::EqUI::CChatManager* cman, int u, Zeal::EqUI::ChatWnd* window)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

    if (window != cman->ChatWindows[0])
    {
        for (auto& ec : cf->Extended_ChannelMaps)
        {
            if (ec.windowHandle == window)
            {
                ec.windowHandle = cman->ChatWindows[0];
            }
        }
        for (int i = 0; i <= ChannelMap40; i++)
        {
            if (cman->ChannelMapWnd[i] == window)
            {
                cman->ChannelMapWnd[i] = cman->ChatWindows[0];
            }
        }
    }
}

void __fastcall ClearChannelMap(Zeal::EqUI::CChatManager* cman, int u, int filter)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    if (cf->isExtendedCM(filter, FILTER_OFFSET))
    {
        int index = filter - (0x10000 - FILTER_OFFSET);
        cf->Extended_ChannelMaps.at(index).windowHandle = cman->ChatWindows[0];
    }
    else if (cf->isStandardCM(filter))
    {
        cman->ChannelMapWnd[filter] = cman->ChatWindows[0];
    }
}

//Return a Window handle for a filter. We'll need to store our own references here.
Zeal::EqUI::ChatWnd* __fastcall GetChannelMap(Zeal::EqUI::CChatManager* cman, int u, int filter)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    Zeal::EqUI::ChatWnd* windowHandle = 0;

    if (cf->isExtendedCM(filter, FILTER_OFFSET))
    {
        int index = filter - (0x10000 - FILTER_OFFSET);
        windowHandle = cf->Extended_ChannelMaps.at(index).windowHandle;

    }
    else if (cf->isStandardCM(filter)) {
        windowHandle = cman->ChannelMapWnd[filter];
    } 
    return windowHandle;
}

void __fastcall SetChannelMap(Zeal::EqUI::CChatManager* cman, int u, int filter, Zeal::EqUI::ChatWnd* window)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    
    if (cf->isExtendedCM(filter, FILTER_OFFSET))
    {
        int index = filter - (0x10000 - FILTER_OFFSET);
        cf->Extended_ChannelMaps.at(index).windowHandle = window; 
    }
    else if (cf->isStandardCM(filter)) {
        cman->ChannelMapWnd[filter] = window;
    }
}

__declspec (naked) void FilterConditional(void)
{
    __asm pushad

    DWORD filterID;
    chatfilter* cf;

    cf = ZealService::get_instance()->chatfilter_hook.get();

    __asm mov filterID, ebx
    if (cf->isExtendedCM(filterID) || cf->isStandardCM(filterID,-FILTER_OFFSET))
    {
        __asm
        {
            popad
            mov eax, 414123h
            jmp eax
        }
    }
    else
    {
        __asm
        {
            popad
            mov eax, 41426Fh
            jmp eax
        }
    }
}

int32_t __fastcall AddMenu(int this_, int u, Zeal::EqUI::ContextMenu* menu)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

    menu->AddSeparator();

    for (auto& ec : cf->Extended_ChannelMaps)
    {
        Zeal::EqUI::CXSTR cstr(ec.name);
        menu->AddMenuItem(cstr, ec.channelMap);
    }

	return ZealService::get_instance()->hooks->hook_map["AddMenu"]->original(AddMenu)(this_, u, menu);
}

void chatfilter::LoadSettings(Zeal::EqUI::CChatManager* cman)
{
    std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
    IO_ini ui_ini(ini_name);
    std::string cmap = "ChannelMap";
    int num = 41;

    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

    for (int i = 0; auto& filter : cf->Extended_ChannelMaps)
    {
        std::string this_cmap = cmap + std::to_string(num + i);
        if (!ui_ini.exists("ChatManager", this_cmap))
        {
            ui_ini.setValue<int>("ChatManager", this_cmap, 0);
        }
        filter.windowHandle = cman->ChatWindows[ui_ini.getValue<int>("ChatManager", this_cmap)];
        i++;
    }
}

int __fastcall CChatManager(Zeal::EqUI::CChatManager* cman, int u)
{
    int  retVal = ZealService::get_instance()->hooks->hook_map["CChatManager"]->original(CChatManager)(cman, u);

    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    cf->LoadSettings(cman);

    return retVal;
}

void __fastcall Deactivate(Zeal::EqUI::CChatManager* cman, int u)
{
    std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
    IO_ini ui_ini(ini_name);
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    std::string cmap = "ChannelMap";
    int num = 41;
    for (int i = 0; auto& filter : cf->Extended_ChannelMaps)
    {
        for (int window_index = 0; window_index < cman->MaxChatWindows; window_index++)
        {
            if (filter.windowHandle==cman->ChatWindows[window_index])
            {
                std::string this_cmap = cmap + std::to_string(num + i);
                ui_ini.setValue<int>("ChatManager", this_cmap, window_index);
                break;
            }
        }
        i++;
    }
    ZealService::get_instance()->hooks->hook_map["Deactivate"]->original(Deactivate)(cman, u);
}

void chatfilter::AddOutputText(Zeal::EqUI::ChatWnd*& wnd, std::string msg, short channel)
{
    for (auto& filter : Extended_ChannelMaps)
    {
        if (filter.isHandled(channel, msg))
            wnd = filter.windowHandle;
    }
    isDamage = false;
}

//std::string GetNameDamage(std::string& data)
//{
//    std::string lower_msg = data;
//    //std::transform(lower_msg.begin(), lower_msg.end(), lower_msg.begin(), ::tolower);
//
//    std::regex damage_pattern(R"((?:^|\]\s*)(\b\w+\b)+[\w\W]+(?:points of damage|but misses))");
//    std::smatch match;
//
//    if (std::regex_search(lower_msg, match, damage_pattern))
//        return match[1].str(); 
//    return "";
//}
void __fastcall PrintSplit(int t, int unused, const char* data, short color_index, bool u)
{
    ZealService::get_instance()->hooks->hook_map["PrintSplit"]->original(PrintSplit)(t, unused, data, USERCOLOR_MONEY_SPLIT, u);
}

void __fastcall PrintAutoSplit(int t, int unused, const char* data, short color_index, bool u)
{
    ZealService::get_instance()->hooks->hook_map["PrintAutoSplit"]->original(PrintAutoSplit)(t, unused, data, USERCOLOR_ECHO_AUTOSPLIT, u);
}

chatfilter::chatfilter(ZealService* zeal, IO_ini* ini)
{
    zeal->callbacks->AddReportSuccessfulHit([this](Zeal::EqStructures::Entity* source, Zeal::EqStructures::Entity* target, WORD type, short spell_id, short damage, int heal) { isDamage = true; damageData = { source, target, type, spell_id, damage, heal }; });
    Extended_ChannelMaps.push_back(CustomFilter("Random", 0x10000, [this](short color, std::string data) { return color == USERCOLOR_RANDOM; }));
    Extended_ChannelMaps.push_back(CustomFilter("Loot", 0x10001, [this](short color, std::string data) { return color == USERCOLOR_LOOT; }));
    Extended_ChannelMaps.push_back(CustomFilter("Money", 0x10002, [this](short color, std::string data) { return color == USERCOLOR_MONEY_SPLIT || color == USERCOLOR_ECHO_AUTOSPLIT; }));
    Extended_ChannelMaps.push_back(CustomFilter("My Pet", 0x10003, [this, zeal](short color, std::string data)
        {
            if (isDamage && damageData.source && damageData.source->PetOwnerSpawnId && damageData.source->PetOwnerSpawnId == Zeal::EqGame::get_self()->SpawnId)
                return true;
            if (isDamage && damageData.target && damageData.target->PetOwnerSpawnId && damageData.target->PetOwnerSpawnId == Zeal::EqGame::get_self()->SpawnId)
                return true;
            return false;
        }));
    Extended_ChannelMaps.push_back(CustomFilter("Other Pets", 0x10004, [this, zeal](short color, std::string data)
        {
            if (isDamage && damageData.source && damageData.source->PetOwnerSpawnId && damageData.source->PetOwnerSpawnId != Zeal::EqGame::get_self()->SpawnId)
                return true;
            if (isDamage && damageData.target && damageData.target->PetOwnerSpawnId && damageData.target->PetOwnerSpawnId != Zeal::EqGame::get_self()->SpawnId)
                return true;
            return false;
        }));

    zeal->callbacks->AddOutputText([this](Zeal::EqUI::ChatWnd*& wnd, std::string msg, short channel) { this->AddOutputText(wnd, msg, channel); });

    zeal->hooks->Add("CChatManager", 0x4100e2, CChatManager, hook_type_detour);
    zeal->hooks->Add("Deactivate", 0x410871, Deactivate, hook_type_detour);
	zeal->hooks->Add("AddMenu", 0x4120DD, AddMenu, hook_type_replace_call);
	zeal->hooks->Add("GetChannelMap", 0x41161D, GetChannelMap, hook_type_detour);
    zeal->hooks->Add("SetChannelMap", 0x4113F1, SetChannelMap, hook_type_detour);
    zeal->hooks->Add("ClearChannelMap", 0x41140C, ClearChannelMap, hook_type_detour);
    zeal->hooks->Add("ClearChannelMaps", 0x411638, ClearChannelMaps, hook_type_detour);
    zeal->hooks->Add("PrintSplit", 0x54755b, PrintSplit, hook_type_replace_call); //fix up money split
    zeal->hooks->Add("PrintAutoSplit", 0x4FB477, PrintAutoSplit, hook_type_replace_call); //fix up money split
    //zeal->hooks->Add("ColorToChannelMap", 0x411173, ColorToChannelMap, hook_type_replace_call);

    //ChatWindow::WndNotification Conditional Patch
    mem::write<byte[2]>(0x414117, { 0x8d, 0x05 });  //lea eax
    mem::write<int>(0x414119, (int)FilterConditional); 
    mem::write<byte[2]>(0x41411D, { 0xFF, 0xE0 });  //jmp eax
    mem::write<byte[4]>(0x41411F, { 0x90, 0x90, 0x90, 0x90 });

    //ChatManager::AddText replace direct dereference
    //mem::write<byte>(0x411178, 0x50);    //push eax (ChannelMap)
    //mem::write<byte>(0x411179, 0x56);    //push esi (this)
    //mem::write<byte[2]>(0x41117A, { 0x8d, 0x05 }); //lea eax
    //mem::write<int>(0x41117C, (int)SelectWindow);
    //mem::write<byte[2]>(0x411180, { 0xFF, 0xD0 });  //call eax
    //mem::write<byte[2]>(0x411182, { 0x89, 0xC6 });  //mov esi, eax
    //mem::write<byte[3]>(0x411184, { 0x83, 0xFE, 0x00 }); //cmp esi, 0x0
}

chatfilter::~chatfilter()
{
}