#include "chat.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
#include <algorithm>
#include <regex>

std::string ReadFromClipboard() {
    std::string text;
    if (OpenClipboard(nullptr)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData != nullptr) {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText != nullptr) {
                text = pszText;
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }
    return text;
}
void SetClipboardText(const std::string& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, text.length() + 1);
        if (hClipboardData != nullptr) {
            char* buffer = static_cast<char*>(GlobalLock(hClipboardData));
            if (buffer != nullptr) {
                memcpy(buffer, text.c_str(), text.length());
                buffer[text.length()] = '\0'; // Null-terminate the string
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_TEXT, hClipboardData);
            }
            else {
                std::cerr << "Failed to lock clipboard memory." << std::endl;
            }
        }
        else {
            std::cerr << "Failed to allocate memory for clipboard data." << std::endl;
        }
        CloseClipboard();
    }
    else {
        std::cerr << "Failed to open clipboard." << std::endl;
    }
}


std::string StripSpecialCharacters(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return c == '\n' || c == '\r';
        }), result.end());
    return result;
}

std::string generateTimestampedString(const std::string& message, bool longform = true) {
    time_t rawtime;
    struct tm timeinfo;
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    std::ostringstream oss;
    if (longform)
    {
        oss << "[" << std::setw(2) << std::setfill('0') << ((timeinfo.tm_hour % 12 == 0) ? 12 : timeinfo.tm_hour % 12) << ":"
            << std::setw(2) << std::setfill('0') << timeinfo.tm_min << ":"
            << std::setw(2) << std::setfill('0') << timeinfo.tm_sec << " "
            << ((timeinfo.tm_hour >= 12) ? "PM" : "AM") << "] "
            << message;
    }
    else
    {
        oss << "[" << std::setw(2) << std::setfill('0') << timeinfo.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << timeinfo.tm_min << "] "
            << message;
    }
    return oss.str();
}
// Function to replace underscores with spaces in a word
std::string replaceUnderscores(const std::smatch& match) {
    std::string word = match[1].str(); // Get the word part
    std::replace(word.begin(), word.end(), '_', ' '); // Replace underscores with spaces
    return word + match[2].str(); // Append the three digits
}

UINT32  __fastcall GetRGBAFromIndex(int t, int u, USHORT index)
{
    chat* c = ZealService::get_instance()->chat_hook.get();
    if ((index == 4 || index == 0x10) && c->UseBlueCon)
        index = 325;
    return ZealService::get_instance()->hooks->hook_map["GetRGBAFromIndex"]->original(GetRGBAFromIndex)(t, u, index);
}


void __fastcall PrintChat(int t, int unused, const char* data, short color_index, bool u)
{
    if (!data || strlen(data) == 0)
        return;
    chat* c = ZealService::get_instance()->chat_hook.get();
    ZealService::get_instance()->pipe->chat_msg(data, color_index);



    std::string data_str = data;
    //if (data_str.length())
    //{
    //    data_str.erase(std::remove(data_str.begin(), data_str.end(), '#'), data_str.end());
    //    std::regex pattern("\\b(?=\\w*)([a-zA-Z_]\\w+)(\\d{3})\\b");
    //    data_str = std::regex_replace(data_str, pattern, "$1");
    //    std::replace(data_str.begin(), data_str.end(), '_', ' ');
    //}
    if (c->TimeStampsStyle && strlen(data) > 0) //remove phantom prints (the game also checks this, no idea why they are sending blank data in here sometimes
    {
        mem::write<byte>(0x5380C9, 0xEB); // don't log information so we can manipulate data before between chat and logs
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, generateTimestampedString(data_str, c->TimeStampsStyle==1).c_str(), color_index, false);
        mem::write<byte>(0x5380C9, 0x75); //reset the logging
        if (u)
        {
            reinterpret_cast<bool(__thiscall*)(int t, const char* data, int)>(0x538110)(t, data, 0); // do the percent convert for logging
            reinterpret_cast<void(__cdecl*)(const char* data)>(0x5240dc)(data); //add to log
        }
    }
    else
    {
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, data_str.c_str(), color_index, false);
        if (u)
        {
            reinterpret_cast<bool(__thiscall*)(int t, const char* data, int)>(0x538110)(t, data, 0); // do the percent convert for logging
            reinterpret_cast<void(__cdecl*)(const char* data)>(0x5240dc)(data); //add to log
        }
    }


}

char* __fastcall StripName(int t, int unused, char* data)
{
    if (ZealService::get_instance()->hooks && ZealService::get_instance()->hooks->hook_map.count("StripName"))
    {
        if (ZealService::get_instance()->chat_hook->UseUniqueNames)
            return data;
        else
            return ZealService::get_instance()->hooks->hook_map["StripName"]->original(StripName)(t, unused, data);
    }
    return data;
}

void chat::LoadSettings(IO_ini* ini)
{
    if (!ini->exists("Zeal", "ChatTimestamps"))
        ini->setValue<int>("Zeal", "ChatTimestamps", 0);
    if (!ini->exists("Zeal", "Bluecon"))
        ini->setValue<bool>("Zeal", "Bluecon", false);
    if (!ini->exists("Zeal", "ZealInput"))
        ini->setValue<bool>("Zeal", "ZealInput", false);
    if (!ini->exists("Zeal", "UniqueNames"))
        ini->setValue<bool>("Zeal", "UniqueNames", false);
    if (!ini->exists("Zeal", "ClassicClasses"))
        ini->setValue<bool>("Zeal", "ClassicClasses", false);
    UseClassicClassNames = ini->getValue<bool>("Zeal", "ClassicClasses");
    set_classes(UseClassicClassNames);
    UseUniqueNames = ini->getValue<bool>("Zeal", "UniqueNames");
    UseBlueCon = ini->getValue<bool>("Zeal", "Bluecon");
    TimeStampsStyle = ini->getValue<int>("Zeal", "ChatTimestamps");
    UseZealInput = ini->getValue<bool>("Zeal", "ZealInput");
}

enum class caret_dir : int
{
    none,
    left,
    right
};

enum class eq_modifier_keys : int
{
    none = 0,
    shift = 1,
    ctrl = 2,
    alt = 4
};

void move_caret(Zeal::EqUI::EditWnd* active_edit, caret_dir dir) {
    static caret_dir highlight_direction = dir;
    int move_index = 0;
    DWORD* caret = &active_edit->Caret_End;

    if (Zeal::EqGame::KeyMods->Shift && active_edit->Caret_End == active_edit->Caret_Start)
        highlight_direction = dir;
    else if (!Zeal::EqGame::KeyMods->Shift)
        highlight_direction = caret_dir::none;

    if (dir == caret_dir::left)
    {
        caret = &active_edit->Caret_Start;
        if (highlight_direction == caret_dir::right && Zeal::EqGame::KeyMods->Shift)
            caret = &active_edit->Caret_End;

        move_index = *caret - 1;
        if (move_index < 0)
            move_index = 0;
    }
    else
    {
        caret = &active_edit->Caret_End;
        if (highlight_direction == caret_dir::left && Zeal::EqGame::KeyMods->Shift)
            caret = &active_edit->Caret_Start;
        move_index = *caret + 1;
        if (move_index > active_edit->GetInputLength())
            move_index = active_edit->GetInputLength();
    }
       

    for (DWORD i = 0; i < active_edit->item_link_count; i++) {
        if ((dir == caret_dir::left && active_edit->LinkEndIndex[i]-1 == move_index) ||
            (dir == caret_dir::right && active_edit->LinkStartIndex[i]+1 == move_index)) {
            move_index = (dir == caret_dir::left) ? active_edit->LinkStartIndex[i] : active_edit->LinkEndIndex[i];
            break;
        }
    }


    if (dir == caret_dir::left) {
        *caret = move_index;
        if (!Zeal::EqGame::KeyMods->Shift)
            active_edit->Caret_End = move_index;
    } else {
        *caret = move_index;
        if (!Zeal::EqGame::KeyMods->Shift)
            active_edit->Caret_Start = move_index;
    }
}

int __fastcall EditWndHandleKey(Zeal::EqUI::EditWnd* active_edit, int u, UINT32 key, int modifier, char keydown)
{
    if (!ZealService::get_instance()->chat_hook->UseZealInput)
        return ZealService::get_instance()->hooks->hook_map["EditWndHandleKey"]->original(EditWndHandleKey)(active_edit, u, key, modifier, keydown);
   // Zeal::EqGame::print_chat("EditWnd: 0x%x key: %x modifier: %i state: %i", active_edit, key, modifier, keydown);
    if (ZealService::get_instance()->tells->HandleKeyPress(key, keydown, modifier))
        return 0;
    //you can use a bitwise & operator on the modifier with eq_modifier_keys to check key states
    if (keydown)
    {

        if (!active_edit->InputText.Data)
            active_edit->InputText.Assure(32, 0);

        static caret_dir last_highlight_dir = caret_dir::none;

        switch (key)
        {
        case 0x1: //escape
        {
            Zeal::EqGame::get_wnd_manager()->Focused = active_edit->ParentWnd;
            return 0;
        }
        case 0xC7: //home
        {
            active_edit->Caret_Start = 0;
            if (!Zeal::EqGame::KeyMods->Shift)
                active_edit->Caret_End = 0;
            last_highlight_dir = caret_dir::left;
            return 0;
        }
        case 0xCF: //end
        {
            active_edit->Caret_End = active_edit->GetInputLength();
            if (!Zeal::EqGame::KeyMods->Shift)
                active_edit->Caret_Start = active_edit->Caret_End;
            last_highlight_dir = caret_dir::right;
            return 0;
        }
        case 0xCB: //left arrow
            move_caret(active_edit, caret_dir::left);
            return 0;
        case 0xCD: //right arrow
            move_caret(active_edit, caret_dir::right);
            return 0;
        }

        if (!Zeal::EqGame::KeyMods->Shift)
            last_highlight_dir = caret_dir::none;

        if (Zeal::EqGame::KeyMods->Ctrl)
        {
            switch (key)
            {
                case 0xCB: //left arrow
                {
                    move_caret(active_edit, caret_dir::left);
                    return 0;
                }
                case 0xCD: //right arrow
                {
                    move_caret(active_edit, caret_dir::right);
                    return 0;
                }
                case 0x2F: //v
                {
                    std::string temp_text = StripSpecialCharacters(ReadFromClipboard());
                    active_edit->InputText.Assure(temp_text.length()+active_edit->InputText.Data->Length, 0);

                    active_edit->ReplaceSelection(temp_text.c_str(), false);
                    return 0;
                }
                case 0x1E: //a
                {
                    active_edit->Caret_Start = 0;
                    active_edit->Caret_End = active_edit->GetInputLength();
                    return 0;
                }
                case 0x2E: //c
                {
                    if (active_edit->Caret_End - active_edit->Caret_Start > 0)
                    {
                        int len_highlighted = active_edit->Caret_End - active_edit->Caret_Start;
                        int highlighted_link_count = 0;
                        int links_prior = 0;
                        for (DWORD i = 0; i < active_edit->item_link_count; i++)
                        {
                            if (active_edit->LinkStartIndex[i] < active_edit->Caret_Start)
                                links_prior++;
                            if (active_edit->LinkEndIndex[i] <= active_edit->Caret_End && active_edit->LinkStartIndex[i] >= active_edit->Caret_Start)
                                highlighted_link_count++;
                        }

                        int new_caret_start = active_edit->Caret_Start + (links_prior * 9);
                        int new_caret_end = active_edit->Caret_End + (links_prior * 9);
                        std::string highlighted_text(active_edit->InputText.Data->Text + new_caret_start, active_edit->InputText.Data->Text + new_caret_end + (highlighted_link_count * 9));
                        SetClipboardText(highlighted_text);
                    }
                    return 0;
                }
            }
        }
    }

    if (key == 0x1C && !keydown) //enter
    {
        ZealService::get_instance()->tells->ResetChatToAlwaysWindow();
        active_edit->InputText.Assure(32, 0);
    }
    return ZealService::get_instance()->hooks->hook_map["EditWndHandleKey"]->original(EditWndHandleKey)(active_edit, u, key, modifier, keydown);
}

void __fastcall DoPercentConvert(int* t, int u, char* data, int u2)
{
    std::string str_data = data;
    if (str_data.find('%', 0) == std::string::npos) //if there isn't any % just early out
        return;
    if (Zeal::EqGame::is_in_game())
    {
        ZealService::get_instance()->chat_hook->DoPercentReplacements(str_data);
        size_t new_len = str_data.length();
        strncpy_s(data, new_len+1, str_data.c_str(), new_len);
        data[new_len] = '\0';
    }
    ZealService::get_instance()->hooks->hook_map["DoPercentConvert"]->original(DoPercentConvert)(t, u, data, u2);
}
void chat::DoPercentReplacements(std::string& str_data)
{
    for (auto& fn : percent_replacements)
        fn(str_data);
}

void chat::InitPercentReplacements()
{
    percent_replacements.push_back([](std::string& str_data) { 
        std::string mana; 
        ZealService::get_instance()->labels_hook->GetLabel(20, mana); 
        Zeal::String::replace(str_data, "%mana", mana + "%"); 
        Zeal::String::replace(str_data, "%n", mana + "%"); 
     });
    percent_replacements.push_back([](std::string& str_data) { 
        std::string hp; 
        ZealService::get_instance()->labels_hook->GetLabel(19, hp);  
        Zeal::String::replace(str_data, "%hp", hp + "%"); 
        Zeal::String::replace(str_data, "%h", hp + "%"); 
    });
    percent_replacements.push_back([](std::string& str_data) { 
        std::stringstream ss; 
        ss << std::fixed << std::setprecision(2) << std::ceil(Zeal::EqGame::get_self()->Position.x * 100) / 100 << ", " << std::ceil(Zeal::EqGame::get_self()->Position.y * 100) / 100 << ", " << std::ceil(Zeal::EqGame::get_self()->Position.z * 100) / 100; 
        Zeal::String::replace(str_data, "%loc", ss.str());
    });
    percent_replacements.push_back([](std::string& str_data) {
        std::string target_hp;
        ZealService::get_instance()->labels_hook->GetLabel(29, target_hp);
        Zeal::String::replace(str_data, "%targethp", target_hp + "%");
        Zeal::String::replace(str_data, "%th", target_hp + "%");
     });
}


void chat::set_input_color(Zeal::EqUI::ARGBCOLOR col)
{
   /* Zeal::EqUI::CXWndManager* mgr = Zeal::EqGame::get_wnd_manager();
    if (!mgr)
        return;
    Zeal::EqUI::EQWND* active_edit = mgr->ActiveEdit;
    if (active_edit)
        active_edit->TextColor = col; */
}


chat::chat(ZealService* zeal, IO_ini* ini)
{
    //zeal->callbacks->add_packet([this](UINT opcode, char* buffer, UINT size) {

    //
    //    if (opcode == 0x4058 || opcode == 0x404A)
    //    {
    //        Zeal::EqGame::print_chat("Opcode: 0x%x Size: %i Buffer: %s", opcode, size, Zeal::String::bytes_to_hex(buffer, size).c_str());
    //    }
    //if (opcode == 0x4236)
    //{
    //    Zeal::EqGame::print_chat("Opcode: 0x%x Size: %i Buffer: %s", opcode, size, Zeal::String::bytes_to_hex(buffer, size).c_str());
    //}
    //
    //return false;
    //}, callback_type::WorldMessage);
    zeal->commands_hook->Add("/timestamp", { "/tms" }, "Toggles timestamps on chat windows.",
        [this](std::vector<std::string>& args) {
            if (args.size() > 1 && args[1] == "2")
            {
                set_timestamp(2);
            }
            else
            {
                set_timestamp(TimeStampsStyle > 0 ? 0 : 1);
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/zealinput", { "/zinput" }, "Toggles zeal input which gives you a more modern input feel.",
        [this](std::vector<std::string>& args) {
            set_input(!UseZealInput);
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/classicclasses", { "/cc" }, "Toggles classic classes only (no longer showing vicar for a 50+ cleric for example).",
        [this](std::vector<std::string>& args) {
            set_classes(!UseClassicClassNames);
            if (UseClassicClassNames)
            {
                Zeal::EqGame::print_chat("Classic Class Names enabled");
            }
            else
            {
                Zeal::EqGame::print_chat("Classic Class Names disabled");
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/bluecon", { }, "Toggles the custom color for blue con that you can adjust in options.",
        [this](std::vector<std::string>& args) {
            set_bluecon(!UseBlueCon);
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/loc", { }, "Adds noprint arguments to /loc to not log the location to your chat.",
        [this](std::vector<std::string>& args) {
            if (args.size() > 1 && args[1]=="noprint")
            {
                std::stringstream ss;
                ss << "Your Location is " << std::fixed << std::setprecision(2) << std::ceil(Zeal::EqGame::get_self()->Position.x * 100) / 100 << ", " << std::ceil(Zeal::EqGame::get_self()->Position.y * 100) / 100 << ", " << std::ceil(Zeal::EqGame::get_self()->Position.z * 100) / 100;
                std::string result = ss.str();
                Zeal::EqGame::log(result);
                return true;
            }
            return false; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/log", {}, "Toggles log on/off or adds something directly to your log",
        [this, zeal](std::vector<std::string>& args) {
            std::string full_str;
            if (args.size() == 2 && (Zeal::String::compare_insensitive(args[1], "on") || Zeal::String::compare_insensitive(args[1], "off")))
            {
                return false;
            }
            if (args.size() > 1)
            {
                for (size_t i = 1; i < args.size(); ++i)
                {
                    if (i < args.size() - 1)
                        full_str += args[i] + " ";
                    else
                        full_str += args[i];
                }
                Zeal::EqGame::DoPercentConvert(full_str);
                Zeal::EqGame::log(full_str);
                return true;
            }
            return false;
        });
    LoadSettings(ini);

  /*  zeal->commands_hook->Add("/uniquenaming", {}, "Toggles off the stripping of mob id and other identifiers from name of npc's (log only)",
        [this, ini](std::vector<std::string>& args) {
            uniquenames = !uniquenames;
            Zeal::EqGame::print_chat("Unique naming is %s", uniquenames ? "Enabled" : "Disabled");
            return true;
        });*/

    //zeal->hooks->Add("StripName", 0x529A8b, StripName, hook_type_replace_call); 
    //zeal->hooks->Add("StripName1", 0x529B1D, StripName, hook_type_replace_call); 
    //zeal->hooks->Add("StripName2", 0x529b6d, StripName, hook_type_replace_call);
    //zeal->hooks->Add("StripName3", 0x529b89, StripName, hook_type_replace_call);
    //zeal->hooks->Add("StripName4", 0x529A55, StripName, hook_type_replace_call);
    //zeal->hooks->Add("StripName5", 0x4EDBE5, StripName, hook_type_replace_call); //killed msg
    //zeal->hooks->Add("StripName6", 0x4EDBDA, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName7", 0x529371, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName8", 0x52933D, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName9", 0x5293EB, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName10", 0x529407, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName11", 0x529423, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName12", 0x5293CF, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName13", 0x5293B3, StripName, hook_type_replace_call);//killed msg
    //zeal->hooks->Add("StripName14", 0x5293A6, StripName, hook_type_replace_call);//killed msg
    zeal->hooks->Add("DoPercentConvert", 0x538110, DoPercentConvert, hook_type_detour); //add extra prints for new loot types
    zeal->hooks->Add("PrintChat", 0x537f99, PrintChat, hook_type_detour); //add extra prints for new loot types
    zeal->hooks->Add("EditWndHandleKey", 0x5A3010, EditWndHandleKey, hook_type_detour); //this makes more sense than the hook I had previously


    //My function for getting instruction length was failing on this function, couldn't be bothered to look into it too deeply atm so just replaced all the calls to it
    zeal->hooks->Add("GetRGBAFromIndex", 0x406b02, GetRGBAFromIndex, hook_type_replace_call); //this is for modifying blue con color everywhere including chat
    zeal->hooks->Add("GetRGBAFromIndex1", 0x406b12, GetRGBAFromIndex, hook_type_replace_call); 
    zeal->hooks->Add("GetRGBAFromIndex2", 0x406cdf, GetRGBAFromIndex, hook_type_replace_call); 
    zeal->hooks->Add("GetRGBAFromIndex3", 0x407d90, GetRGBAFromIndex, hook_type_replace_call); 
    zeal->hooks->Add("GetRGBAFromIndex4", 0x407da2, GetRGBAFromIndex, hook_type_replace_call); 
    zeal->hooks->Add("GetRGBAFromIndex5", 0x4139eb, GetRGBAFromIndex, hook_type_replace_call); 
    zeal->hooks->Add("GetRGBAFromIndex6", 0x438719, GetRGBAFromIndex, hook_type_replace_call);   
    InitPercentReplacements();
}
void chat::set_classes(bool val)
{
    UseClassicClassNames = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "ClassicClasses", UseClassicClassNames);
    if (UseClassicClassNames)
    {
        mem::write<byte>(0x4bc090, 66); //this just changes the if statement to check if the player is > 65 rather than > 50
    }
    else
    {
        mem::write<byte>(0x4bc090, 51);
    }
    if (ZealService::get_instance()->ui && ZealService::get_instance()->ui->options)
        ZealService::get_instance()->ui->options->UpdateOptions();
}

void chat::set_input(bool val)
{
    UseZealInput = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "ZealInput", UseZealInput);
    if (UseZealInput)
        Zeal::EqGame::print_chat("Zeal special input enabled");
    else
        Zeal::EqGame::print_chat("Zeal special input disabled");
    ZealService::get_instance()->ui->options->UpdateOptions();
}
void chat::set_timestamp(int val)
{
    TimeStampsStyle = val;
    ZealService::get_instance()->ini->setValue<int>("Zeal", "ChatTimestamps", TimeStampsStyle);
    if (TimeStampsStyle)
        Zeal::EqGame::print_chat("Timestamps enabled");
    else
        Zeal::EqGame::print_chat("Timestamps disabled");
    ZealService::get_instance()->ui->options->UpdateOptions();
}
void chat::set_bluecon(bool val)
{
    UseBlueCon = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "Bluecon", UseBlueCon);
    if (UseBlueCon)
        Zeal::EqGame::print_chat("Blue con color is now set to usercolor 70");
    else
        Zeal::EqGame::print_chat("Default blue con color.");
    ZealService::get_instance()->ui->options->UpdateOptions();
}
chat::~chat()
{
}

