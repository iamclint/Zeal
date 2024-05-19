#pragma once
#include "json.hpp"
#include <Windows.h>
#include <string>
#include <thread>
enum struct pipe_data_type
{
	log,
	label,
	gauge,
	player,
	custom
};
struct pipe_data
{
	pipe_data_type type;
	UINT data_len;
	std::string data;
	std::string character;
	pipe_data(pipe_data_type _type, std::string _data);
	pipe_data() : data{ "" }, character{ "" } {};
	nlohmann::json serialize() const
	{
		return nlohmann::json{ {"type", type}, {"data_len", data_len}, {"data", data}, {"character", character} };
	}
	void deserialize(nlohmann::json json_obj)
	{
		type = json_obj["type"];
		data_len = json_obj["data_len"];
		data = json_obj["data"];
	}
};
class named_pipe
{
public:
	named_pipe(class ZealService* zeal, class IO_ini* ini);
	~named_pipe();
	void chat_msg(const char* data, int color_index);
	void write(std::string data, pipe_data_type data_type);
	void write(std::string data);
	void write(const char* format, ...);
	void main_loop();
	void update_delay(unsigned new_delay);
private:
	int pipe_delay=500;
	bool end_thread = false;
	std::string name = "\\\\.\\pipe\\zeal_";
	std::vector<HANDLE> pipe_handles;
	std::thread pipe_thread;
};

