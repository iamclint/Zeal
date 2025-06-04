#include "spellsets.h"
#include "Zeal.h"
#include "string_util.h"
#include "EqAddresses.h"
#include "SpellCategories.h"
#include <algorithm>

// Message IDs used in callbacks from context menus.
static constexpr int kSpellsBaseMsgId = 0x10000;  // Used in single gem selection. 
static constexpr int kSpellSetSaveMsgId = 0x21000;  // Trigger to start a save.
static constexpr int kSpellSetBaseMsgId = 0x20000;  // Offset for selecting a set.
static constexpr int kSpellSetDeleteBaseMsgId = 0x22000;  // Offset for deleting.
static constexpr int kSpellSetHeaderMsgId = 0x30000;  // Header ID.

// Helper function to verify the spellset name is valid.
static bool is_valid_name(const std::string& name) {
    if (name.empty() || name.size() > 32)
    {
        Zeal::EqGame::print_chat("Invalid spellset name");
        return false;
    }
    return true;
}

// Saves currently memorized spell gems to the spellset name in the ini file.
void SpellSets::save(const std::string& name)
{
    if (!is_valid_name(name))
        return;

    initialize_ini_filename();
    Zeal::EqGame::print_chat("Saving spellset [%s]", name.c_str());
    for (size_t i = 0; i < EQ_NUM_SPELL_GEMS; i++)
        ini.setValue(name, std::to_string(i), Zeal::EqGame::get_char_info()->MemorizedSpell[i]);
    create_spellsets_menus();  // Updates context menu.
}

// Removes (deletes) the spellset from the ini file.
void SpellSets::remove(const std::string& name)
{
    initialize_ini_filename();
    Zeal::EqGame::print_chat("Removing spellset [%s]", name.c_str());
    if (!ini.deleteSection(name))
        Zeal::EqGame::print_chat("Error removing spellset [%s]", name.c_str());
    create_spellsets_menus();  // Updates context menu
}

// Loads a spellset from the ini file and initiates memorization.
void SpellSets::load(const std::string& name)
{
    bool interrupted = !mem_buffer.empty();
    mem_buffer.clear();
    Zeal::EqGame::Spells::StopSpellBookAction();  // Clears in-progress state.

    if (!is_valid_name(name))
        return;

    const auto* spell_manager = Zeal::EqGame::get_spell_mgr();
    if (!spell_manager || !Zeal::EqGame::get_char_info())
        return;

    initialize_ini_filename();
    if (!ini.exists(name, "0"))
    {
        Zeal::EqGame::print_chat("The spellset [%s] does not exist", name.c_str());
        return;
    }
    Zeal::EqGame::print_chat("Loading spellset [%s]", name.c_str());

    for (int gem_index = EQ_NUM_SPELL_GEMS - 1; gem_index >= 0; gem_index--)
    {
      short spell_id = ini.getValue<WORD>(name, std::to_string(gem_index));
      if (spell_id == -1) { // Empty slot when saved.
          continue;  // Leave it unchanged.
      }

      if (spell_id < 1 || spell_id >= EQ_NUM_SPELLS || !spell_manager->Spells[spell_id])
      {
          Zeal::EqGame::print_chat("Error loading spellset [%s]: spell id at index [%i] is [%i]",
                    name.c_str(), gem_index, spell_id);
          break;
      }

      short memmed_spell = Zeal::EqGame::get_char_info()->MemorizedSpell[gem_index];
      if (memmed_spell != spell_id)
      {
          if (memmed_spell != -1)
              Zeal::EqGame::Spells::Forget(gem_index);

          // Bail out early if too low level (skips memorization time and state issues).
          if (!check_caster_level(spell_id))
              continue;

          for (size_t book_index = 0; book_index < EQ_NUM_SPELL_BOOK_SPELLS; book_index++)
          {
              if (Zeal::EqGame::get_char_info()->SpellBook[book_index] == spell_id)
              {
                  mem_buffer.push_back({ book_index,gem_index });
                  break;
              }
          }
      }
    }
    if (mem_buffer.size())
    {
        if (!interrupted)
            original_stance = (Stance)Zeal::EqGame::get_self()->StandingState;
        Zeal::EqGame::Spells::Memorize(mem_buffer.back().first, mem_buffer.back().second);
    }
}

// Called after the client finishes the spell memorization process.
void SpellSets::handle_finished_memorizing(int a1, int a2)
{
    if (!mem_buffer.size()) 
        return;

    // Handle interruptions gracefully by clearing state and restoring stance.
    if (!Zeal::EqGame::Windows->SpellBook || !Zeal::EqGame::Windows->SpellBook->IsVisible)
    {
        if (Zeal::EqGame::get_self() && Zeal::EqGame::is_in_game() &&
                    original_stance != Stance::Sit &&
                    ((Stance)Zeal::EqGame::get_self()->StandingState == Stance::Sit))
            Zeal::EqGame::get_self()->ChangeStance(original_stance);
        mem_buffer.clear();
        return;
    }

    mem_buffer.pop_back();
    if (mem_buffer.size())
        Zeal::EqGame::Spells::Memorize(mem_buffer.back().first, mem_buffer.back().second);
    else if (Zeal::EqGame::Windows->SpellBook->IsVisible)
    {
        Zeal::EqGame::get_self()->ChangeStance(original_stance);
        Zeal::EqGame::Windows->SpellBook->show(0, false);
    }
}

// Called after scribing a new spell into spellbook.
void SpellSets::handle_finished_scribing(int a1, int a2)
{
    create_spells_menus();  // Update context menu with new spell.
}

// Helper lookup function for spell information.
static Zeal::EqStructures::SPELL* GetSpell(int id)
{
    id = Zeal::EqGame::get_char_info()->SpellBook[id];
    if (id == 0 || id >= EQ_NUM_SPELLS)
        return 0;
    return Zeal::EqGame::get_spell_mgr()->Spells[id];
}

// Memorization fails (and locks up spellbook state) if the caster level is too low.
bool SpellSets::check_caster_level(int spell_id) const {
    const auto* spell_manager = Zeal::EqGame::get_spell_mgr();
    const auto* char_info = Zeal::EqGame::get_char_info();
    if (!spell_manager || !char_info) {
        Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "SpellSet game state error. Aborting.");
        return false;
    }

    const int spell_level = spell_manager->Spells[spell_id]->ClassLevel[char_info->Class];
    if (char_info->Level < spell_level)
    {
        Zeal::EqGame::print_chat(USERCOLOR_SHOUT,
            "Stop dying. You will have to achieve level %i to memorize the spell %s",
            spell_level, spell_manager->Spells[spell_id]->Name);
        return false;
    }

    return true;
}

// Handles right click memorization of a single spell gem.
void SpellSets::memorize_spell(int book_index, int gem_index)
{
    bool interrupted = !mem_buffer.empty();
    mem_buffer.clear();  // Clear any in-progress reloads.
    Zeal::EqGame::Spells::StopSpellBookAction();

    const auto* spell_manager = Zeal::EqGame::get_spell_mgr();
    const auto* char_info = Zeal::EqGame::get_char_info();
    if (!spell_manager || !char_info)
        return;
    
    // Bail out early if too low level (skips memorization time).
    const int spell_id = char_info->SpellBook[book_index];
    if (!check_caster_level(spell_id))
        return;

    if (!interrupted)
        original_stance = (Stance)Zeal::EqGame::get_self()->StandingState;
    mem_buffer.push_back({ book_index,gem_index });
    Zeal::EqGame::Spells::Memorize(book_index, gem_index);
}

// Handles context menu callback for single gem spell selection.
void SpellSets::handle_spell_gem_menu_notification(int msg_data)
{
    if (spells_menus.empty())
        return;

    ZealService* zeal = ZealService::get_instance();
    int index = -1;
    for (int i = 0; i < 8; i++)
    {
        if (Zeal::EqGame::Windows->SpellGems->SpellSlots[i] == zeal->spell_sets->last_gem_clicked)
        {
            index = i;
            break;
        }
    }
    if (index == -1) {
        Zeal::EqGame::print_chat("Could not find gem index to update");
        return;
    }

    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    int spell_id = msg_data - kSpellsBaseMsgId;
    int book_index = -1;
    for (int i = 0; i < EQ_NUM_SPELL_BOOK_SPELLS; i++)
    {
        if (char_info->SpellBook[i] == spell_id)
        {
            book_index = i;
            break;
        }
    }
    if (book_index != -1)
        memorize_spell(book_index, index);
    else
        Zeal::EqGame::print_chat("Could not find spell [%i] in your spellbook.", spell_id);
}

// Handles context menu callback for spell set selections.
void SpellSets::handle_spellset_menu_notification(int msg_data)
{
    if (spellsets_menus.empty())
        return;

    // Handle spell set Save trigger.
    if (msg_data == kSpellSetSaveMsgId) {
        ZealService::get_instance()->ui->inputDialog->
            show("Spell Set", "Save spell set as:", "Save", "Cancel",
                [this](std::string msg) {save(msg); }, nullptr, true);
        return;
    }

    // Handle spell set Delete triggers.
    if (msg_data >= kSpellSetDeleteBaseMsgId) {
        static constexpr int delete_offset = kSpellSetDeleteBaseMsgId - kSpellSetBaseMsgId;
        ZealService::get_instance()->ui->inputDialog->
            show("Spell Set", "Delete " + spellsets_map[msg_data - delete_offset] + "?", "Yes", "No",
                [msg_data, this](std::string msg) {remove(spellsets_map[msg_data - delete_offset]); },
                nullptr, false);
        return;
    }

    // Handle spell set load triggers.
    ZealService::get_instance()->spell_sets->load(spellsets_map[msg_data]);
}

// Triggers the spell selection context menu after a right click up on a spell gem.
void SpellSets::handle_spell_gem_rbutton_up(Zeal::EqUI::SpellGemWnd* gem, Zeal::EqUI::CXPoint pt)
{
    if (spells_menus.empty() || !gem || gem->spellicon != -1)
        return;

    last_gem_clicked = gem;  // Saved for use in handle_spell_gem_menu_notification().
    const auto& pair = spells_menus.back();  // Root menu stored last.
    Zeal::EqGame::Windows->ContextMenuManager->PopupMenu(pair.index, pt, pair.menu);
}

// Triggers the spell sets context menu after a right click up on the castwnd's book button.
void SpellSets::handle_spell_book_rbutton_up(Zeal::EqUI::CXPoint pt) const
{
    if (spellsets_menus.empty())
        return;
    ZealService* zeal = ZealService::get_instance();
    const auto& pair = spellsets_menus.back();
    Zeal::EqGame::Windows->ContextMenuManager->PopupMenu(pair.index, pt, pair.menu);
}

// Releases context menu resources.
void SpellSets::destroy_menus(std::vector<SpellSets::MenuPair>& menus)
{
    if (menus.empty() || !Zeal::EqGame::Windows->ContextMenuManager)
        return;

    for (const auto& pair : menus) {
        // Only release if there's a match (should always match).
        if (pair.menu == Zeal::EqGame::Windows->ContextMenuManager->Menus[pair.index])             {
            // The manager should call the custom destructor and release the resources.
            Zeal::EqGame::Windows->ContextMenuManager->RemoveMenu(pair.index, true);
        }
    }

    menus.clear();
}

// Initializes the character dependent filename useed to store spell sets.
void SpellSets::initialize_ini_filename()
{
    std::stringstream ss;
    ss << ".\\" << Zeal::EqGame::get_char_info()->Name << "_spellsets.ini";
    ini.set(ss.str());
}

// Helper function that allocates and initializes new context menus.
static Zeal::EqUI::ContextMenu* CreateContextMenu(void* notificationFunc = nullptr) {
    auto new_menu = Zeal::EqUI::ContextMenu::Create(0, 0, { 100, 100, 100, 100 });
    if (!new_menu)
        throw std::bad_alloc();

    new_menu->HasChildren = true;  // Note: Evaluate if these are still necessary.
    new_menu->HasSiblings = true;
    new_menu->Unknown0x015 = 0;
    new_menu->Unknown0x016 = 0;
    new_menu->Unknown0x017 = 0;
    new_menu->fnTable->WndNotification = notificationFunc;
    return new_menu;
}

// Registers the menu with the ContextMenuManager and adds it to the menus.
int SpellSets::add_menu_to_manager(std::vector<SpellSets::MenuPair>& menus, Zeal::EqUI::ContextMenu* new_menu)
{
    int addedindex = Zeal::EqGame::Windows->ContextMenuManager->AddMenu(new_menu);
    menus.push_back({ addedindex, new_menu });
    return addedindex;
}

// Helper function for sorting the spells within categories.
static bool compareBySpellLevel(const Zeal::EqStructures::SPELL* a, const Zeal::EqStructures::SPELL* b) {
    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    int aLevel = a->ClassLevel[char_info->Class];
    int bLevel = b->ClassLevel[char_info->Class];
    if (aLevel != bLevel)
        return aLevel > bLevel;
    return a->Name > b->Name;
}

// Assembles all spells in the spellbook and returns a categorized structure for the menus.
std::map<std::string, std::map<std::string, std::list<menudata>>> SpellSets::get_spell_categories()
{
    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    std::vector<Zeal::EqStructures::SPELL*> spells;
    for (int N = 0; N < EQ_NUM_SPELL_BOOK_SPELLS; N++) {
        if (Zeal::EqStructures::SPELL* pSpell = GetSpell(N))
            spells.push_back(pSpell);
    }
    std::sort(spells.begin(), spells.end(), compareBySpellLevel);
    std::map<std::string, std::map<std::string, std::list<menudata>>> spell_category;
    for (auto& s : spells)
    {
        SpellCat spell_cat_data = getSpellCategoryAndSubcategory(s->ID);
        menudata md;
        md.ID = s->ID;
        int level = s->ClassLevel[char_info->Class];
        std::stringstream ss;
        if (spell_cat_data.NewName != nullptr)
            ss << level << " - " << spell_cat_data.NewName;
        else
            ss << level << " - " << s->Name;
        md.Name = ss.str();

        std::string category = GetSpellCategoryName(spell_cat_data.Category);
        std::string subcategory = GetSpellSubCategoryName(spell_cat_data.SubCategory);
        spell_category[category][subcategory].push_back(md);
    }
    return spell_category;
}

// Callback provided to context manager to handle spell gem selection notifications.
static int __fastcall SpellsMenuNotification(void* menu_this, int unused_edx,
    Zeal::EqUI::EQWND* pWnd, unsigned int Message, void* data)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->spell_sets->handle_spell_gem_menu_notification(reinterpret_cast<int>(data));
    return 1;
}

// Creates the spells_menus context menus used in spell gem selection.
void SpellSets::create_spells_menus()
{
    if (!Zeal::EqGame::is_new_ui()) { return; } // prevent callback crashing oldui

    destroy_menus(spells_menus);

    if (!Zeal::EqGame::get_self() || !Zeal::EqGame::get_char_info())
        return;

    auto spells_root_menu = CreateContextMenu(SpellsMenuNotification);

    const auto& spell_categories = get_spell_categories();
    for (auto& [cat, sub] : spell_categories)
    {
        Zeal::EqUI::ContextMenu* sub_category = CreateContextMenu();
        for (auto& [subcat, spells] : sub)
        {
            Zeal::EqUI::ContextMenu* sub_menu = CreateContextMenu();
            for (auto& sp : spells)
                sub_menu->AddMenuItem(sp.Name, kSpellsBaseMsgId + sp.ID);
            sub_category->AddMenuItem(subcat, add_menu_to_manager(spells_menus, sub_menu), false, true);
        }
        spells_root_menu->AddMenuItem(cat, add_menu_to_manager(spells_menus, sub_category), false, true);
    }
    add_menu_to_manager(spells_menus, spells_root_menu);  // Root menu is added last (at .back()).
}

// Callback provided to context manager to handle spell set selection notifications.
static int __fastcall SpellSetMenuNotification(void* menu_this, int unused_edx,
    Zeal::EqUI::EQWND* pWnd, unsigned int Message, void* data)
{
    int msg_data = (int)data;
    ZealService::get_instance()->spell_sets->handle_spellset_menu_notification(msg_data);
    return 1;
}

// Creates the spellsets_menus context menus used to handle spell set selection.
void SpellSets::create_spellsets_menus()
{
    if (!Zeal::EqGame::is_new_ui()) { return; } // prevent callback crashing oldui

    destroy_menus(spellsets_menus);

    if (!Zeal::EqGame::get_self() || !Zeal::EqGame::get_char_info())
        return;

    initialize_ini_filename();  // Retrieve the spellsets from the file system.
    std::vector<std::string>  spellsets = ini.getSectionNames();
    std::sort(spellsets.begin(), spellsets.end());
 
    auto root_menu = CreateContextMenu(SpellSetMenuNotification);
    int header_index = root_menu->AddMenuItem("Spell Sets", kSpellSetHeaderMsgId, false);
    root_menu->EnableLine(header_index, false);
    //spellset_menu->SetItemColor(header_index, { 255,255,255,255 });
    for (int i = 0; auto & s : spellsets)
    {
        root_menu->AddMenuItem(s, kSpellSetBaseMsgId + i);
        spellsets_map[kSpellSetBaseMsgId + i] = s;
        i++;
    }
    root_menu->AddSeparator();
    Zeal::EqUI::ContextMenu* DeleteSubCatMenu = CreateContextMenu();
    for (int i = 0; auto & s : spellsets)
    {
        DeleteSubCatMenu->AddMenuItem(s, kSpellSetDeleteBaseMsgId + i);
        i++;
    }
    root_menu->AddMenuItem("Delete", add_menu_to_manager(spellsets_menus, DeleteSubCatMenu), false, true);
    root_menu->AddSeparator();
    root_menu->AddMenuItem("Save New", kSpellSetSaveMsgId);
    add_menu_to_manager(spellsets_menus, root_menu);
}

// Called after client UI is initialized.
void SpellSets::callback_init_ui()
{
    create_spells_menus();  // Create up to date spells from spellbook.
    create_spellsets_menus();  // Load spellsets from ini.
}

// Called right before client UI is de-allocated.
void SpellSets::callback_clean_ui()
{
    destroy_menus(spells_menus);
    destroy_menus(spellsets_menus);
}

// Main processing loop callback to handle various reasons why memorization could be aborted.
void SpellSets::callback_main()
{
    if (mem_buffer.size() && (!Zeal::EqGame::Windows ||
        !Zeal::EqGame::Windows->SpellBook ||
        !Zeal::EqGame::Windows->SpellBook->IsVisible))
        mem_buffer.clear();  // Abort in-process memorization if spellbook drops.
}

// Hook called after the client finishes memorizing a spell.
static void __fastcall FinishMemorizing(int t, int u, int a1, int a2)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->spell_sets->handle_finished_memorizing(a1, a2);
    zeal->hooks->hook_map["FinishMemorizing"]->original(FinishMemorizing)(t, u, a1, a2);
}

// Hook called after the client finishes scribing a new spell into the spellbook.
static void __fastcall FinishScribing(int t, int u, int a1, int a2)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->hooks->hook_map["FinishScribing"]->original(FinishScribing)(t, u, a1, a2);
    zeal->spell_sets->handle_finished_scribing(a1, a2);
}

// Hook called after a right click up on a spell gem.
static int __fastcall SpellGemWnd_HandleRButtonUp(Zeal::EqUI::SpellGemWnd* gem, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->spell_sets->handle_spell_gem_rbutton_up(gem, pt);
    return zeal->hooks->hook_map["SpellGemRbutton"]->original(SpellGemWnd_HandleRButtonUp)(gem, unused, pt, flag);
}

// Hook called to deal with UI notification events to the CastSpellWnd.
// Note: ItemDisplay is re-routing the CastSpellWnd vtable:WndNotification to it's own internal
// function which will then route non-intercepted calls to the detour below.
static int __fastcall CastSpellWnd_WndNotification(Zeal::EqUI::CastSpellWnd* wnd, int unused_edx,
    Zeal::EqUI::BasicWnd* src_wnd, unsigned int flag, void* unknown3)
{
    ZealService* zeal = ZealService::get_instance();
    if (src_wnd && src_wnd == wnd->SpellBook && flag == 0x03) {  // Right button up.
        Zeal::EqUI::CXPoint pt(*Zeal::EqGame::mouse_client_x, *Zeal::EqGame::mouse_client_y);
        zeal->spell_sets->handle_spell_book_rbutton_up(pt);
    }
    return zeal->hooks->hook_map["CastSpellWnd_WndNotification"]->
        original(CastSpellWnd_WndNotification)(wnd, unused_edx, src_wnd, flag, unknown3);
}

SpellSets::SpellSets(ZealService* zeal)
{
    zeal->callbacks->AddGeneric([this]() { callback_main();  }, callback_type::Render);
    zeal->callbacks->AddGeneric([this]() { callback_init_ui(); }, callback_type::InitUI);
    zeal->callbacks->AddGeneric([this]() { callback_clean_ui();  }, callback_type::CleanUI);

    zeal->hooks->Add("FinishMemorizing", 0x434b38, FinishMemorizing, hook_type_detour);
    zeal->hooks->Add("FinishScribing", 0x43501f, FinishScribing, hook_type_detour);
    zeal->hooks->Add("SpellGemRbutton", 0x5A67B0, SpellGemWnd_HandleRButtonUp, hook_type_detour);
    zeal->hooks->Add("CastSpellWnd_WndNotification", 0x0040a32a, CastSpellWnd_WndNotification, hook_type_detour);

    // Block registering new command with old UI (not supported)
    if (!Zeal::EqGame::is_new_ui())
        return;

    zeal->commands_hook->Add("/spellset", {"/ss"}, "Load, save, delete or list your spellsets.",
        [this, zeal](std::vector<std::string>& args) {
            if (args.size() < 3 || !Zeal::EqGame::get_self() || !Zeal::EqGame::get_char_info())
            {
                Zeal::EqGame::print_chat("usage: /spellset save/load/list [name]");
            }
            else
            {
                if (Zeal::String::compare_insensitive(args[1], "test"))
                {
                    create_spells_menus();
                    create_spellsets_menus();
                }
                if (Zeal::String::compare_insensitive(args[1], "save"))
                {
                    save(args[2]);
                }
                if (Zeal::String::compare_insensitive(args[1], "delete") || Zeal::String::compare_insensitive(args[1], "remove"))
                {
                    remove(args[2]);
                }
                if (Zeal::String::compare_insensitive(args[1], "load"))
                {
                    load(args[2]);
                }
                if (Zeal::String::compare_insensitive(args[1], "list"))
                {
                    initialize_ini_filename();
                    std::vector<std::string> sets = ini.getSectionNames();
                    Zeal::EqGame::print_chat("--- spell sets (%i) ---", sets.size());
                    for (auto& set : sets)
                    {
                        Zeal::EqGame::print_chat(set);
                    }
                    Zeal::EqGame::print_chat("--- end of spell sets ---", sets.size());
                }
            }
            return true;
        }
    );
}

SpellSets::~SpellSets()
{
    callback_clean_ui();  // Should be a redundant no-op call.
}
