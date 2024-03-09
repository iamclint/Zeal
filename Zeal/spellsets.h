#pragma once
#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "IO_ini.h"

struct menudata
{
	std::string Name;
	std::string Category;
	std::string SubCategory;
	DWORD ID;
};

class SpellSets
{
public:
	void save(const std::string& name);
	void load(const std::string& name);
	std::shared_ptr<IO_ini> ini = nullptr;
	void finished_memorizing(int a1, int a2);
	void create_context_menus();
	Zeal::EqUI::ContextMenu* menu;
	Zeal::EqUI::ContextMenu* spellset_menu;
	int SpellMenuIndex = 0;
	int SpellSetMenuIndex = 0;
	std::unordered_map<std::string, std::unordered_map<std::string, std::list<menudata>>>  SpellCategory;
	std::unordered_map<int, Zeal::EqUI::ContextMenu*> MenuMap;
	Zeal::EqUI::SpellGem* last_gem_clicked=0;
	std::vector<std::pair<int, int>> mem_buffer;
	void handle_menu_mem(int book_index, int gem_index);
	SpellSets(class ZealService* zeal);
	~SpellSets();
private:
	Stance original_stance;
	void callback_main();
};



