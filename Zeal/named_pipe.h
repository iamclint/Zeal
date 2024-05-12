#pragma once
#include "json.hpp"
#include <Windows.h>
#include <string>
#include <thread>
enum struct pipe_data_type
{
	log,
	label
};
struct pipe_data
{
	pipe_data_type type;
	UINT data_len;
	std::string data;
	nlohmann::json serialize()
	{
		return nlohmann::json{ {"type", type}, {"data_len", data_len}, {"data", data} };
	}
};
class named_pipe
{
public:
	named_pipe(class ZealService* zeal);
	~named_pipe();
	void write(std::string data);
	void write(const char* format, ...);
private:
	bool end_thread = false;
	const char* name = "\\\\.\\pipe\\zeal";
	std::vector<HANDLE> pipe_handles;
	std::thread pipe_thread;
};

