#include "netstat.h"
#include "Zeal.h"

static BYTE* netstat_flag = (BYTE*)0x7985EC;
static bool netstat_flag_visible() { return *netstat_flag; }

void Netstat::toggle_netstat(int keydown)
{
	if (!Zeal::EqGame::game_wants_input() && keydown)
	{
		is_visible = !is_visible;
		update_netstat_state();
	}
}

void Netstat::callback_main()
{
	// uncomment the below line when character select callback functions properly.
	// netstat_flag_was_reset = true;

	// comment the below out in the event that the character select callbacks are functioning properly.
	mem::write<byte>(0x7985EC, (int)is_visible);
}

void Netstat::callback_characterselect()
{
	// Can't seem to trigger the character select callback into working properly. Fallback to continuous memory writes on main.
	if (netstat_flag_was_reset)
	{
		mem::write<byte>(0x7985EC, 0);
		netstat_flag_was_reset = false;
	}
}

void Netstat::load_settings()
{
	if (!ini_handle->exists("Zeal", "NetstatVisibilityState"))
		ini_handle->setValue<bool>("Zeal", "NetstatVisibilityState", true);

	is_visible = ini_handle->getValue<bool>("Zeal", "NetstatVisibilityState");
}

void Netstat::update_netstat_state()
{
	mem::write<byte>(0x7985EC, (int)is_visible);
	ini_handle->setValue<bool>("Zeal", "NetstatVisibilityState", is_visible);
}

Netstat::Netstat(ZealService* zeal, class IO_ini* ini)
{
	ini_handle = ini;
	load_settings();

	zeal->callbacks->AddGeneric([this]() { callback_main(); }, callback_type::MainLoop);
	//zeal->main_loop_hook->add_callback([this]() { callback_characterselect(); }, callback_fn::CharacterSelect);
}