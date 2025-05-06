#pragma once
#include "json.hpp"
#include <Windows.h>
#include <string>
#include <mutex>
#include <thread>
#include "ZealSettings.h"
enum struct pipe_data_type
{
	log,
	label,
	gauge,
	player,
	custom,
	raid,
	group
};
struct pipe_data
{
	pipe_data_type type;
	std::string data;
	std::string character;
	pipe_data(pipe_data_type _type, std::string _data);
	pipe_data() : type(pipe_data_type::custom), data{ "" }, character{ "" } {};
	nlohmann::json serialize() const
	{
		return nlohmann::json{ {"type", type}, {"data_len", data.length()}, {"data", data}, {"character", character}};
	}
	void deserialize(nlohmann::json json_obj)
	{
		type = json_obj["type"];
		data = json_obj["data"];
	}
};
class named_pipe
{
public:
	named_pipe(class ZealService* zeal);
	~named_pipe();
	void chat_msg(const char* data, int color_index);
	void write(std::string data, pipe_data_type data_type);
	void write(std::string data);
	void write(const char* format, ...);
	void main_loop();
	void update_delay(unsigned new_delay);
	bool is_connected() const { return pipe_handles.size() != 0; }
private:
	void add_new_pipe_handle(const HANDLE& handle);
	void update_pipe_handles();
	ZealSetting<int> pipe_delay = { 100, "Zeal", "PipeDelay", false };
	bool end_thread = false;
	std::string name = "\\\\.\\pipe\\zeal_";
	std::vector<HANDLE> pipe_handles;
	std::vector<HANDLE> pipe_handle_queue;  // Mutex protected transfer queue.
	std::thread pipe_thread;
	std::mutex pipe_handle_mutex;
};

