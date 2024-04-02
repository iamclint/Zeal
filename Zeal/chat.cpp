#include "chat.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

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

std::string generateTimestampedString(const char* message) {
    time_t rawtime;
    struct tm timeinfo;
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    std::ostringstream oss;
    oss << "[" << std::setw(2) << std::setfill('0') << ((timeinfo.tm_hour % 12 == 0) ? 12 : timeinfo.tm_hour % 12) << ":"
        << std::setw(2) << std::setfill('0') << timeinfo.tm_min << ":"
        << std::setw(2) << std::setfill('0') << timeinfo.tm_sec << " "
        << ((timeinfo.tm_hour >= 12) ? "PM" : "AM") << "] "
        << message;
    return oss.str();
}

void __fastcall PrintChat(int t, int unused, const char* data, short color_index, bool u)
{
    chat* c = ZealService::get_instance()->chat_hook.get();
    if (color_index == 4 && c->bluecon)
        color_index = 325;

    if (c->timestamps && strlen(data) > 0) //remove phantom prints (the game also checks this, no idea why they are sending blank data in here sometimes
    {
        mem::write<byte>(0x5380C9, 0xEB); // don't log information so we can manipulate data before between chat and logs
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, generateTimestampedString(data).c_str(), color_index, u);
        mem::write<byte>(0x5380C9, 0x75); //reset the logging

        reinterpret_cast<void(__cdecl*)( const char* data)>(0x5240dc)(data); //add to log
    }
    else
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, data, color_index, u);

}

void chat::LoadSettings(IO_ini* ini)
{
    if (!ini->exists("Zeal", "ChatTimestamps"))
        ini->setValue<bool>("Zeal", "ChatTimestamps", false);
    if (!ini->exists("Zeal", "Bluecon"))
        ini->setValue<bool>("Zeal", "Bluecon", false);
    if (!ini->exists("Zeal", "ZealInput"))
        ini->setValue<bool>("Zeal", "ZealInput", false);
    bluecon = ini->getValue<bool>("Zeal", "Bluecon");
    timestamps = ini->getValue<bool>("Zeal", "ChatTimestamps");
    zealinput = ini->getValue<bool>("Zeal", "ZealInput");
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
    //Zeal::EqGame::print_chat("EditWnd: 0x%x key: %x modifier: %i state: %i", active_edit, key, modifier, keydown);
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
    return ZealService::get_instance()->hooks->hook_map["EditWndHandleKey"]->original(EditWndHandleKey)(active_edit, u, key, modifier, keydown);
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
    zeal->commands_hook->add("/timestamp", { "/tms" },
        [this](std::vector<std::string>& args) {
            set_timestamp(!timestamps);
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->add("/zealinput", { "/zinput" },
        [this](std::vector<std::string>& args) {
            set_input(!zealinput);
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->add("/bluecon", { },
        [this](std::vector<std::string>& args) {
            set_bluecon(!bluecon);
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    LoadSettings(ini);

    zeal->hooks->Add("PrintChat", 0x537f99, PrintChat, hook_type_detour); //add extra prints for new loot types
    zeal->hooks->Add("EditWndHandleKey", 0x5A3010, EditWndHandleKey, hook_type_detour); //this makes more sense than the hook I had previously
  
}
void chat::set_input(bool val)
{
    zealinput = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "ZealInput", zealinput);
    if (zealinput)
        Zeal::EqGame::print_chat("Zeal special input enabled");
    else
        Zeal::EqGame::print_chat("Zeal special input disabled");
    ZealService::get_instance()->ui->options->UpdateOptions();
}
void chat::set_timestamp(bool val)
{
    timestamps = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "ChatTimestamps", timestamps);
    if (timestamps)
        Zeal::EqGame::print_chat("Timestamps enabled");
    else
        Zeal::EqGame::print_chat("Timestamps disabled");
    ZealService::get_instance()->ui->options->UpdateOptions();
}
void chat::set_bluecon(bool val)
{
    bluecon = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "Bluecon", bluecon);
    if (bluecon)
        Zeal::EqGame::print_chat("Blue con color is now set to usercolor 70");
    else
        Zeal::EqGame::print_chat("Default blue con color.");
    ZealService::get_instance()->ui->options->UpdateOptions();
}
chat::~chat()
{
}

