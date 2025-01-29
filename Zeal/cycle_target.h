#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqStructures.h"
#include "ZealSettings.h"
class CycleTarget
{
public:
	CycleTarget(class ZealService* zeal);
	~CycleTarget();
	Zeal::EqStructures::Entity* get_next_ent(float dist, BYTE type);
	Zeal::EqStructures::Entity* get_nearest_ent(float dist, BYTE type);

	// Assist on/off placed here since commands.cpp can't use settings and it is target related.
	ZealSetting<bool> setting_assist_on = { false, "Zeal", "AssistOn", true };

	ZealSetting<bool> setting_use_zeal_assist_on = { false, "Zeal", "UseZealAssistOn", true,
		[this](bool val) {if (val) Zeal::EqGame::set_attack_on_assist(setting_assist_on.get()); },
		true };
private:
	hook* hook;
};
