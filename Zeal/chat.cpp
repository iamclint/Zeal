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
    ui_options* options = ZealService::get_instance()->ui->options.get();
    chat* c = ZealService::get_instance()->chat_hook.get();

    switch (index)
    {
        case 4:
        case 0x10:
            if (c->UseBlueCon.get())
            {
                return options->GetColor(14);
            }
            break;
        case CHANNEL_MYPETDMG:
            return options->GetColor(19);
        case CHANNEL_OTHERPETDMG:
            return options->GetColor(20);
        case CHANNEL_MYPETSAY:
            return options->GetColor(21);
        case CHANNEL_OTHERPETSAY:
            return options->GetColor(22);
    }
    return ZealService::get_instance()->hooks->hook_map["GetRGBAFromIndex"]->original(GetRGBAFromIndex)(t, u, index);
}


void __fastcall PrintChat(int t, int unused, const char* data, short color_index, bool add_log)
{
    if (!data || strlen(data) == 0)
        return;
    chat* c = ZealService::get_instance()->chat_hook.get();
    ZealService::get_instance()->pipe->chat_msg(data, color_index);

    std::string data_str = data; //keeping this here for future use to strip out the unique identifiers _208 in names in the commented regex below.
    //if (data_str.length())
    //{
    //    data_str.erase(std::remove(data_str.begin(), data_str.end(), '#'), data_str.end());
    //    std::regex pattern("\\b(?=\\w*)([a-zA-Z_]\\w+)(\\d{3})\\b");
    //    data_str = std::regex_replace(data_str, pattern, "$1");
    //    std::replace(data_str.begin(), data_str.end(), '_', ' ');
    //}

    if (c->TimeStampsStyle.get() && strlen(data) > 0) //remove phantom prints (the game also checks this, no idea why they are sending blank data in here sometimes
    {
        std::string timestamp_buffer = generateTimestampedString(data_str, c->TimeStampsStyle.get() == 1);
        timestamp_buffer.resize(2048);
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, timestamp_buffer.data(), color_index, false);
        if (add_log)
        {
            reinterpret_cast<bool(__thiscall*)(int t, const char* data, int)>(0x538110)(t, data, 0); // do the percent convert for logging
            reinterpret_cast<void(__cdecl*)(const char* data)>(0x5240dc)(data); //add to log
        }
    }
    else
    {
        data_str.resize(2048);
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, data_str.c_str(), color_index, false);
        if (add_log)
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
        if (ZealService::get_instance()->chat_hook->UseUniqueNames.get())
            return data;
        else
            return ZealService::get_instance()->hooks->hook_map["StripName"]->original(StripName)(t, unused, data);
    }
    return data;
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

static std::string get_tab_completion_target(const std::string& text, const char* prefix) {
    // Only process if the correct prefix and some characters after the prefix's trailing space.
    std::string prefix_with_space = std::string(prefix) + " ";
    int length = prefix_with_space.length();
    if (text.starts_with(prefix_with_space) && text.length() > length)
    {
        // Valid target if either no second space or the space is the final character.
        auto second_space = text.find(' ', length);
        bool no_space = (second_space == std::string::npos);
        if (no_space || (second_space == text.length() - 1))
        {
            auto count = no_space ? std::string::npos : (second_space - length);
            return text.substr(length, count);  // Return with stripped trailing space.
        }
    }
    return std::string();
}

// Search the client's tell list, which is a most recent first sorted list of other players that
// have sent tells to the client.
static std::vector<std::string> get_tell_list_matches(const std::string& start_of_name) {
    std::vector<std::string> result;
    const int tell_list_size = 31;  // Stores most recent 31 tell names.
    const char (*tell_list)[64] = reinterpret_cast<const char (*)[64]>(0x007CE45C);
    for (int i = 0; i < tell_list_size; ++i)
    {
        if (tell_list[i][0] == 0)  // Rest of list is empty.
            break;
        if (start_of_name.empty() ||
            _strnicmp(tell_list[i], start_of_name.c_str(), start_of_name.length()) == 0)
            result.push_back(tell_list[i]);
    }
    return result;
}

// Search the client's raid list for matches.
static std::vector<std::string> get_raid_list_matches(const std::string& start_of_name) {
    std::vector<std::string> result;
    const Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
    if (!raid_info->is_in_raid())
        return result;

    const int kRaidMaxMembers = Zeal::EqStructures::RaidInfo::kRaidMaxMembers;
    for (int i = 0; i < kRaidMaxMembers; ++i) {
        const auto& member = raid_info->MemberList[i];
        if (_strnicmp(member.Name, start_of_name.c_str(), start_of_name.length()) == 0)
            result.push_back(member.Name);
    }
    return result;
}

// Assembles a list of all tell list, raid, and local zone name matches
static std::vector<std::string> get_all_matches(const std::string& start_of_name)
{
    auto matches = get_tell_list_matches(start_of_name);
    auto raid_matches = get_raid_list_matches(start_of_name);
    for (const auto& name : raid_matches)
        if (std::find(matches.begin(), matches.end(), name) == matches.end())
            matches.push_back(name);

    auto entity_manager = ZealService::get_instance()->entity_manager.get();  // Short-term ptr.
    if (entity_manager)
    {
        auto ent_matches = entity_manager->GetPlayerPartialMatches(start_of_name);
        for (const auto& name : ent_matches)
            if (std::find(matches.begin(), matches.end(), name) == matches.end())
                matches.push_back(name);
    }
    if (Zeal::EqGame::get_self())
        std::erase(matches, std::string(Zeal::EqGame::get_self()->Name));
    return matches;
}

// Returns true if prefix, prefix + " ", or if prefix + " <name> ".
static bool is_autocycle_tell(const std::string& text, const char* prefix) {
    if (!text.starts_with(prefix))
        return false;
    int length = strlen(prefix);
    if (text.length() == length)
        return true;
    if (text[length] != ' ')
        return false;
    if (text.length() == (length + 1))
        return true;

    auto second_space = text.find(' ', length + 1);
    return (second_space == text.length() - 1);
}

// Safely convert a wide Unicode string to an UTF8 string.
static std::string wchar_to_utf8(const wchar_t* input)
{
    std::wstring wstr = input;
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], size_needed, NULL, NULL);
    return result;
}

// Enhance the /tell tab completion to include the raid and zone player list and also enable
// it for /t and /consent.
static bool check_for_tab_completion(Zeal::EqUI::EditWnd* active_edit, UINT32 key, int modifier, char keydown)
{
    static std::vector<std::string> matches;  // Cache state for cycling through the list.

    bool not_a_tab_down = (key != 0xf || !keydown || (modifier != 0 && modifier != 1));
    if (not_a_tab_down || !active_edit->InputText.Data || active_edit->item_link_count
         || active_edit->InputText.Data->Length < 2)
    {
        // Clear the cache if keydown is not a shift key (DIK_LSHIFT or DIK_RSHIFT).
        if (keydown && (key != 0x2a && key != 0x36))
            matches.clear();  // Flush the cached list (if any) for any other keydown press.
        return false;
    }

    // First check if the start of the text is one that supports tab completion.
    // We cycle through this even if we already have matches just to re-use code.
    std::vector<const char*> prefix_list = { "/tell", "/t", "/consent" };
    const char* input_text = active_edit->InputText.Data->Text;
    std::string text = (active_edit->InputText.Data->Encoding == 0) ?
        std::string(input_text) :
        wchar_to_utf8(reinterpret_cast<const wchar_t*>(input_text));
    for (const char* prefix : prefix_list)
    {
        if (!text.starts_with(prefix))
            continue;

        bool cycle_list = true;
        // Replicate client /tell tab behavior first when not obviously trying to auto-complete.
        if (matches.size() < 2 && is_autocycle_tell(text, prefix)) {
            matches = get_tell_list_matches("");
            cycle_list = !matches.empty() && (text == std::string(prefix) + " " + matches.front() + " ");
        }
        else if (matches.empty()) {
            auto target = get_tab_completion_target(text, prefix);
            if (target.empty())
                continue;

            cycle_list = false;
            matches = get_all_matches(target);
            if (matches.size() > 1)
            {
                Zeal::EqGame::print_chat("Possible matches:");
                for (const auto& match : matches)
                    Zeal::EqGame::print_chat("  %s", match.c_str());
            }
        }
        if (cycle_list && matches.size() > 1) {
            if (modifier == 0)  // Shift not down, cycle forwards
            {
                std::string old_front = matches.front();  // Rotate list forward.
                for (int i = 1; i < matches.size(); ++i)
                    matches[i - 1] = matches[i];
                matches.back() = old_front;
            }
            else
            {
                std::string old_back = matches.back();  // Rotate list backwards.
                for (int i = matches.size() - 1; i > 0; --i)
                    matches[i] = matches[i - 1];
                matches.front() = old_back;
            }
        }
        if (matches.empty())
            Zeal::EqGame::print_chat("No matches");
        else  // Update the text with the first match in the list.
        {
            std::string updated_text = std::string(prefix) + " " + matches.front() + " ";
            active_edit->InputText.FreeRep();
            active_edit->InputText = Zeal::EqUI::CXSTR(updated_text);
            active_edit->Caret_End = active_edit->GetInputLength();
            active_edit->Caret_Start = active_edit->Caret_End;
        }
        return true;  // Break and ignore further processing of the tab key.
    }
    matches.clear();
    return false;
}

int __fastcall EditWndHandleKey(Zeal::EqUI::EditWnd* active_edit, int u, UINT32 key, int modifier, char keydown)
{
    if (!ZealService::get_instance()->chat_hook->UseZealInput.get())
        return ZealService::get_instance()->hooks->hook_map["EditWndHandleKey"]->original(EditWndHandleKey)(active_edit, u, key, modifier, keydown);
   // Zeal::EqGame::print_chat("EditWnd: 0x%x key: %x modifier: %i state: %i", active_edit, key, modifier, keydown);
    if (check_for_tab_completion(active_edit, key, modifier, keydown)) {
        return 0;
    }
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
                case 0x2F: //v (paste)//025595 Doljonijiarnimorinar
                {
                    std::string temp_text = StripSpecialCharacters(ReadFromClipboard());
                    int links_in_paste = std::count(temp_text.begin(), temp_text.end(), '');
                    if (links_in_paste > 0)
                        links_in_paste /= 2;

                    if (active_edit->item_link_count + links_in_paste < 10)
                    {
                        active_edit->InputText.Assure(temp_text.length() + active_edit->InputText.Data->Length + 1, 0);
                        active_edit->ReplaceSelection(temp_text.c_str(), false);
                    }
                    else
                    {
                        Zeal::EqGame::print_chat_wnd(Zeal::EqGame::Windows->ChatManager->GetActiveChatWindow(), 0, "<c \"#FF0000\">Too many item links to paste</c>");
                    }
                    return 0;
                }
                case 0x1E: //a (select all)
                {
                    active_edit->Caret_Start = 0;
                    active_edit->Caret_End = active_edit->GetInputLength();
                    return 0;
                }
                case 0x2D: //x (cut)
                case 0x2E: //c (copy)
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
                    if (key == 0x2D)
                        active_edit->ReplaceSelection("", false);
                    return 0;
                }
            }
        }
    }
    return ZealService::get_instance()->hooks->hook_map["EditWndHandleKey"]->original(EditWndHandleKey)(active_edit, u, key, modifier, keydown);
}

void __fastcall DoPercentConvert(int* t, int u, char* data, int u2)
{
    if (!data || !ZealService::get_instance()) 
        return;  // Early exit if data is null or ZealService instance is invalid

    std::string str_data = data;

    // Check if there's a '%' character; if not, exit early
    if (str_data.find('%') == std::string::npos) {
        return;
    }

    if (Zeal::EqGame::is_in_game()) {
        try {
            auto chat_hook = ZealService::get_instance()->chat_hook;
            if (chat_hook) {
                chat_hook->DoPercentReplacements(str_data);
                size_t new_len = str_data.length();
                std::memcpy(data, str_data.c_str(), new_len);
                data[new_len] = '\0';  // Ensure null-termination
            }
        }
        catch (const std::exception& e) {
            Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Error in DoPercentConvert: %s",e.what());
            return;
        }
    }

    // Call original function using the stored hook
    if (auto hook = ZealService::get_instance()->hooks->hook_map["DoPercentConvert"]) {
        hook->original(DoPercentConvert)(t, u, data, u2);
    }
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
                TimeStampsStyle.set(2);
            }
            else
            {
                TimeStampsStyle.set(TimeStampsStyle.get() > 0 ? 0 : 1);
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/zealinput", { "/zinput" }, "Toggles zeal input which gives you a more modern input feel.",
        [this](std::vector<std::string>& args) {
            UseZealInput.toggle();
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/classicclasses", { "/cc" }, "Toggles classic classes only (no longer showing vicar for a 50+ cleric for example).",
        [this](std::vector<std::string>& args) {
            UseClassicClassNames.toggle();
            if (UseClassicClassNames.get())
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
            UseBlueCon.toggle();
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

    // Disable the cycle reply forwards and backwards if ZealInput enabled
    zeal->binds_hook->replace_cmd(0x3c, [this](int state) { return ZealService::get_instance()->chat_hook->UseZealInput.get(); });
    zeal->binds_hook->replace_cmd(0x3d, [this](int state) { return ZealService::get_instance()->chat_hook->UseZealInput.get(); });

}
void chat::set_classes()
{
    if (UseClassicClassNames.get())
    {
        mem::write<byte>(0x4bc090, 66); //this just changes the if statement to check if the player is > 65 rather than > 50
    }
    else
    {
        mem::write<byte>(0x4bc090, 51);
    }
}

chat::~chat()
{
}

