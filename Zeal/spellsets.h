#pragma once
#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "IO_ini.h"

class SpellSets
{
public:
	void save(const std::string& name);
	void load(const std::string& name);
	std::shared_ptr<IO_ini> ini = nullptr;
	void finished_memorizing(int a1, int a2);
	SpellSets(class ZealService* zeal);
	~SpellSets();
private:
	Stance original_stance;
	void callback_main();
	std::vector<std::pair<int, int>> mem_buffer;
};



