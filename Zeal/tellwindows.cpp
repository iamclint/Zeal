#include "tellwindows.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"

//people will see this commit and be like OMG, then they will see this comment and message the discord channel.
//such hopes and dreams -- its on the list!

bool TellWindows::HandleTell(std::string& cmd_data)
{
    if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->ChatManager)
    {
        Zeal::EqUI::ChatWnd* wnd = Zeal::EqGame::Windows->ChatManager->GetActiveChatWindow();
        if (wnd)
        {
            std::string window_title = wnd->Text.Data->Text;
            if (window_title.substr(0,1) == "*")
            {
                cmd_data = "/tell " + window_title.substr(1, window_title.length()-1) + " " + cmd_data;
                return true;
            }
        }
    }
    return false;
}

Zeal::EqUI::ChatWnd* FindTellWnd(std::string& name)
{
    for (int i = 0; i < Zeal::EqGame::Windows->ChatManager->MaxChatWindows; i++)
    {
        Zeal::EqUI::ChatWnd* cwnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[i];
        if (cwnd && cwnd->Text.Data)
        {
            std::string title = cwnd->Text.Data->Text;
            if (title.substr(0, 1) == "*" && Zeal::String::compare_insensitive(title.substr(1, title.length() - 1), name))
                return cwnd;
        }
    }
    return nullptr;
}

std::string GetName(std::string& data)
{
    std::string lower_msg = data;
    std::transform(lower_msg.begin(), lower_msg.end(), lower_msg.begin(), ::tolower);

    // Define phrases for both scenarios
    std::string told_phrase = "you told ";
    std::string tells_phrase = " tells you";

    std::size_t start_pos;
    std::string name="";
    bool found_name = false;

    // Check if the message indicates that you told someone
    if ((start_pos = lower_msg.find(told_phrase)) != std::string::npos) {
        start_pos += told_phrase.length();
        std::size_t end_pos = lower_msg.find(',', start_pos);
        if (end_pos != std::string::npos)
        {
            name = lower_msg.substr(start_pos, end_pos - start_pos);
            found_name = true;
        }
    }
    else if ((start_pos = lower_msg.find(tells_phrase)) != std::string::npos) {
        std::size_t end_pos = lower_msg.rfind(' ', start_pos - 1); // Find the space before the sender's name
        if (end_pos != std::string::npos)
        {
            name = lower_msg.substr(end_pos + 1, start_pos - end_pos - 1);
            found_name = true;
        }
    }
    return name;
}


void __fastcall AddOutputText(Zeal::EqUI::ChatWnd* wnd, int u, Zeal::EqUI::CXSTR msg, byte channel)
{
    int multiByteSize = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)msg.Data->Text, -1, NULL, 0, NULL, NULL);
    char* multiByteStr = new char[multiByteSize];
    WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)msg.Data->Text, -1, multiByteStr, multiByteSize, NULL, NULL);
    std::string msg_data = multiByteStr;
    std::string name = GetName(msg_data);
    if (name.length())
    {
        Zeal::EqUI::ChatWnd* tell_window = FindTellWnd(name);
        if (!tell_window)
        {
            std::string WinName = "*" + name;
            Zeal::EqGame::Windows->ChatManager->CreateChatWindow(WinName.c_str(), 0, 3, -1, "", 3);
            tell_window = FindTellWnd(name);
        }

        if (tell_window)
            wnd = tell_window;
    }
    ZealService::get_instance()->hooks->hook_map["AddOutputText"]->original(AddOutputText)(wnd, u, msg, channel);
}

void TellWindows::CleanUI()
{
    if (!Zeal::EqGame::Windows || !Zeal::EqGame::Windows->ChatManager)
        return;
    for (int i = 0; i < Zeal::EqGame::Windows->ChatManager->MaxChatWindows; i++)
    {
        Zeal::EqUI::ChatWnd* cwnd = Zeal::EqGame::Windows->ChatManager->ChatWindows[i];
        if (cwnd && cwnd->Text.Data)
        {
            std::string title = cwnd->Text.Data->Text;
            if (title.substr(0, 1) == "*")
            {
                cwnd->show(0, false);
                break;
            }
        }
    }
}

TellWindows::TellWindows(ZealService* zeal, IO_ini* ini)
{
    zeal->binds_hook->replace_cmd(0x3B, [this](int state)
        {
            if (state && !Zeal::EqGame::EqGameInternal::UI_ChatInputCheck())
            {
                int last_tell_index = *(int*)0x7cf0dc;
                char* reply_to = (char*)(0x7CE45C + (last_tell_index * 0x40));
                std::string reply_to_str = reply_to;
                Zeal::EqUI::ChatWnd* wnd = FindTellWnd(reply_to_str);
                if (wnd && wnd->edit)
                {
                    wnd->edit->SetFocus();
                    return true;
                }
                else
                    return false;
            }
            return false;

        }); //reply hotkey

    zeal->hooks->Add("AddOutputText", 0x4139A2, AddOutputText, hook_type_detour);
    zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);

}

TellWindows::~TellWindows()
{
}