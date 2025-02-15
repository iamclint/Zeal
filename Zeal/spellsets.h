#pragma once
#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "IO_ini.h"
#include "EqFunctions.h"

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
	void remove(const std::string& name);
	void remove_selected();//from ui
	void set_ini();
	std::shared_ptr<IO_ini> ini = nullptr;
	void finished_memorizing(int a1, int a2);
	void finished_scribing(int a1, int a2);
	void create_context_menus(bool force = false);
	int add_menu_to_manager(Zeal::EqUI::ContextMenu* new_menu);
	void destroy_context_menus();
	bool check_caster_level(int spell_id) const;

	std::string ui_selected_name;
	Zeal::EqUI::ContextMenu* menu;
	Zeal::EqUI::ContextMenu* spellset_menu;
	int SpellMenuIndex = 0;
	int SpellSetMenuIndex = 0;
	std::map<std::string, std::map<std::string, std::list<menudata>>>  SpellCategory;
	std::map<int, Zeal::EqUI::ContextMenu*> MenuMap;
	std::map<int, std::string> spellset_map;
	std::vector<std::string> spellsets;
	Zeal::EqUI::SpellGemWnd* last_gem_clicked=0;
	std::vector<std::pair<int, int>> mem_buffer;
	void handle_menu_mem(int book_index, int gem_index);
	SpellSets(class ZealService* zeal);
	~SpellSets();
private:
	Stance original_stance;
	void CleanUI();
	void callback_main();
	//void callback_characterselect();
};



