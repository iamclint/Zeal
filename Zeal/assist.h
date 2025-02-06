#pragma once
#include "EqStructures.h"
#include "ZealSettings.h"

// Enhancements for the /assist command. Separate class since commands.cpp can't use settings.
class Assist
{
public:
	Assist(class ZealService* zeal);
	~Assist();

	// Assist on/off per character support.
	ZealSetting<bool> setting_assist_on = { false, "Zeal", "AssistOn", true };
	ZealSetting<bool> setting_use_zeal_assist_on = { false, "Zeal", "UseZealAssistOn", true,
		[this](bool val) {if (val) Zeal::EqGame::set_attack_on_assist(setting_assist_on.get()); },
		true };

	// Support for detecting / reporting that /assist failed to update to a new target.
	ZealSetting<bool> setting_detect_assist_failure = { false, "Zeal", "DetectAssistFailure", true };

protected:
	bool handle_assist_command(const std::vector<std::string>& args);
	bool handle_assist_response(const Zeal::Packets::EntityId_Struct* packet);
};

