#include "chat.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"

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
bool SetClipboardText(const std::string& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1); // +1 for null terminator
        if (hMem != nullptr) {
            char* pData = static_cast<char*>(GlobalLock(hMem));
            if (pData != nullptr) {
                memcpy(pData, text.c_str(), text.size() + 1);
                GlobalUnlock(hMem);
                SetClipboardData(CF_TEXT, hMem);
                CloseClipboard();
                return true;
            }
            GlobalFree(hMem);
        }
        CloseClipboard();
    }
    return false;
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

    bluecon = ini->getValue<bool>("Zeal", "Bluecon");
    timestamps = ini->getValue<bool>("Zeal", "ChatTimestamps");
}

void __fastcall HandleKey(int t, int u, UINT32 key, int KeyDown)
{
    Zeal::EqUI::CXWndManager* mgr = Zeal::EqGame::get_wnd_manager();
    Zeal::EqUI::EQWND* active_chat = mgr->ActiveEdit;
    if (Zeal::EqGame::game_wants_input() && KeyDown && mgr->ActiveEdit == mgr->Focused)
    {

        Zeal::EqGame::print_chat("chat key: %x  uk: %x", key, KeyDown);
        static int last_highlight_dir = 0;

            switch (key)
            {
            case 0x1: //escape
            {
                mgr->Focused = active_chat->ParentWnd;
                return;
            }
            case 0xC7: //home
            {
                active_chat->Caret_Start = 0;
                if (!Zeal::EqGame::KeyMods->Shift)
                    active_chat->Caret_End = 0;
                last_highlight_dir = 1;
                return;
            }
            case 0xCF: //end
            {
                active_chat->Caret_End = active_chat->Text.ptr->Length;
                if (!Zeal::EqGame::KeyMods->Shift)
                    active_chat->Caret_Start = active_chat->Caret_End;
                last_highlight_dir = 2;
                return;
            }
            case 0xCB: //left arrow
                if (active_chat->Caret_Start > 0)
                {
                    if (last_highlight_dir==2 && Zeal::EqGame::KeyMods->Shift) //right
                        active_chat->Caret_End--;
                    else if (active_chat->Caret_Start)
                    {
                        active_chat->Caret_Start--;
                        if (!Zeal::EqGame::KeyMods->Shift)
                            active_chat->Caret_End = active_chat->Caret_Start;
                        last_highlight_dir = 1;
                    }

                    if (active_chat->Caret_End == active_chat->Caret_Start)
                        last_highlight_dir = 0;
                }
                return;
            case 0xCD: //right arrow
                if (last_highlight_dir == 1 && Zeal::EqGame::KeyMods->Shift) //left
                    active_chat->Caret_Start++;
                else if (active_chat->Caret_End < active_chat->Text.ptr->Length)
                {
                    active_chat->Caret_End++;
                    if (!Zeal::EqGame::KeyMods->Shift)
                        active_chat->Caret_Start= active_chat->Caret_End;
                    last_highlight_dir = 2;
                }

                if (active_chat->Caret_End == active_chat->Caret_Start)
                    last_highlight_dir = 0;
                return;
            }
        
        if (!Zeal::EqGame::KeyMods->Shift)
            last_highlight_dir = 0;

        if (Zeal::EqGame::KeyMods->Ctrl)
        {
            switch (key)
            {
                case 0x2F: //v
                {
                    const char* temp_text = StripSpecialCharacters(ReadFromClipboard()).c_str();
                    int str_pos = (int)&(active_chat->Text.ptr->Text) + (active_chat->Caret_Start);
                    mem::copy(str_pos, (int)temp_text, strlen(temp_text));
                    active_chat->Text.ptr->Length += strlen(temp_text);
                    active_chat->Text.ptr->Encoding = 0;
                    active_chat->Caret_Start += strlen(temp_text);
                    active_chat->Caret_End = active_chat->Caret_Start;
                    return;
                }
                case 0x1E: //a
                {
                    active_chat->Caret_Start = 0;
                    active_chat->Caret_End = active_chat->Text.ptr->Length;
                    return;
                }
                case 0x2E: //c
                {
                    if (active_chat->Caret_End - active_chat->Caret_Start > 0)
                    {
                        int len = active_chat->Caret_End - active_chat->Caret_Start;
                        int start = active_chat->Caret_Start;
                    }
                    return;
                }
            }
        }
    }

    ZealService::get_instance()->hooks->hook_map["HandleKey"]->original(HandleKey)(t,u,key,KeyDown);
}




chat::chat(ZealService* zeal, IO_ini* ini)
{
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
}
