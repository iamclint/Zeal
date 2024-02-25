#include "raid.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"

void raid::callback_main()
{

}

raid::~raid()
{

}

raid::raid(ZealService* zeal)
{
	zeal->main_loop_hook->add_callback([this]() { callback_main(); });
}
