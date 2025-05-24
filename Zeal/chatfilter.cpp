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

static Zeal::EqUI::ContextMenu* InitializeMenu(void* notificationFunc = nullptr) {
    auto menu = Zeal::EqUI::ContextMenu::Create(0, 0, { 100, 100, 100, 100 });
    if (!menu)
        throw std::bad_alloc();

    menu->HasChildren = true;  // Note: Evaluate if these are still necessary.
    menu->HasSiblings = true;
    menu->Unknown0x015 = 0;
    menu->Unknown0x016 = 0;
    menu->Unknown0x017 = 0;
    menu->fnTable->WndNotification = notificationFunc;
    return menu;
}

int32_t __fastcall AddMenu(int this_, int u, Zeal::EqUI::ContextMenu* menu)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    cf->ZealMenu = InitializeMenu();

    for (auto& ec : cf->Extended_ChannelMaps)
        cf->ZealMenu->AddMenuItem(ec.name, ec.channelMap);
    cf->menuIndex = Zeal::EqGame::Windows->ContextMenuManager->AddMenu(cf->ZealMenu);

    menu->AddMenuItem("Zeal", cf->menuIndex, true, true);

	return ZealService::get_instance()->hooks->hook_map["AddMenu"]->original(AddMenu)(this_, u, menu);
}

void chatfilter::LoadSettings(Zeal::EqUI::CChatManager* cman)
{
    std::string ini_name = ZealService::get_instance()->ui->GetUIIni();
    if (ini_name.length())
    {
        IO_ini ui_ini(ini_name);
        std::string cmap = "ChannelMap";
        int num = 41;

        chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

        for (int i = 0; auto & filter : cf->Extended_ChannelMaps)
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
}

int __fastcall CChatManager(Zeal::EqUI::CChatManager* cman, int u)
{
    int  retVal = ZealService::get_instance()->hooks->hook_map["CChatManager"]->original(CChatManager)(cman, u);

    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    cf->LoadSettings(cman);

    return retVal;
}

void __fastcall UpdateContextMenus(Zeal::EqUI::CChatManager* cman, int u, Zeal::EqUI::ChatWnd * window)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

    if (cf->ZealMenu)
    {
        for (int i = 0; i < cf->Extended_ChannelMaps.size(); i++)
        {
            Zeal::EqUI::ChatWnd* mapped = cf->Extended_ChannelMaps.at(i).windowHandle;
            cf->ZealMenu->CheckMenuItem(i, mapped == window);
        }
    }
    ZealService::get_instance()->hooks->hook_map["UpdateContextMenus"]->original(UpdateContextMenus)(cman, u, window);
}

void __fastcall Deactivate(Zeal::EqUI::CChatManager* cman, int u)
{
    std::string ini_name = ZealService::get_instance()->ui->GetUIIni();
    if (ini_name.length())
    {
        IO_ini ui_ini(ini_name);
        chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
        std::string cmap = "ChannelMap";
        int num = 41;
        for (int i = 0; auto & filter : cf->Extended_ChannelMaps)
        {
            for (int window_index = 0; window_index < cman->MaxChatWindows; window_index++)
            {
                if (filter.windowHandle == cman->ChatWindows[window_index])
                {
                    std::string this_cmap = cmap + std::to_string(num + i);
                    ui_ini.setValue<int>("ChatManager", this_cmap, window_index);
                    break;
                }
            }
            i++;
        }
    }
    ZealService::get_instance()->hooks->hook_map["Deactivate"]->original(Deactivate)(cman, u);
}

void chatfilter::callback_clean_ui()
{
    if (ZealMenu and menuIndex != -1)
    {
        Zeal::EqGame::Windows->ContextMenuManager->RemoveMenu(menuIndex, true);
    }
    menuIndex = -1;
    ZealMenu = NULL;
}


void chatfilter::AddOutputText(Zeal::EqUI::ChatWnd*& wnd, std::string msg, short& channel)
{
    for (auto& filter : Extended_ChannelMaps)
    {
        if (filter.isHandled(channel, msg))
            wnd = filter.windowHandle;
    }
}

void __fastcall whoGlobalPrintChat_wrapped(int t, int unused, const char* data)
{
    Zeal::EqGame::print_chat(USERCOLOR_WHO, data);
}

void __fastcall whoGlobalPrintChat_full(int t, int unused, const char* data, short color, bool un)
{
    Zeal::EqGame::print_chat(USERCOLOR_WHO, data);
}

void __fastcall PrintSplit(int t, int unused, const char* data, short color_index, bool u)
{
    ZealService::get_instance()->hooks->hook_map["PrintSplit"]->original(PrintSplit)(t, unused, data, USERCOLOR_MONEY_SPLIT, u);
}

void __fastcall PrintAutoSplit(int t, int unused, const char* data, short color_index, bool u)
{
    ZealService::get_instance()->hooks->hook_map["PrintAutoSplit"]->original(PrintAutoSplit)(t, unused, data, USERCOLOR_ECHO_AUTOSPLIT, u);
}

static void HandleMyHitsMode(char* buffer, const char* s1, const char* s2, const char* s3, int damage)
{
    // Support re-routing special melee attacks to the special chat color index.
    if (Zeal::EqGame::is_new_ui() && 
        (strcmp(s1, "backstab") == 0 ||
        strcmp(s1, "kick") == 0 ||
        strcmp(s1, "strike") == 0))
    {
        char* output = buffer;
        char new_buffer[512];
        int hitmode = Zeal::EqGame::Windows->ChatManager->MyHitsMode;
        if (hitmode == 1) {
            snprintf(new_buffer, sizeof(new_buffer), "%s %s for %d", s1, s2, damage);
            output = new_buffer;
        }
        else if (hitmode == 2) {
            snprintf(new_buffer, sizeof(new_buffer), "%d", damage);
            output = new_buffer;
        }

        if (hitmode >= 0 && hitmode <= 2)  // Print with logging disabled (for abbreviated cases).
            Zeal::EqGame::EqGameInternal::print_chat(*(int*)0x809478, 0, output, CHANNEL_MYMELEESPECIAL, false);

        if (*Zeal::EqGame::is_logging_enabled && damage > -0x29)  // Comparison copied from client code.
            Zeal::EqGame::log(buffer);

        return;
    }

    ZealService::get_instance()->hooks->hook_map["HandleMyHitsMode"]->original(HandleMyHitsMode)(buffer, s1, s2, s3, damage);
}

void HandleOtherHitsOtherMode(char* buffer, const char* s1, const char* s2, const char* s3, int damage)
{
    // Support re-routing special melee attacks to the special chat color index.
    if (Zeal::EqGame::is_new_ui() &&
        (strcmp(s1, "backstabs") == 0 ||
            strcmp(s1, "kicks") == 0 ||
            strcmp(s1, "strikes") == 0))
    {
        char* output = buffer;
        char new_buffer[512];
        int hitmode = Zeal::EqGame::Windows->ChatManager->OthersHitsMode;
        if (hitmode == 1) {
            snprintf(new_buffer, sizeof(new_buffer), "%s %s %s for %d", s3, s1, s2, damage);
            output = new_buffer;
        }
        else if (hitmode == 2) {
            snprintf(new_buffer, sizeof(new_buffer), "%d", damage);
            output = new_buffer;
        }

        if (hitmode >= 0 && hitmode <= 2)  // Print with logging disabled (for abbreviated cases).
            Zeal::EqGame::EqGameInternal::print_chat(*(int*)0x809478, 0, output, CHANNEL_OTHERMELEESPECIAL, false);

        if (*Zeal::EqGame::is_logging_enabled && damage > -0x29)  // Comparison copied from client code.
            Zeal::EqGame::log(buffer);

        return;
    }

    ZealService::get_instance()->hooks->hook_map["HandleOtherHitsOtherMode"]->original(HandleOtherHitsOtherMode)(buffer, s1, s2, s3, damage);
}

// Returns true if the fizzle message is not from a group member.
static bool is_non_group_fizzle(const char* data)
{
    if (!data)
        return false;  
    std::string message(data);
    const std::string suffix("'s spell fizzles!");
    if (!message.ends_with(suffix))
        return false;
    if (!Zeal::EqGame::GroupInfo->is_in_group())
        return true;
    std::string name = message.substr(0, message.length() - suffix.length());
    for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; i++)
        if (name == Zeal::EqGame::GroupInfo->Names[i])
            return false;
    return true;
}

void __fastcall serverPrintChat(int t, int unused, const char* data, short color_index, bool u)
{
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();
    if (cf->current_string_id == 1219 && cf->setting_suppress_missed_notes.get()
        && !strncmp("A missed note brings ", data, 21)) {
        cf->current_string_id = 0;
        return;  // Just drop missed note messages from others.
    }
    else if (cf->current_string_id == 1218 && cf->setting_suppress_other_fizzles.get()
        && is_non_group_fizzle(data)) {
        cf->current_string_id = 0;
        return;  // Just drop fizzles from others.
    }

    if (cf->isMyPetSay)
        color_index = CHANNEL_MYPETSAY;
    else if (cf->isPetMessage)
        color_index = CHANNEL_OTHERPETSAY;
    ZealService::get_instance()->hooks->hook_map["serverPrintChat"]->original(serverPrintChat)(t, unused, data, color_index, u);
    cf->isMyPetSay = false;
    cf->isPetMessage = false;
    cf->current_string_id = 0;
}

char* __fastcall serverGetString(int stringtable, int unused, int string_id, bool* valid)
{   
    char** args;
    //Steal a reference to the previous EBP so we can grab the WorldMessage arguments
    __asm
    {
        push edi
        mov edi, [ebp]
        lea edi, [edi - 0x348]
        mov args, edi
        pop edi
    }

    std::vector<int> pet_sayings{
    555, //%1 tells you, 'I am unable to wake %2, master.'
    5501 //%1 tells you, 'Attacking %2 Master.'
    };

    std::vector<int> pet_t2_strings{
    1130, //Changing position, Master.
    1131, //Sorry, Master..calming down.
    1132, //Following you, Master.
    1134, //Guarding with my life..oh splendid one.
    1135, //As you wish, oh great one.
    1136, //My leader is %3.
    1139, //I beg forgiveness, Master.  That is not a legal target.
    };

    char* name = args[0];
    char* t2_string = args[1];
    chatfilter* cf = ZealService::get_instance()->chatfilter_hook.get();

    cf->current_string_id = string_id;
    for (const int& i : pet_sayings)
    {
        if (string_id == i)
            cf->isMyPetSay = true;
    }

    if (!cf->isMyPetSay)
    {
        Zeal::EqStructures::Entity* pet = Zeal::EqGame::get_pet();
        if (pet)
        {
            const char* pet_name = Zeal::EqGame::strip_name(pet->Name);
            if (strcmp(name, pet_name) == 0)
                cf->isMyPetSay = true;
        }
    }

    //554, //%1 says '%T2'
    //We need to check the next string to see if it's a pet phrase
    if (string_id == 554)
    {
        int t2_string_id = std::stoi(t2_string);
        for (const int& i : pet_t2_strings)
        {
            if (t2_string_id == i)
            {
                cf->isPetMessage = true;
                break;
            }
        }
    }
    return ZealService::get_instance()->hooks->hook_map["serverGetString"]->original(serverGetString)(stringtable, unused, string_id, valid);
}


chatfilter::chatfilter(ZealService* zeal)
{
    zeal->hooks->Add("DamageOutputText", 0x52A8C1, CChatManager, hook_type_replace_call);
    zeal->callbacks->AddReportSuccessfulHit([this](Zeal::EqStructures::Entity* source, Zeal::EqStructures::Entity* target, WORD type, short spell_id, short damage, char output_text) { 
        if (output_text) { 
            isDamage = true; 
            damageData = { source, target, type, spell_id, damage }; 
        }
        if (spell_id > 0 && source != Zeal::EqGame::get_self() && target != Zeal::EqGame::get_self())
        {
            if (damage > 0)
            {
                isDamage = true;
                damageData = { source, target, type, spell_id, damage };
                if (source->Position.Dist2D(Zeal::EqGame::get_self()->Position) < 500 || target->Position.Dist2D(Zeal::EqGame::get_self()->Position) < 500)
                    Zeal::EqGame::print_chat(USERCOLOR_NON_MELEE, "%s hit %s for %i points of non-melee damage.", Zeal::EqGame::strip_name(source->Name), Zeal::EqGame::strip_name(target->Name), damage);
            }
            else if (damage < 0)
            {
                if (type >= 244 && type <= 249 && target->Type == Zeal::EqEnums::EntityTypes::NPC && target->PetOwnerSpawnId == 0) // Show DS Damage to NPCs (non-pets)
                {
                    if (source->Position.Dist2D(Zeal::EqGame::get_self()->Position) < 500 || target->Position.Dist2D(Zeal::EqGame::get_self()->Position) < 500)
                        Zeal::EqGame::print_chat(USERCOLOR_NON_MELEE, "%s hit %s for %i points of non-melee damage.", Zeal::EqGame::strip_name(source->Name), Zeal::EqGame::strip_name(target->Name), -damage);
                }
            }
        }
     });
    Extended_ChannelMaps.push_back(CustomFilter("Random", 0x10000, [this](short& color, std::string data) { return color == USERCOLOR_RANDOM; }));
    Extended_ChannelMaps.push_back(CustomFilter("Loot", 0x10001, [this](short& color, std::string data) { return color == USERCOLOR_LOOT; }));
    Extended_ChannelMaps.push_back(CustomFilter("Money", 0x10002, [this](short& color, std::string data) { return color == USERCOLOR_MONEY_SPLIT || color == USERCOLOR_ECHO_AUTOSPLIT; }));
    Extended_ChannelMaps.push_back(CustomFilter("My Pet Say", 0x10003, [this, zeal](short& color, std::string data) { return color == CHANNEL_MYPETSAY; }));
    Extended_ChannelMaps.push_back(CustomFilter("My Pet Damage", 0x10004, [this, zeal](short& color, std::string data)
        {
            if (isDamage && damageData.source && damageData.source->PetOwnerSpawnId && damageData.source->PetOwnerSpawnId == Zeal::EqGame::get_self()->SpawnId)
            {
                color = CHANNEL_MYPETDMG;
                return true;
            }
            if (isDamage && damageData.target && damageData.target->PetOwnerSpawnId && damageData.target->PetOwnerSpawnId == Zeal::EqGame::get_self()->SpawnId)
            {
                color = CHANNEL_MYPETDMG;
                return true;
            }
            return false;
        }));
    Extended_ChannelMaps.push_back(CustomFilter("Other Pet Say", 0x10005, [this, zeal](short& color, std::string data) { return color == CHANNEL_OTHERPETSAY; }));
    Extended_ChannelMaps.push_back(CustomFilter("Other Pet Damage", 0x10006, [this, zeal](short& color, std::string data)
        {
            if (isDamage && damageData.source && damageData.source->PetOwnerSpawnId && damageData.source->PetOwnerSpawnId != Zeal::EqGame::get_self()->SpawnId)
            {
                color = CHANNEL_OTHERPETDMG;
                return true;
            }
            if (isDamage && damageData.target && damageData.target->PetOwnerSpawnId && damageData.target->PetOwnerSpawnId != Zeal::EqGame::get_self()->SpawnId)
            {
                color = CHANNEL_OTHERPETDMG;
                return true;
            }
            return false;
        }));
    Extended_ChannelMaps.push_back(CustomFilter("/who", 0x10007, [this, zeal](short& color, std::string data) { return color == USERCOLOR_WHO; }));
    Extended_ChannelMaps.push_back(CustomFilter("My Melee Special", 0x10008, [this, zeal](short& color, std::string data) { return color == CHANNEL_MYMELEESPECIAL; }));
    Extended_ChannelMaps.push_back(CustomFilter("Other Melee Special", 0x10009, [this, zeal](short& color, std::string data) { return color == CHANNEL_OTHERMELEESPECIAL; }));

    //Callbacks
    zeal->callbacks->AddOutputText([this](Zeal::EqUI::ChatWnd*& wnd, std::string msg, short& channel) { this->AddOutputText(wnd, msg, channel); });
    zeal->callbacks->AddGeneric([this]() { callback_clean_ui(); }, callback_type::CleanUI);
    
    //ChatManager
    zeal->hooks->Add("CChatManager", 0x4100e2, CChatManager, hook_type_detour);
    zeal->hooks->Add("Deactivate", 0x410871, Deactivate, hook_type_detour);
    zeal->hooks->Add("AddMenu", 0x4120DD, AddMenu, hook_type_replace_call);
    zeal->hooks->Add("GetChannelMap", 0x41161D, GetChannelMap, hook_type_detour);
    zeal->hooks->Add("SetChannelMap", 0x4113F1, SetChannelMap, hook_type_detour);
    zeal->hooks->Add("ClearChannelMap", 0x41140C, ClearChannelMap, hook_type_detour);
    zeal->hooks->Add("ClearChannelMaps", 0x411638, ClearChannelMaps, hook_type_detour);
    zeal->hooks->Add("UpdateContextMenus", 0x412f9b, UpdateContextMenus, hook_type_detour);
    zeal->hooks->Add("HandleMyHitsMode", 0x500f86, HandleMyHitsMode, hook_type_detour);
    zeal->hooks->Add("HandleOtherHitsOtherMode", 0x501168, HandleOtherHitsOtherMode, hook_type_detour);

    //Individiual Modifications
    zeal->hooks->Add("PrintSplit", 0x54755b, PrintSplit, hook_type_replace_call); //fix up money split
    zeal->hooks->Add("PrintAutoSplit", 0x4FB477, PrintAutoSplit, hook_type_replace_call); //fix up money split
    zeal->hooks->Add("serverGetString", 0x4EE6C9, serverGetString, hook_type_replace_call);
    zeal->hooks->Add("serverPrintChat", 0x4ee727, serverPrintChat, hook_type_replace_call);
    zeal->hooks->Add("whoGlobalPrintChat1", 0x4e4d6f, whoGlobalPrintChat_wrapped, hook_type_replace_call);
    zeal->hooks->Add("whoGlobalPrintChat2", 0x4e4d7e, whoGlobalPrintChat_wrapped, hook_type_replace_call);
    zeal->hooks->Add("whoGlobalPrintChat3", 0x4e523a, whoGlobalPrintChat_full, hook_type_replace_call);
    zeal->hooks->Add("whoGlobalPrintChat4", 0x4e53b1, whoGlobalPrintChat_wrapped, hook_type_replace_call);
    
    //ChatWindow::WndNotification Conditional Patch
    mem::write<byte[2]>(0x414117, { 0x8d, 0x05 });  //lea eax
    mem::write<int>(0x414119, (int)FilterConditional);
    mem::write<byte[2]>(0x41411D, { 0xFF, 0xE0 });  //jmp eax
    mem::write<byte[4]>(0x41411F, { 0x90, 0x90, 0x90, 0x90 });
}

chatfilter::~chatfilter()
{
}
