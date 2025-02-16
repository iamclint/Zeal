#include "tick.h"
#include "Zeal.h"
#include "EqStructures.h"
#include "EqPackets.h"

constexpr DWORD kAverageTickDuration = 6010;
constexpr DWORD kGaugeScale = 1000;

constexpr const char* TICK_MESSAGE = "Tick";

UINT64 LastKnownServerTick = 0;

DWORD Tick::GetTimeUntilTick()
{
	if (LastKnownServerTick != 0) // Don't calculate until tick is known
	{
		DWORD now_point = GetTickCount64() % kAverageTickDuration;
		DWORD tick_point = LastKnownServerTick % kAverageTickDuration;
		DWORD millis_until_tick = tick_point >= now_point
			? tick_point - now_point
			: kAverageTickDuration - (now_point - tick_point);
		return millis_until_tick;
	}
	return 0;
}

DWORD Tick::GetTickGauge(Zeal::EqUI::CXSTR* str)
{
	DWORD value = GetTimeUntilTick();

	// The seconds remaining (1-6)
	DWORD seconds_display = (value / 1000) + 1;
	if (seconds_display > 6)
		seconds_display = 6;
	Zeal::EqGame::CXStr_PrintString(str, "%u", seconds_display);

	// Scales the gauge from 0-1000
	value = value * kGaugeScale / kAverageTickDuration;

	// Toggle can reverse the gauge display (drain / fill)
	if (ReverseDirection.get())
		value = kGaugeScale - value;

	return value;
}

void OnServerTick()
{
	LastKnownServerTick = GetTickCount64();
	ZealService::get_instance()->pipe->chat_msg(TICK_MESSAGE, 0); // Send 'Tick' to ZealPipe
}

void __cdecl Handle_OP_Stamina(Zeal::Packets::Stamina_Struct* packet)
{
	Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();

	// Fatigue packet is the first thing sent on a Server Tick. If Food/Water are unchanged, then we know this is sent via Server Tick.
	if (char_info && char_info->Hunger == packet->food && char_info->Thirst == packet->water)
		OnServerTick();

	ZealService::get_instance()->hooks->hook_map["Handle_OP_Stamina"]->original(Handle_OP_Stamina)(packet);
}

Tick::Tick(ZealService* zeal)
{
	zeal->hooks->Add("Handle_OP_Stamina", 0x4E47A2, Handle_OP_Stamina, hook_type_detour);
	zeal->callbacks->AddGeneric([]() { LastKnownServerTick = 0; }, callback_type::Zone);

	zeal->commands_hook->Add("/tickreverse", {}, "Swaps the direction of the tick progress.", [this](std::vector<std::string>& args) {
		ReverseDirection.toggle();
		Zeal::EqGame::print_chat("Server Tick direction swapped.");
		return true;
	});
}

Tick::~Tick()
{
}