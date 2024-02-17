#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>

struct  ZealCommand
{
	std::vector<std::string> aliases;
	std::function<bool(std::vector<std::string>& args)> callback;
	ZealCommand(std::vector<std::string> _aliases, std::function<bool(std::vector<std::string>& args)> _callback)
	{
		callback = _callback;
		aliases = _aliases;
	}
	ZealCommand() {};
};

class ChatCommands
{
public:
	ChatCommands(class ZealService* zeal);
	~ChatCommands();
	void add(std::string cmd, std::vector<std::string> aliases, std::function<bool(std::vector<std::string>& args)> callback);
	std::unordered_map<std::string, ZealCommand> CommandFunctions;
};


