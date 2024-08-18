#include "tellwindows.h"
#include "Zeal.h"
#include "EqAddresses.h"


//people will see this commit and be like OMG, then they will see this comment and message the discord channel.
//such hopes and dreams -- its on the list!
TellWindows::TellWindows(ZealService* zeal, IO_ini* ini)
{
    zeal->commands_hook->Add("/chatwnd", { }, "Creates a new chat window",
        [this](std::vector<std::string>& args) {
            if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->ChatManager)
            {
               Zeal::EqUI::ChatWnd* wnd =  Zeal::EqGame::Windows->ChatManager->CreateChatWindow();
               if (wnd && wnd->INIStorageName.Data)
               {
                   Zeal::EqGame::CXStr_PrintString(&wnd->Text, "%s", "Some Tell");//casting_spell->Name);
                //   Zeal::EqGame::print_chat(wnd->INIStorageName.Data->Text);
               }
               
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
}

TellWindows::~TellWindows()
{
}