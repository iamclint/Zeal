#pragma once

#include "ZealSettings.h"

// Implements a simple /survey interface for polling a raid group.
class Survey {
public:
	Survey(class ZealService* zeal_service);

	// Should be used by callbacks only.
	void check_message_for_survey_trigger(const char* message);
	bool check_for_response_message(const char* message, int color_index);

	ZealSetting<bool> setting_enable = { false, "Zeal", "SurveyEnable", false };
	ZealSetting<std::string> setting_response_channel = { "", "Zeal", "SurveyResponseChannel", false };

protected:

	void send_survey(const std::string& channel, const std::string& question);
	void handle_start_of_survey(const std::string& channel, const std::string& question);
	void send_response(const std::string& response);
	void print_results(bool send_to_raidsay = false) const;
	bool parse_command(const std::vector<std::string>& args);

	std::string survey_channel;  // The reponse chat channel name for the most recent survey.
	std::string survey_question;  // The most recent survey question.
	std::unordered_map<std::string, std::vector<std::string>> survey_responses;  // Stored responses.
};
