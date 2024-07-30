#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>

struct  ZealCommand
{
	std::vector<std::string> aliases;
	std::string description;
	std::function<bool(std::vector<std::string>& args)> callback;
	ZealCommand(std::vector<std::string> _aliases, std::string _description, std::function<bool(std::vector<std::string>& args)> _callback)
	{
		callback = _callback;
		aliases = _aliases;
		description = _description;
	}
	ZealCommand() {};
};

class ChatCommands
{
public:
	void print_commands();
	ChatCommands(class ZealService* zeal);
	~ChatCommands();
	void Add(std::string cmd, std::vector<std::string> aliases, std::string description, std::function<bool(std::vector<std::string>& args)> callback);
	std::unordered_map<std::string, ZealCommand> CommandFunctions;
};


