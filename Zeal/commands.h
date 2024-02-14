#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>

class Commands
{
public:
	Commands(class ZealService* zeal);
	~Commands();
	void add(std::string cmd, std::vector<std::string> aliases, std::function<bool(std::vector<std::string>& args)> callback);
	std::unordered_map<std::string, std::function<bool(std::vector<std::string>& args)>*> CommandAliases;
	std::unordered_map<std::string, std::function<bool(std::vector<std::string>& args)>> CommandFunctions;
private:
	hook* hook;
};


