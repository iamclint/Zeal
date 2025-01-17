#include "music.h"
#include "Zeal.h"
#include <random>


static int g_LastMusicStop = 0;
static int g_curMusicTrack = 2;
static int g_curGlobalMusicTrack = 0;
static bool g_isWaterPlaying = false;
static int g_prevGlobalMusicTrack = 0;


int __fastcall MusicManager_Set(int pthis, int unused, int musicIdx, int unknown1, int trackIdx, int volume, int unknown, int timeoutDelay, int timeInDelay, int range /* ? */, int bIsMp3)
{
	auto mm_set = ZealService::get_instance()->hooks->hook_map["MusicManager_Set"]->original(MusicManager_Set);
	if (musicIdx == 2 && ZealService::get_instance()->music->ClassicMusic.get())
	{
		mm_set(pthis, unused, 2500, unknown1, 0, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2501, unknown1, 1, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2502, unknown1, 2, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2503, unknown1, 3, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2504, unknown1, 4, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2505, unknown1, 5, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2506, unknown1, 6, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2507, unknown1, 7, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2508, unknown1, 8, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2509, unknown1, 9, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2510, unknown1, 10, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2511, unknown1, 11, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2512, unknown1, 12, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2513, unknown1, 13, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2514, unknown1, 14, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2515, unknown1, 15, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2516, unknown1, 16, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2517, unknown1, 17, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2518, unknown1, 18, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2519, unknown1, 19, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2520, unknown1, 20, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2521, unknown1, 21, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
		mm_set(pthis, unused, 2522, unknown1, 22, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);
	}

	return mm_set(pthis, unused, musicIdx, unknown1, trackIdx, volume, unknown, timeoutDelay, timeInDelay, range, bIsMp3);

}
int __fastcall MusicManager_Play(int pthis, int unused, int trackIdx, int bStartStop)
{

	if (ZealService::get_instance()->music->ClassicMusic.get())
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(1, 3);
		auto tickCount = GetTickCount();

		if (trackIdx == 2)
		{
			if (g_LastMusicStop == 0)
			{
				g_LastMusicStop = tickCount;
			}

			if (bStartStop == 1)
			{
				if (tickCount >= g_LastMusicStop + 10000)
				{
					switch (dist(mt))
					{
					case 1:
					{
						g_curMusicTrack = 2501;
						break;
					}
					case 2:
					{
						g_curMusicTrack = 2502;
						break;
					}
					case 3:
					default:
					{
						g_curMusicTrack = 2500;
						break;
					}
					}
				}
				trackIdx = g_curMusicTrack;
			}
			else if (bStartStop == 0)
			{
				trackIdx = g_curMusicTrack;
				g_LastMusicStop = GetTickCount();
			}
		}
	}
	if (bStartStop == 1)
	{
		g_curGlobalMusicTrack = trackIdx;
	}
	else
	{
		g_curGlobalMusicTrack = 0;
	}
	return ZealService::get_instance()->hooks->hook_map["MusicManager_Play"]->original(MusicManager_Play)(pthis, unused, trackIdx, bStartStop);
}

int  __fastcall MusicManager_WavPlay(int pthis, int unused, int wavIdx, int soundControl)
{
	auto play = ZealService::get_instance()->hooks->hook_map["MusicManager_Play"]->original(MusicManager_Play); 
	if (ZealService::get_instance()->music->ClassicMusic.get())
	{
		if (wavIdx == 100 && !g_isWaterPlaying)
		{
			if (g_curGlobalMusicTrack != 0 && g_curGlobalMusicTrack != 2509)
			{
				g_prevGlobalMusicTrack = g_curGlobalMusicTrack;
				play(pthis, unused, g_curGlobalMusicTrack, 0);
			}
			play(pthis, unused, 2508, 1);
			g_isWaterPlaying = true;
		}
		else if (wavIdx == 99 && g_isWaterPlaying)
		{
			int prevTrack = g_prevGlobalMusicTrack;
			play(pthis, unused, 2508, 0);
			if (g_prevGlobalMusicTrack != 0)
				play(pthis,unused, g_prevGlobalMusicTrack, 1);
			g_isWaterPlaying = false;
		}
	}

	return ZealService::get_instance()->hooks->hook_map["MusicManager_WavPlay"]->original(MusicManager_WavPlay)(pthis, unused, wavIdx, soundControl);
}


MusicManager::MusicManager(ZealService* zeal)
{
	
	//zeal->hooks->Add("MusicManager_WavPlay", 0x4D518B, MusicManager_Play, hook_type_detour); //this is commented out in eqgame.dll as well, added it in case it does something
	zeal->hooks->Add("MusicManager_Play", 0x4D54C1, MusicManager_Play, hook_type_detour);
	zeal->hooks->Add("MusicManager_Set", 0x550AF8, MusicManager_Set, hook_type_detour);
}

MusicManager::~MusicManager()
{

}