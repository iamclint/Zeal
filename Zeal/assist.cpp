#include "assist.h"
#include "Zeal.h"
#include "EqFunctions.h"
#include "string_util.h"


Assist::Assist(ZealService* zeal)
{
	zeal->commands_hook->Add("/assist", {}, "Supports optional per character settings for /assist on/off.",
		[this](std::vector<std::string>& args) { return handle_assist_command(args); });

	zeal->callbacks->AddPacket([this](UINT opcode, char* buffer, UINT len) {
		if (opcode == Zeal::Packets::Assist && len == sizeof(Zeal::Packets::EntityId_Struct))
			return handle_assist_response(reinterpret_cast<Zeal::Packets::EntityId_Struct*>(buffer));
		return false; // continue processing
		}, callback_type::WorldMessage);
}

Assist::~Assist()
{
}

bool Assist::handle_assist_command(const std::vector<std::string>& args)
{
	if (!setting_use_zeal_assist_on.get() && !setting_detect_assist_failure.get())
		return false;  // Disabled, use /normal assist.

	// If no target param, either there's a target and the OP_Assist will be sent or the client's
	// /assist handler will print out a usage format message.
	if (args.size() == 1)
		return false;

	// The default client ignores parameters beyond args[1], so we'll do the same.
	bool turn_on = Zeal::String::compare_insensitive(args[1], "on");
	bool turn_off = !turn_on && Zeal::String::compare_insensitive(args[1], "off");
	if (turn_on || turn_off) {
		if (setting_use_zeal_assist_on.get()) {
			setting_assist_on.set(turn_on);
			Zeal::EqGame::set_attack_on_assist(turn_on);
			Zeal::EqGame::print_chat("Per character attack on assist: %s",
				turn_on ? "ON" : "OFF");
			return true;  // Skip normal /assist processing.
		}
		return false;  // Let normal /assist handle the on or off.
	}

	if (setting_detect_assist_failure.get()) {
		// Duplicate the /assist command code to check that an OP_Assist message will go out that
		// can be monitored for success.
		std::string name = args[1];
		Zeal::EqGame::DoPercentConvert(name);
		if (!Zeal::EqGame::get_player_partial_name(name.c_str())) {
			Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Assistee %s not found!", name.c_str());
			Zeal::EqGame::set_target(nullptr);  // Will emit message if had a target.
			return true;
		}
	}

	return false;  // Let normal /assist process the args.
}

bool Assist::handle_assist_response(const Zeal::Packets::EntityId_Struct* packet) {
	// If the assist request failed, the response will contain either -1 or self->SpawnId depending on
	// RuleB(Combat, AssistNoTargetSelf). The latter is difficult to be 100% sure if it is a failure
	// or a desired result (like assisting a mob for heals that is targeting you now), and since it is
	// a server flag we just emit a warning.
	if (setting_detect_assist_failure.get() && packet) {
		if (packet->entity_id == -1) {
			Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Assist failed! (Out of range or no assistee target)");
			Zeal::EqGame::set_target(nullptr);  // Will emit message if had a target.
			return true;  // Skip downstream response handling.
		}
		else if (Zeal::EqGame::get_self() && Zeal::EqGame::get_self()->SpawnId == packet->entity_id)
			Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Assist targeting self.");  // Just warn.
	}
	return false;
}
