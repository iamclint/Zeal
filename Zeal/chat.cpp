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

        mem::write<byte>(0x53805B, 0xEB); //disable print chat for NewUI
        mem::write<byte>(0x538090, 0xEB); //disable print chat for OldUI
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, data, color_index, u);
        mem::write<byte>(0x538090, 0x74); //reset print chat for OldUI
        mem::write<byte>(0x53805B, 0x74); //reset print chat for NewUI
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

enum last_caret_dir
{
    none,
    left,
    right
};

void __fastcall HandleKey(int t, int u, UINT32 key, int KeyDown)
{
    Zeal::EqUI::CXWndManager* mgr = Zeal::EqGame::get_wnd_manager();
    if (!Zeal::EqGame::is_new_ui() || !ZealService::get_instance()->chat_hook->zealinput || !mgr->ActiveEdit)
        return ZealService::get_instance()->hooks->hook_map["HandleKey"]->original(HandleKey)(t, u, key, KeyDown);
     
    Zeal::EqUI::EditWnd* active_edit = mgr->ActiveEdit;

    //Zeal::EqGame::print_chat("Wants input: %i  keydown: %i  activedit: %x  focused: %x start: %i end: %i", Zeal::EqGame::game_wants_input(), KeyDown, mgr->ActiveEdit, mgr->Focused, active_edit->Caret_Start, active_edit->Caret_End);
    if (Zeal::EqGame::game_wants_input() && KeyDown)
    {
        if (!active_edit->InputText.Data)
            active_edit->InputText.Assure(32, 0);

        static last_caret_dir last_highlight_dir = last_caret_dir::none;
        
            switch (key)
            {
            case 0x1: //escape
            {
                mgr->Focused = active_edit->ParentWnd;
                return;
            }
            case 0xC7: //home
            {
                active_edit->Caret_Start = 0;
                if (!Zeal::EqGame::KeyMods->Shift)
                    active_edit->Caret_End = 0;
                last_highlight_dir = last_caret_dir::left;
                return;
            }
            case 0xCF: //end
            {
                active_edit->Caret_End = active_edit->GetInputLength();
                if (!Zeal::EqGame::KeyMods->Shift)
                    active_edit->Caret_Start = active_edit->Caret_End;
                last_highlight_dir = last_caret_dir::right;
                return;
            }
            case 0xCB: //left arrow

                if (last_highlight_dir == last_caret_dir::right && Zeal::EqGame::KeyMods->Shift && active_edit->Caret_End != active_edit->Caret_Start)
                {
                    active_edit->Caret_End--;
                }
                else
                {
                    if (Zeal::EqGame::KeyMods->Shift)
                    {
                        if (active_edit->Caret_Start > 0)
                        {
                            int move_index = active_edit->Caret_Start - 1;
                            for (int i = 0; i < active_edit->item_link_count; i++)
                            {
                                if (active_edit->LinkEndIndex[i] == move_index)
                                {
                                    move_index = active_edit->LinkStartIndex[i]-1;
                                    break;
                                }
                            }
                            active_edit->Caret_Start = move_index;
                        }
                        last_highlight_dir = last_caret_dir::left;
                    }
                    else
                    {
                        if (active_edit->Caret_Start > 0)
                        {
                            int move_index = active_edit->Caret_Start - 1;
                            for (int i = 0; i < active_edit->item_link_count; i++)
                            {
                                if (active_edit->LinkEndIndex[i] == move_index)
                                {
                                    move_index = active_edit->LinkStartIndex[i]-1;
                                    break;
                                }
                            }
                            active_edit->Caret_Start = move_index;
                        }
                        active_edit->Caret_End=active_edit->Caret_Start;
                    }
                    
                }
                if (active_edit->Caret_End == active_edit->Caret_Start)
                    last_highlight_dir = last_caret_dir::none;
                return;
            case 0xCD: //right arrow
                if (last_highlight_dir == last_caret_dir::left && Zeal::EqGame::KeyMods->Shift && active_edit->Caret_End != active_edit->Caret_Start)
                {
                    active_edit->Caret_Start++;
                }
                else
                {
                    if (Zeal::EqGame::KeyMods->Shift)
                    {
                        if (active_edit->Caret_End < active_edit->GetInputLength())
                        {
                            int move_index = active_edit->Caret_End + 1;
                            for (int i = 0; i < active_edit->item_link_count; i++)
                            {
                                if (active_edit->LinkStartIndex[i] == move_index)
                                {
                                    move_index = active_edit->LinkEndIndex[i];
                                    break;
                                }
                            }
                            active_edit->Caret_End=move_index;
                        }
                        last_highlight_dir = last_caret_dir::right;
                    }
                    else
                    {
                        if (active_edit->Caret_End < active_edit->GetInputLength())
                        {
                            int move_index = active_edit->Caret_End + 1;
                            for (int i = 0; i < active_edit->item_link_count; i++)
                            {
                                if (active_edit->LinkStartIndex[i] == move_index)
                                {
                                    move_index = active_edit->LinkEndIndex[i];
                                    break;
                                }
                            }
                            active_edit->Caret_End = move_index;
                        }
                        active_edit->Caret_Start = active_edit->Caret_End;
                    }

                }
                if (active_edit->Caret_End == active_edit->Caret_Start)
                    last_highlight_dir = last_caret_dir::none;
                return;
            }
        
        if (!Zeal::EqGame::KeyMods->Shift)
            last_highlight_dir = last_caret_dir::none;

        if (Zeal::EqGame::KeyMods->Ctrl)
        {
            switch (key)
            {
                case 0x2F: //v
                {
                    active_edit->next_is_item_link = 1;
                    std::string temp_text = StripSpecialCharacters(ReadFromClipboard());
                    active_edit->ReplaceSelection(temp_text.c_str(), false);
                    return;
                }
                case 0x1E: //a
                {
                    active_edit->Caret_Start = 0;
                    active_edit->Caret_End = active_edit->GetInputLength();
                    return;
                }
                case 0x2E: //c
                {
                    if (active_edit->Caret_End - active_edit->Caret_Start > 0)
                    {
                        int len_highlighted = active_edit->Caret_End - active_edit->Caret_Start;
                        int link_count = 0;
                        for (int i = 0; i < active_edit->item_link_count; i++)
                        {
                            if (active_edit->LinkStartIndex[i] <= active_edit->Caret_End && active_edit->LinkStartIndex[i]>=active_edit->Caret_Start)
                            {
                                link_count++;
                                break;
                            }
                        }
                        std::string highlighted_text(active_edit->InputText.Data->Text + active_edit->Caret_Start, active_edit->InputText.Data->Text + active_edit->Caret_End + (link_count*9));
                        SetClipboardText(highlighted_text);
                    }
                    return;
                }
            }
        }
    }

    ZealService::get_instance()->hooks->hook_map["HandleKey"]->original(HandleKey)(t,u,key,KeyDown);
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
    //zeal->main_loop_hook->add_callback([this]() { 
    //    if (zealinput && Zeal::EqGame::game_wants_input())
    //    {
    //        if (zealinput)
    //            set_input_color(Zeal::EqUI::ARGBCOLOR(0xFF, 0xEE, 0xAA, 0xFF));//goldish input color
    //        else
    //            set_input_color(Zeal::EqUI::ARGBCOLOR(0xFF, 0xFF, 0xFF, 0xFF));
    //    }
    //});
    zeal->commands_hook->add("/timestamp", { "/tms" },
        [this](std::vector<std::string>& args) {
            timestamps = !timestamps;
            ZealService::get_instance()->ini->setValue<bool>("Zeal", "ChatTimestamps", timestamps);
            if (timestamps)
                Zeal::EqGame::print_chat("Timestamps enabled");
            else
                Zeal::EqGame::print_chat("Timestamps disabled");

            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->add("/zealinput", { "/zinput" },
        [this](std::vector<std::string>& args) {
            zealinput = !zealinput;
            ZealService::get_instance()->ini->setValue<bool>("Zeal", "ZealInput", zealinput);
            if (zealinput)
                Zeal::EqGame::print_chat("Zeal special input enabled");
            else
                Zeal::EqGame::print_chat("Zeal special input disabled");

            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->add("/bluecon", { },
        [this](std::vector<std::string>& args) {
            bluecon = !bluecon;
            ZealService::get_instance()->ini->setValue<bool>("Zeal", "Bluecon", bluecon);
            if (bluecon)
                Zeal::EqGame::print_chat("Blue con color is now set to usercolor 70");
            else
                Zeal::EqGame::print_chat("Default blue con color.");

            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    LoadSettings(ini);

    zeal->hooks->Add("PrintChat", 0x537f99, PrintChat, hook_type_detour); //add extra prints for new loot types
    zeal->hooks->Add("HandleKey", 0x59e780, HandleKey, hook_type_detour); //add extra prints for new loot types
}
chat::~chat()
{
   // set_input_color(Zeal::EqUI::ARGBCOLOR(0xFF, 0xFF, 0xFF, 0xFF));
}

