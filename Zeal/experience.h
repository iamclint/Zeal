#pragma once
#include <string>
#include <deque>
#include <Windows.h>
struct _ExpData
{
	ULONGLONG TimeStamp;
	ULONGLONG Duration;
	int Gained;
	_ExpData(int _gained, ULONGLONG LastTimeStamp)
	{
		TimeStamp = GetTickCount64();
		Gained = _gained;
		Duration = TimeStamp - LastTimeStamp;
	}
};
class Experience
{
public:
	void callback_main();
	Experience(class ZealService* zeal);
	~Experience();
	int exp;
	static constexpr float max_exp = 330.f;
	float exp_per_hour_tot;
	float exp_per_hour_pct_tot;
	std::string ttl;
	std::deque<_ExpData> ExpInfo;
};

