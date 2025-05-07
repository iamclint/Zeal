#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class EQStr
{
public:
	std::unordered_map<int, const char*> str_replacements;
	std::unordered_map<int, bool> str_noprint;
	EQStr(class ZealService* zeal);
	~EQStr();
private:
};
