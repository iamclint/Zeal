#include "tooltip.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "string_util.h"
#include "Zeal.h"



Tooltip::~Tooltip()
{

}

void Tooltip::set_timer(int _time)
{
    hover_timeout.set(_time);
    mem::write<int>(0x59E10D, hover_timeout.get());
}

void Tooltip::set_alt_all_containers(bool enabled)
{
    all_containers = enabled;
    if (enabled)
    {
        mem::set(0x416D1D, 0x90, 2); //remove the hovered check for showing tooltips while holding alt for containers
        mem::set(0x416D22, 0x90, 2);//remove the hovered check for showing tooltips while holding alt for containers
    }
    else
    {
        mem::write<byte[2]>(0x416d1d, { 0x74, 0x74 });
        mem::write<byte[2]>(0x416D22, { 0x74, 0x6F });
    }
    //Zeal::EqGame::print_chat("Tooltips for all containers while holding alt: %s", all_containers ? "enabled" : "disabled");
}


Tooltip::Tooltip(ZealService* zeal)
{
    mem::write<byte>(0x59e112, 0x82); //change from jbe to jb so if you disable the tooltip timer completely it doesnt flash as you move your cursor (unnoticeable change for normal)
    set_alt_all_containers(all_containers.get());
    set_timer(hover_timeout.get());
    zeal->commands_hook->Add("/tooltipall", {}, "Toggle showing all open containers tooltips when holding alt.",
        [this](std::vector<std::string>& args) {
            set_alt_all_containers(!all_containers.get());
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/tooltiptimer", { "/ttimer"}, "Set the time limit before a tooltip appears.",
        [this](std::vector<std::string>& args) {
            int timeout = 0;
            if (args.size() > 1 && Zeal::String::tryParse(args[1], &timeout))
            {
                set_timer(timeout);
                Zeal::EqGame::print_chat("Tooltip hover timer now set to %i", hover_timeout);
            }
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });

}
