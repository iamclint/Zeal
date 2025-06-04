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
	SpellSets(class ZealService* zeal);
	~SpellSets();

	// Client event hook callbacks.
	void handle_finished_memorizing(int a1, int a2);
	void handle_finished_scribing(int a1, int a2);
	void handle_spell_gem_rbutton_up(Zeal::EqUI::SpellGemWnd* gem, Zeal::EqUI::CXPoint pt);
	void handle_spell_book_rbutton_up(Zeal::EqUI::CXPoint pt) const;

	// Custom context menu callbacks.
	void handle_spell_gem_menu_notification(int msg_data);
	void handle_spellset_menu_notification(int msg_data);

private:
	struct MenuPair {
		int index;
		Zeal::EqUI::ContextMenu* menu;
	};

	// File system storage of spell sets.
	void initialize_ini_filename();
	void save(const std::string& name);
	void load(const std::string& name);
	void remove(const std::string& name);

	std::map<std::string, std::map<std::string, std::list<menudata>>> get_spell_categories();
	void create_spells_menus();
	void create_spellsets_menus();
	void destroy_menus(std::vector<SpellSets::MenuPair>& menus);
	int add_menu_to_manager(std::vector<SpellSets::MenuPair>& menus, Zeal::EqUI::ContextMenu* new_menu);
	bool check_caster_level(int spell_id) const;
	void memorize_spell(int book_index, int gem_index);

	void callback_clean_ui();
	void callback_main();
	void callback_init_ui();

	std::vector<MenuPair> spells_menus;  // Spellbook spells.
	Zeal::EqUI::SpellGemWnd* last_gem_clicked = 0;  // Caches clicked gem between operations.
	Stance original_stance = Stance::Stand;  // Preserves starting stance across operations.

	std::vector<MenuPair> spellsets_menus;  // Spellsets.
	std::map<int, std::string> spellsets_map; // Links menu IDs to spellset names.
	std::vector<std::pair<int, int>> mem_buffer;  // In-progress list of spells to memorize.
	IO_ini ini = IO_ini(".\\spellsets.ini");  // Filename updated later to per character.
};



