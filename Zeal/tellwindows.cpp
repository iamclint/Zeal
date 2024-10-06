#include "tellwindows.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include <regex>

//people will see this commit and be like OMG, then they will see this comment and message the discord channel.
//such hopes and dreams -- its on the list!
std::string TellWindowIdentifier = " ";

void RestoreWindowState(Zeal::EqUI::ChatWnd* wnd)
{
    while (wnd->ParentWnd)
    {
        wnd = (Zeal::EqUI::ChatWnd*)wnd->ParentWnd;
    }
    if (wnd->IsMinimized)
        wnd->MinimizeToggle();
}

Zeal::EqUI::ChatWnd* __fastcall GetActiveChatWindow(Zeal::EqUI::CChatManager* cm, int unused)
{
    //fix up a little bit so the active window for things like linking items don't go to your always chat here if you are using tell windows
    if (ZealService::get_instance()->tells && ZealService::get_instance()->tells->enabled)
    {
        Zeal::EqUI::ChatWnd* wnd = cm->ChatWindows[Zeal::EqGame::Windows->ChatManager->ActiveChatWnd];
        if (ZealService::get_instance()->tells->IsTellWindow(wnd))
            return wnd;
    }
    return ZealService::get_instance()->hooks->hook_map["GetActiveChatWindow"]->original(GetActiveChatWindow)(cm, unused);
}
Zeal::EqUI::ChatWnd* TellWindows::FindPreviousTellWnd()
{
    Zeal::EqUI::ChatWnd* wnd = nullptr;
    if (Zeal::EqGame::Windows->ChatManager->ActiveChatWnd > 0)
    {
        for (int i = Zeal::EqGame::Windows->ChatManager->ActiveChatWnd - 1; i != Zeal::EqGame::Windows->ChatManager->ActiveChatWnd; i--)
        {
            if (i<=-1)
                i = Zeal::EqGame::Windows->ChatManager->MaxChatWindows;
          
            wnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[i];
            if (IsTellWindow(wnd))
                return wnd;
        }
    }
    return nullptr;
}
Zeal::EqUI::ChatWnd* TellWindows::FindNextTellWnd()
{
    Zeal::EqUI::ChatWnd* wnd = nullptr;

    for (int i = Zeal::EqGame::Windows->ChatManager->ActiveChatWnd + 1 ; i != Zeal::EqGame::Windows->ChatManager->ActiveChatWnd; i++)
    {
        if (i == Zeal::EqGame::Windows->ChatManager->MaxChatWindows)
            i = 0;

        wnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[i];
        if (IsTellWindow(wnd))
            return wnd;
    }
    return nullptr;
}

bool TellWindows::HandleKeyPress(int key, bool down, int modifier)
{
    if (!enabled || !Zeal::EqGame::Windows || !Zeal::EqGame::Windows->ChatManager)
        return false;
    Zeal::EqUI::ChatWnd* wnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[Zeal::EqGame::Windows->ChatManager->ActiveChatWnd];
    if (IsTellWindow(wnd))
    {
        if (key == 0xf && down) //tab and shift tab to cycle through tell windows
        {
            Zeal::EqUI::ChatWnd* focus_wnd = nullptr;
            if (modifier==1)
                focus_wnd=FindPreviousTellWnd();
            else
                focus_wnd=FindNextTellWnd();
            if (focus_wnd && focus_wnd->edit)
            {
                RestoreWindowState(focus_wnd);
                focus_wnd->edit->SetFocus();
                return true;
            }
        }
    }
    return false;
}
bool TellWindows::HandleTell(std::string& cmd_data)
{
    if (!enabled)
        return false;
    if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->ChatManager)
    {
        Zeal::EqUI::ChatWnd* wnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[Zeal::EqGame::Windows->ChatManager->ActiveChatWnd];
        if (IsTellWindow(wnd))
        {
                std::string window_title = wnd->Text.Data->Text;
                cmd_data = "/tell " + window_title.substr(1, window_title.length()-1) + " " + cmd_data;
                return true;
        }
    }
    return false;
}

Zeal::EqUI::ChatWnd* TellWindows::FindTellWnd(std::string& name)
{
    for (int i = 0; i < Zeal::EqGame::Windows->ChatManager->MaxChatWindows; i++)
    {
        Zeal::EqUI::ChatWnd* cwnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[i];
        if (cwnd && cwnd->Text.Data)
        {
            std::string title = cwnd->Text.Data->Text;
            if (IsTellWindow(cwnd) && Zeal::String::compare_insensitive(title.substr(1, title.length() - 1), name))
                return cwnd;
        }
    }
    return nullptr;
}

std::string GetName(std::string& data)
{
    std::string lower_msg = data;
    std::transform(lower_msg.begin(), lower_msg.end(), lower_msg.begin(), ::tolower);

    // Regex pattern for matching exactly one word before "tells you"
    std::regex tells_pattern(R"((?:^|\]\s*)(\b\w+\b)\s+tells\s+you)");
    // Regex pattern for matching exactly one word after "you told"
    std::regex told_pattern(R"((?:^|\]\s*)you\s+told\s+(\b\w+\b))");

    std::smatch match;

    // Check for "tells you" pattern with only one word before it
    if (std::regex_search(lower_msg, match, tells_pattern)) {
        return match[1].str();  // Return the matched single word before "tells you"
    }
    // Check for "you told" pattern with only one word after it
    else if (std::regex_search(lower_msg, match, told_pattern)) {
        return match[1].str();  // Return the matched single word after "you told"
    }

    return "";  // Return an empty string if no match found
}

void TellWindows::AddOutputText(Zeal::EqUI::ChatWnd*& wnd, std::string msg, short channel)
{
    if (!ZealService::get_instance()->tells->enabled) //just early out if tell windows are not enabled
            return;
    if (channel == USERCOLOR_TELL || channel== USERCOLOR_ECHO_TELL) //tell channel
    {
        std::string name = GetName(msg);
        if (name.length())
        {
            name[0] = std::toupper(name[0]);
            Zeal::EqUI::ChatWnd* tell_window = ZealService::get_instance()->tells->FindTellWnd(name);
            if (!tell_window)
            {
                std::string WinName = TellWindowIdentifier + name;
                std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
                IO_ini ui_ini(ini_name);
                int font_size = ui_ini.getValue<int>("ChatManager", "ChatWindow0_FontStyle");
                font_size = std::clamp(font_size, 1, 6);
                Zeal::EqGame::Windows->ChatManager->CreateChatWindow(WinName.c_str(), 0, 3, -1, "", font_size);
                tell_window = ZealService::get_instance()->tells->FindTellWnd(name);
                wnd = tell_window;
            }
            else 
            {
                wnd = tell_window;
                RestoreWindowState(wnd);
            }
        }
    }
}


bool TellWindows::IsTellWindow(Zeal::EqUI::ChatWnd* wnd)
{
    if (wnd && wnd->Text.Data)
    {
        std::string title = wnd->Text.Data->Text;
        if (title.substr(0, 1) == TellWindowIdentifier)
        {
            return true;
        }
    }
    return false;
}

void TellWindows::CleanUI()
{
    if (!Zeal::EqGame::Windows || !Zeal::EqGame::Windows->ChatManager || !enabled)
        return;
    for (int i = 0; i < Zeal::EqGame::Windows->ChatManager->MaxChatWindows; i++)
    {
        Zeal::EqUI::ChatWnd* cwnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[i];
        if (IsTellWindow(cwnd))
            cwnd->show(false, false);
    }
}

void TellWindows::SetEnabled(bool val)
{
    enabled = val;
    if (Zeal::EqGame::get_self() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>(Zeal::EqGame::get_self()->Name, "TellWindows", val);
    ZealService::get_instance()->ui->options->UpdateOptions();
}

void TellWindows::LoadUI()
{
    IO_ini* ini = ZealService::get_instance()->ini.get();
    if (!ini->exists(Zeal::EqGame::get_self()->Name, "TellWindows"))
        ini->setValue<bool>(Zeal::EqGame::get_self()->Name, "TellWindows", false);
    enabled = ini->getValue<bool>(Zeal::EqGame::get_self()->Name, "TellWindows");
    ZealService::get_instance()->ui->options->UpdateOptions();
}


void __fastcall DeactivateChatManager(Zeal::EqUI::CChatManager* t, int u) 
{
    //toggle the tell windows to not load on next game load
    ZealService::get_instance()->hooks->hook_map["DeactivateChatManager"]->original(DeactivateChatManager)(t, u);
    std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
    IO_ini ini(ini_name);
    for (int i = 0; i < t->MaxChatWindows; i++)
    {
        if (ZealService::get_instance()->tells->IsTellWindow(t->ChatWindows[i]))
        {
            ini.setValue<std::string>("ChatManager", "ChatWindow" + std::to_string(i) + "_Language", "@42");
        }
    }
}

TellWindows::TellWindows(ZealService* zeal, IO_ini* ini)
{
    zeal->hooks->Add("GetActiveChatWindow", 0x425D27, GetActiveChatWindow, hook_type_replace_call);//hook to fix item linking to tell windows if always chat here is selected anywhere
    zeal->hooks->Add("DeactivateChatManager", 0x410871, DeactivateChatManager, hook_type_detour);
    //zeal->hooks->Add("DeactivateMainUI", 0x4a7705, DeactivateMainUI, hook_type_detour); //clean up tell windows just before they save
    //zeal->callbacks->AddGeneric([this]() { Deactivate_Window(); }, callback_type::DeactivateUI);
    zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
    zeal->callbacks->AddGeneric([this]() { LoadUI(); }, callback_type::InitUI);
    zeal->callbacks->AddOutputText([this](Zeal::EqUI::ChatWnd*& wnd, std::string msg, short channel) { this->AddOutputText(wnd, msg, channel); });

    zeal->commands_hook->Add("/tellwindows", {}, "Toggle tell windows",
        [this](std::vector<std::string>& args) {
            enabled = !enabled;
            if (enabled)
                Zeal::EqGame::print_chat("Tell windows enabled.");
            else
                Zeal::EqGame::print_chat("Tell windows disabled.");

            SetEnabled(enabled);
            return true;
        });


    zeal->binds_hook->replace_cmd(0x3B, [this](int state)
        {
            if (!enabled)
                return false;
            if (state && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
            {
                int last_tell_index = *(int*)0x7cf0dc;
                char* reply_to = (char*)(0x7CE45C + (last_tell_index * 0x40));
                std::string reply_to_str = reply_to;
                Zeal::EqUI::ChatWnd* wnd = FindTellWnd(reply_to_str);
                if (wnd && wnd->edit)
                {
                    RestoreWindowState(wnd);
                    wnd->edit->SetFocus();
                    return true;
                }
                else
                    return false;
            }
            return false;

        }); //reply hotkey


    zeal->binds_hook->replace_cmd(0xCE, [this](int state)
        {
            if (!enabled)
                return false;

            if (state && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
            {
                if (Zeal::EqGame::Windows->ChatManager->AlwaysChatHereIndex>=0)
                    Zeal::EqGame::Windows->ChatManager->ActiveChatWnd = Zeal::EqGame::Windows->ChatManager->AlwaysChatHereIndex;
            }
            return false;
        }); //chat hotkey



}

TellWindows::~TellWindows()
{
}