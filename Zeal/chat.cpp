#include "chat.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"

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

void __fastcall PrintChat(int t, int unused, const char* data, short color, bool u)
{
	chat* c = ZealService::get_instance()->chat_hook.get();
    if (c->timestamps && strlen(data)>0) //remove phantom prints (the game also checks this, no idea why they are sending blank data in here sometimes
    {
        mem::write<byte>(0x5380C9, 0xEB); // don't log information so we can manipulate data before between chat and logs
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, generateTimestampedString(data).c_str(), color, u);
        mem::write<byte>(0x5380C9, 0x75); //reset the logging

        mem::write<byte>(0x53805B, 0xEB); //disable print chat for NewUI
        mem::write<byte>(0x538090, 0xEB); //disable print chat for OldUI
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, data, color, u);
        mem::write<byte>(0x538090, 0x74); //reset print chat for OldUI
        mem::write<byte>(0x53805B, 0x74); //reset print chat for NewUI
    }
    else
        ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(t, unused, data, color, u);

}

void chat::LoadSettings(IO_ini* ini)
{
	if (!ini->exists("Zeal", "ChatTimestamps"))
		ini->setValue<bool>("Zeal", "ChatTimestamps", false);

	timestamps = ini->getValue<bool>("Zeal", "ChatTimestamps");
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
	LoadSettings(ini);

	zeal->hooks->Add("PrintChat", 0x537f99, PrintChat, hook_type_detour); //add extra prints for new loot types
}
chat::~chat()
{
    
}
