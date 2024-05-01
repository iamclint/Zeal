#include "tooltip.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "string_util.h"
#include "Zeal.h"



tooltip::~tooltip()
{

}

void tooltip::set_timer(int _time)
{
    time = _time;
    mem::write<int>(0x59E10D, time);
    ZealService::get_instance()->ini->setValue<float>("Zeal", "TooltipTime", time);
//    ZealService::get_instance()->ui->options->UpdateOptions();
}
void tooltip::LoadSettings(IO_ini* ini)
{
    if (!ini->exists("Zeal", "TooltipTime"))
        ini->setValue<int>("Zeal", "TooltipTime", 500);
    time = ini->getValue<bool>("Zeal", "ZealInput");
}

tooltip::tooltip(ZealService* zeal, IO_ini* ini)
{
    mem::write<byte>(0x59e112, 0x82); //change from jbe to jb so if you disable the tooltip timer completely it doesnt flash as you move your cursor (unnoticeable change for normal)
	LoadSettings(ini);
    set_timer(time);
    zeal->commands_hook->add("/tooltiptimer", { "/ttimer"}, "Set the time limit before a tooltip appears.",
        [this](std::vector<std::string>& args) {
            float timeout = 0;
            if (args.size() > 1 && Zeal::String::tryParse(args[1], &timeout))
            {
                set_timer(timeout);
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });

}
