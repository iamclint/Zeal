#include "experience.h"
#include "Zeal.h"


std::string format_duration(std::chrono::milliseconds ms) {
	using namespace std::chrono;
	auto secs = duration_cast<seconds>(ms);
	ms -= duration_cast<milliseconds>(secs);
	auto mins = duration_cast<minutes>(secs);
	secs -= duration_cast<seconds>(mins);
	auto hour = duration_cast<hours>(mins);
	mins -= duration_cast<minutes>(hour);

	std::stringstream ss;
	ss << hour.count() << ":" << mins.count() << ":" << secs.count();
	return ss.str();
}

void Experience::check_reset()
{
	Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
	static int zone_id = self->ZoneId;
	static int prev_level = 0;
	if (exp_per_hour_pct_tot < 0)
		ExpInfo.clear();

	if (self->Level != prev_level) //level up or delevel
		ExpInfo.clear();

	if (zone_id != self->ZoneId) //zoned
		ExpInfo.clear();

	prev_level = self->Level;
	zone_id = self->ZoneId;
}

void Experience::callback_main()
{
	Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
	if (!self || !Zeal::EqGame::is_in_game())
		return;
	if (!self->CharInfo)
		return;

	check_reset();
	exp_per_hour_tot = 0;
	exp_per_hour_pct_tot = 0;
	int total_gained = 0;
	ULONGLONG total_duration = 0;
	for (auto& e : ExpInfo)
	{
		total_gained += e.Gained;
		total_duration += e.Duration;
	}
	if (ExpInfo.size())
		total_duration += GetTickCount64() - ExpInfo.back().TimeStamp;

	if (total_gained && total_duration)
	{
		exp_per_hour_tot = ((((float)total_gained / (float)total_duration) * 1000) * 60) * 60;
		exp_per_hour_pct_tot = (exp_per_hour_tot / max_exp) * 100;
	}
	std::chrono::milliseconds ms_to_level = std::chrono::milliseconds(0);
	if (exp_per_hour_tot)
	{
		float experience_needed = max_exp - self->CharInfo->Experience;
		float total_hour_to_level = experience_needed / exp_per_hour_tot;
		float total_minutes_to_level = total_hour_to_level * 60;
		float total_seconds_to_level = total_minutes_to_level * 60;
		int toal_ms_to_level = (int)(total_seconds_to_level * 1000);
		ms_to_level = std::chrono::milliseconds(toal_ms_to_level);
	}
	ttl = format_duration(ms_to_level);

	if (exp != self->CharInfo->Experience && exp != 0)
	{
		int gained = self->CharInfo->Experience - exp;
		ULONGLONG last_stamp = GetTickCount64();
		if (ExpInfo.size())
			last_stamp = ExpInfo.back().TimeStamp;
		ExpInfo.push_back(_ExpData(gained, last_stamp));
		if (ExpInfo.size() > 20)
			ExpInfo.pop_front();

		//float gained_pct = ((float)gained / max_exp) * 100.f;
		//Zeal::EqGame::print_chat("percent gained: %.2f%%  actual value: %i", gained_pct, gained);
	}
	exp = self->CharInfo->Experience;
}


Experience::~Experience()
{

}
Experience::Experience(ZealService* zeal)
{
	exp = 0;
	exp_per_hour_pct_tot = 0;
	exp_per_hour_tot = 0;
	zeal->callbacks->add_callback([this]() { callback_main();  });
}