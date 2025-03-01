#include "survey.h"

#include "hook_wrapper.h"
#include "string_util.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "ui_inputdialog.h"

namespace {
static constexpr int kRaidTextChannel = 15;  // For chan_num in ChannelMessage_Struct.
static constexpr char kZealSurveyHeader[] = "ZEAL_SURVEY";
static constexpr char kZealResponseHeader[] = "ZEAL_RESPONSE";
static constexpr char kDelimiter[] = " | ";
static constexpr char kZealSurveyPrefix[] = "survey";

// Intercepts incoming channel messages from the server.
static void msg_new_text(char* msg_data)
{
	if (!Zeal::EqGame::get_self())
		return;   // Self is null, drop text to avoid potential crashes.

	ZealService* zeal = ZealService::get_instance();
	if (msg_data && zeal->survey) {
		auto msg = reinterpret_cast<Zeal::Packets::ChannelMessage_Struct*>(msg_data);
		if (msg->chan_num == kRaidTextChannel)
			zeal->survey->check_message_for_survey_trigger(msg->message);
	}

	zeal->hooks->hook_map["MsgNewText"]->original(msg_new_text)(msg_data);
}

// This replaces the PrintChat call used by incoming chat channel messages.
static void __fastcall chatPrintChat(int t, int unused, const char* data, short color_index, bool u)
{
	ZealService* zeal = ZealService::get_instance();
	if (data && zeal->survey && zeal->survey->check_for_response_message(data, color_index))
		return;

	ZealService::get_instance()->hooks->hook_map["chatPrintChat"]->original(chatPrintChat)(t, unused, data, color_index, u);
}

// Utility function for pulling out an single quoted string within a string.
std::string extract_quoted_string(const std::string& input)
{
	size_t start = input.find('\'');
	if (start == std::string::npos)
		return "";  // No opening quote found.
	start++;
	size_t end = input.find('\'', start);
	if (end == std::string::npos)
		return "";  // No closing quote.
	return input.substr(start, end - start);
}

} // namespace

Survey::Survey(ZealService* zeal)
{
	// Hook incoming text messages (raid, chat channels) to intercept survey messages.
	zeal->hooks->Add("MsgNewText", 0x004e25a1, msg_new_text, hook_type_detour);
	zeal->hooks->Add("chatPrintChat", 0x00524ca2, chatPrintChat, hook_type_replace_call);

	zeal->commands_hook->Add("/survey", {}, "Handles player surveys",
		[this](const std::vector<std::string>& args) { return parse_command(args); });

}

// Scans for a specifically formatted message that triggers start of new survey.
void Survey::check_message_for_survey_trigger(const char* message)
{
	if (!setting_enable.get() || !message || !strstr(message, kZealSurveyHeader))
		return;

	std::string message_str = message;
	if (!message_str.starts_with(kZealSurveyHeader))
		return;

	auto split = Zeal::String::split_text(message_str, kDelimiter);
	if (split.size() != 3)
		return;

	if (split[0] != kZealSurveyHeader)
		return;

	handle_start_of_survey(split[1], split[2]);
}

void Survey::handle_start_of_survey(const std::string& channel, const std::string& question)
{
	survey_responses.clear();
	survey_channel = channel;
	survey_question = question;

	if (!survey_channel.starts_with(kZealSurveyPrefix) || survey_question.size() == 0)
		return;

	// Join the channel to send and receive responses.
	if (Zeal::EqGame::get_channel_number(survey_channel.c_str()) < 0)
		Zeal::EqGame::do_join(Zeal::EqGame::get_self(), survey_channel.c_str());

	if (!ZealService::get_instance()->ui || !ZealService::get_instance()->ui->inputDialog)
		return;

	// Close / abort any open dialog and then open up a new one with the survey.
	ZealService::get_instance()->ui->inputDialog->hide();
	ZealService::get_instance()->ui->inputDialog->show("Zeal Survey", survey_question, "Yes", "No",
		[this](std::string response) { send_response("Yes"); },
		[this](std::string response) { send_response("No"); }, false);
}

// Scans for specifically formatted result replies on the response channel to accumulate results.
bool Survey::check_for_response_message(const char* message, int color_index)
{
	if (!setting_enable.get() || survey_channel.empty())
		return false;

	// Only scan the expected response channel (if not joined, channel will be -1).
	int channel = Zeal::EqGame::get_channel_number(survey_channel.c_str());
	if (channel < 0 || (
		(USERCOLOR_CHAT_1 + channel) != color_index &&
		(USERCOLOR_ECHO_CHAT_1 + channel) != color_index))
		return false;

	std::string message_str = message;
	size_t first_space = message_str.find(' ');  // Find first space.
	if (first_space == std::string::npos)
		return true;  // No space found. Unknown format, just drop messages on this channel.
	std::string name = message_str.substr(0, first_space);
	if (name == "You" && Zeal::EqGame::get_self())
		name = Zeal::EqGame::get_self()->Name;
	std::string response = extract_quoted_string(message_str);
	auto split = Zeal::String::split_text(response, kDelimiter);
	if (!name.empty() && split.size() == 2 && split[0] == kZealResponseHeader) {
		// Allow changing of votes (and prevent double-voting) by removing any existing votes.
		for (auto& pair : survey_responses) {
			auto it = std::find(pair.second.begin(), pair.second.end(), name);
			if (it != pair.second.end())
				pair.second.erase(it);  // Future: Maybe log the change in vote.
		}
		auto it = survey_responses.find(split[1]);
		if (it == survey_responses.end())
			survey_responses.insert({ split[1], std::vector<std::string>() });
		survey_responses[split[1]].push_back(name);
	}

	return true;
}

// Broadcasts the start of survey trigger with the response channel and question.
void Survey::send_survey(const std::string& channel, const std::string& question)
{
	survey_responses.clear();  // Reset old results.

	// Redundant check that response channel name has the proper prefix.
	if (!channel.starts_with(kZealSurveyPrefix)) {
		Zeal::EqGame::print_chat("Response channel name must start with survey (like 'survey123')");
		return;
	}

	if (question.empty()) {
		Zeal::EqGame::print_chat("Survey question must not be empty");
		return;
	}

	if (!Zeal::EqGame::RaidInfo->is_in_raid()) {
		Zeal::EqGame::print_chat("Must be in a raid to start a survey poll");
		return;
	}

	if (!setting_enable.get()) {
		Zeal::EqGame::print_chat("Enabling /survey to support sending one");
		setting_enable.set(true);
	}

	std::string message = std::format("{0}{1}{2}{3}{4}", kZealSurveyHeader, kDelimiter,
		channel, kDelimiter, question);
	Zeal::EqGame::send_raid_chat(message.c_str());

	// The originator of the survey does not receive the raid broadcast back, so these need to
	// be initialized here.
	handle_start_of_survey(channel, question);
}

// Sends the formatted response back to the response channel.
void Survey::send_response(const std::string& response)
{
	int channel = Zeal::EqGame::get_channel_number(survey_channel.c_str());
	if (channel < 0) {
		Zeal::EqGame::print_chat("Unable to send response to unjoined chat channel %s",
			survey_channel.c_str());
		return;
	}

	std::string message = std::format("{0}{1}{2}", kZealResponseHeader, kDelimiter, response);
	Zeal::EqGame::send_to_channel(channel, message.c_str());
	Zeal::EqGame::print_chat("You responded '%s' to %s", response.c_str(), survey_channel.c_str());
}

// Report the accumulated results of the latest survey.
void Survey::print_results(bool send_to_raidsay) const
{
	if (survey_question.empty() || survey_channel.empty()) {
		Zeal::EqGame::print_chat("No active survey");
		return;
	}

	Zeal::EqGame::print_chat("Results for question: %s", survey_question.c_str());
	if (survey_responses.empty()) {
		Zeal::EqGame::print_chat("Zero responses");
		return;
	}

	// Report the counts (and initial names) of unique responses.
	for (const auto& pair : survey_responses) {
		int count = pair.second.size();
		if (!count)
			continue;  // Answer could have been changed / erased, so skip.
		std::string names;
		int num_names = min(25, pair.second.size());  // Limit # of names for now (buffer size < 2048).
		for (int i = 0; i < num_names; ++i) {
			names += pair.second[i];
			if (i < pair.second.size() - 1)
				names += ",";
		}
		std::string message = std::format("Response: {0} = {1}", pair.first, count);
		if (send_to_raidsay)
			Zeal::EqGame::send_raid_chat(message.c_str());
		else
			Zeal::EqGame::print_chat((message + " (" + names + ")").c_str());
	}

	// Create a list of all non-responding raid members.
	std::vector<std::string> no_responses;
	auto raid_info = Zeal::EqGame::RaidInfo;
	if (!raid_info->is_in_raid())
		return;

	for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i) {
		const char* name = raid_info->MemberList[i].Name;
		if (!name[0])
			continue;
		bool found = false;
		for (const auto& pair : survey_responses) {
			if (std::find(pair.second.begin(), pair.second.end(), name) != pair.second.end())
				found = true;
		}
		if (!found)
			no_responses.push_back(name);
	}
	int num_names = min(25, no_responses.size());  // Limit # of names for now (buffer size < 2048).
	std::string names;
	for (int i = 0; i < num_names; ++i) {
		names += no_responses[i];
		if (i < no_responses.size() - 1)
			names += ",";
	}

	std::string message = no_responses.empty() ? std::string("Everyone has responded") :
		std::string("Not yet responded: ") + std::to_string(no_responses.size());
	if (send_to_raidsay)
		Zeal::EqGame::send_raid_chat(message.c_str());
	else if (no_responses.empty())
		Zeal::EqGame::print_chat(message.c_str());
	else
		Zeal::EqGame::print_chat((message + " (" + names + ")").c_str());
}

bool Survey::parse_command(const std::vector<std::string>& args)
{
	if (args.size() > 2 && args[1] == "new") {
		std::string question = std::string();  // Need to stitch question with spaces back together.
		for (size_t i = 2; i < args.size(); ++i) {
			question += args[i];
			if (i < args.size() - 1)
				question += " ";
		}
		send_survey(setting_response_channel.get(), question);
	}
	else if (args.size() == 3 && args[1] == "channel") {
		if (!args[2].starts_with(kZealSurveyPrefix))
			Zeal::EqGame::print_chat("Response channel name must start with survey (like 'survey123')");
		else {
			setting_response_channel.set(args[2]);
			Zeal::EqGame::print_chat("Response channel name set to %s",
				setting_response_channel.get().c_str());
		}
	}
	else if (args.size() > 2 && args[1] == "response") {
		std::string response = std::string();  // Need to stitch response with spaces back together.
		for (size_t i = 2; i < args.size(); ++i) {
			response += args[i];
			if (i < args.size() - 1)
				response += " ";
		}
		send_response(response);
		ZealService::get_instance()->ui->inputDialog->hide();  // Close if open.
	}
	else if (args.size() == 2 && args[1] == "results") {
		print_results(false);
	}
	else if (args.size() == 2 && args[1] == "share") {
		print_results(true);
	}
	else if (args.size() == 2 && args[1] == "on") {
		setting_enable.set(true);
		Zeal::EqGame::print_chat("/survey is enabled");
	}
	else if (args.size() == 2 && args[1] == "off") {
		setting_enable.set(false);
		survey_responses.clear();
		survey_question = "";
		Zeal::EqGame::print_chat("/survey is disabled");
	}
	else {
		Zeal::EqGame::print_chat("Usage: /survey on, /survey off");
		Zeal::EqGame::print_chat("Usage: /survey channel <response_channel_name>");
		Zeal::EqGame::print_chat("Usage: /survey new <question>");
		Zeal::EqGame::print_chat("Usage: /survey response <answer>");
		Zeal::EqGame::print_chat("Usage: /survey results (local chat), /survey share (to raid)");
	}
	return true;
}
