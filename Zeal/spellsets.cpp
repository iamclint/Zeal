#include "spellsets.h"
#include "Zeal.h"
#include "string_util.h"
#include "EqAddresses.h"
#include "SpellCategories.h"
#include <algorithm>

static bool is_valid_name(const std::string& name) {
    if (name.empty() || name.size() > 32)
    {
        Zeal::EqGame::print_chat("Invalid spellset name");
        return false;
    }
    return true;
}

void SpellSets::save(const std::string& name)
{
    if (!is_valid_name(name))
        return;

    set_ini();
    Zeal::EqGame::print_chat("Saving spellset [%s]", name.c_str());
    for (size_t i = 0; i < EQ_NUM_SPELL_GEMS; i++)
    {
        ini->setValue(name, std::to_string(i), Zeal::EqGame::get_self()->CharInfo->MemorizedSpell[i]);
    }
    create_context_menus(true);
}
void SpellSets::remove(const std::string& name)
{
    set_ini();
    Zeal::EqGame::print_chat("Removing spellset [%s]", name.c_str());
    if (!ini->deleteSection(name))
        Zeal::EqGame::print_chat("Error removing spellset [%s]", name.c_str());
    create_context_menus(true);
}
void SpellSets::remove_selected()
{
    remove(ui_selected_name);
}
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

    set_ini();

    if (!ini->exists(name, "0"))
    {
        Zeal::EqGame::print_chat("The spellset [%s] does not exist", name.c_str());
        return;
    }
    Zeal::EqGame::print_chat("Loading spellset [%s]", name.c_str());

    for (int gem_index = EQ_NUM_SPELL_GEMS - 1; gem_index >= 0; gem_index--)
    {
      short spell_id = ini->getValue<WORD>(name, std::to_string(gem_index));
      if (spell_id == -1) { // Empty slot when saved.
          Zeal::EqGame::Spells::Forget(gem_index);
          continue;  // Leave it empty.
      }

      if (spell_id < 1 || spell_id >= EQ_NUM_SPELLS || !spell_manager->Spells[spell_id])
      {
          Zeal::EqGame::print_chat("Error loading spellset [%s]: spell id at index [%i] is [%i]",
                    name.c_str(), gem_index, spell_id);
          break;
      }

      short memmed_spell = Zeal::EqGame::get_self()->CharInfo->MemorizedSpell[gem_index];
      if (memmed_spell != spell_id)
      {
          if (memmed_spell != -1)
              Zeal::EqGame::Spells::Forget(gem_index);

          // Bail out early if too low level (skips memorization time and state issues).
          if (!check_caster_level(spell_id))
              continue;

          for (size_t book_index = 0; book_index < EQ_NUM_SPELL_BOOK_SPELLS; book_index++)
          {
              if (Zeal::EqGame::get_self()->CharInfo->SpellBook[book_index] == spell_id)
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


void SpellSets::finished_scribing(int a1, int a2)
{
    create_context_menus(true);
}

void SpellSets::finished_memorizing(int a1, int a2)
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

void __fastcall FinishMemorizing(int t, int u, int a1, int a2)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->spell_sets->finished_memorizing(a1, a2);
    zeal->hooks->hook_map["FinishMemorizing"]->original(FinishMemorizing)(t, u, a1, a2);
}
void __fastcall FinishScribing(int t, int u, int a1, int a2)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->hooks->hook_map["FinishScribing"]->original(FinishScribing)(t, u, a1, a2);
    zeal->spell_sets->finished_scribing(a1, a2);
}

void SpellSets::callback_main()
{
    if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->SpellBook && !Zeal::EqGame::Windows->SpellBook->IsVisible && mem_buffer.size())
        mem_buffer.clear();

    create_context_menus();
}

static Zeal::EqStructures::SPELL* GetSpell(int id)
{
    Zeal::EqStructures::EQCHARINFO* self_char = Zeal::EqGame::get_self()->CharInfo;
    id = self_char->SpellBook[id];
    if (id == 0 || id >= 4000)
        return 0;
    return Zeal::EqGame::get_spell_mgr()->Spells[id];
}

// Memorization fails (and locks up spellbook state) if the caster level is too low.
bool SpellSets::check_caster_level(int spell_id) const {
    const auto* spell_manager = Zeal::EqGame::get_spell_mgr();
    const auto* self_char = Zeal::EqGame::get_char_info();
    if (!spell_manager || !self_char) {
        Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "SpellSet game state error. Aborting.");
        return false;
    }

    const int spell_level = spell_manager->Spells[spell_id]->ClassLevel[self_char->Class];
    if (self_char->Level < spell_level)
    {
        Zeal::EqGame::print_chat(USERCOLOR_SHOUT,
            "Stop dying. You will have to achieve level %i to memorize the spell %s",
            spell_level, spell_manager->Spells[spell_id]->Name);
        return false;
    }

    return true;
}

void SpellSets::handle_menu_mem(int book_index, int gem_index)
{
    bool interrupted = !mem_buffer.empty();
    mem_buffer.clear();  // Clear any in-progress reloads.
    Zeal::EqGame::Spells::StopSpellBookAction();

    const auto* spell_manager = Zeal::EqGame::get_spell_mgr();
    const auto* self_char = Zeal::EqGame::get_char_info();
    if (!spell_manager || !self_char)
        return;
    
    // Bail out early if too low level (skips memorization time).
    const int spell_id = self_char->SpellBook[book_index];
    if (!check_caster_level(spell_id))
        return;

    if (!interrupted)
        original_stance = (Stance)Zeal::EqGame::get_self()->StandingState;
    mem_buffer.push_back({ book_index,gem_index });
    Zeal::EqGame::Spells::Memorize(book_index, gem_index);
}

static int __stdcall SpellSetMenuNotification(Zeal::EqUI::EQWND* pWnd, unsigned int Message, void* data)
{
    int msg_data = (int)data;
    if (msg_data == 0x21000)
        ZealService::get_instance()->ui->inputDialog->show("Spell Set", "Save spell set as:", "Save", "Cancel", [](std::string msg) { ZealService::get_instance()->spell_sets->save(msg); }, nullptr, true);
    else if (msg_data>=0x22000)
        ZealService::get_instance()->ui->inputDialog->show("Spell Set", "Delete " + ZealService::get_instance()->spell_sets->spellset_map[msg_data - 0x02000] + "?", "Yes", "No", [msg_data](std::string msg) { ZealService::get_instance()->spell_sets->remove(ZealService::get_instance()->spell_sets->spellset_map[msg_data - 0x02000]); }, nullptr, false);
    else
        ZealService::get_instance()->spell_sets->load(ZealService::get_instance()->spell_sets->spellset_map[msg_data]);
    return 1;
}
static int __stdcall SpellSetDeleteMenuNotification(Zeal::EqUI::EQWND* pWnd, unsigned int Message, void* data)
{
    ZealService::get_instance()->spell_sets->remove_selected();
    return 1;
}
static int __stdcall SpellsMenuNotification(Zeal::EqUI::EQWND* pWnd, unsigned int Message, void* data)
{
    ZealService* zeal = ZealService::get_instance();
    Zeal::EqStructures::EQCHARINFO* self_char = Zeal::EqGame::get_self()->CharInfo;
    int index = -1;
    for (int i = 0; i < 8; i++)
    {
        if (Zeal::EqGame::Windows->SpellGems->SpellSlots[i] == zeal->spell_sets->last_gem_clicked)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        Zeal::EqStructures::EQCHARINFO* self_char = Zeal::EqGame::get_self()->CharInfo;
        int spell_id = (DWORD)data - 0x10000;
        int book_index = -1;
        for (int i = 0; i < EQ_NUM_SPELL_BOOK_SPELLS; i++)
        {
            if (self_char->SpellBook[i]==spell_id)
            {
                book_index = i;
                break;
            }
        }
        if (book_index != -1)
        {
            ZealService::get_instance()->spell_sets->handle_menu_mem(book_index, index);
        }
        else
        {
            Zeal::EqGame::print_chat("Could not find spell [%i] in your spellbook.", spell_id);
        }
    }
    else
    {
        Zeal::EqGame::print_chat("Incorrect gem index [%i].", index);
    }
    return 1;
}


static int __fastcall SpellGemWnd_HandleRButtonUp(Zeal::EqUI::SpellGem* gem, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
    ZealService* zeal = ZealService::get_instance();
    if (gem->spellicon == -1)
    {
        zeal->spell_sets->last_gem_clicked = gem;
        Zeal::EqGame::Windows->ContextMenuManager->PopupMenu(zeal->spell_sets->SpellMenuIndex, pt, (Zeal::EqUI::EQWND*)zeal->spell_sets->menu);
    }
    return zeal->hooks->hook_map["SpellGemRbutton"]->original(SpellGemWnd_HandleRButtonUp)(gem, unused, pt, flag);
}

static int __fastcall SpellGemWnd_Book_HandleRButtonUp(Zeal::EqUI::EQWND* btn, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
    ZealService* zeal = ZealService::get_instance();

    if (!Zeal::EqGame::Windows || !Zeal::EqGame::Windows->SpellGems)
        return zeal->hooks->hook_map["SpellGemWnd_Book_HandleRButtonUp"]->original(SpellGemWnd_Book_HandleRButtonUp)(btn, unused, pt, flag);
    if (btn == Zeal::EqGame::Windows->SpellGems->SpellBook)
        Zeal::EqGame::Windows->ContextMenuManager->PopupMenu(zeal->spell_sets->SpellSetMenuIndex, pt, (Zeal::EqUI::EQWND*)zeal->spell_sets->spellset_menu);
    return zeal->hooks->hook_map["SpellGemWnd_Book_HandleRButtonUp"]->original(SpellGemWnd_Book_HandleRButtonUp)(btn, unused, pt, flag);
}


bool compareBySpellLevel(const Zeal::EqStructures::SPELL* a, const Zeal::EqStructures::SPELL* b) {
    Zeal::EqStructures::EQCHARINFO* self_char = Zeal::EqGame::get_self()->CharInfo;
    int aLevel = a->ClassLevel[self_char->Class];
    int bLevel = b->ClassLevel[self_char->Class];
    if (aLevel != bLevel)
        return aLevel > bLevel;
    return a->Name > b->Name;
}

void SpellSets::destroy_context_menus()
{
    if (!Zeal::EqGame::is_new_ui()) { return; } // prevent callback crashing oldui

    if (Zeal::EqGame::Windows->ContextMenuManager)
    {
        int* ptr = reinterpret_cast<int*>(Zeal::EqGame::Windows->ContextMenuManager);

        for (auto it = MenuMap.rbegin(); it != MenuMap.rend(); ++it) {
            auto [index, cmenu] = *it;
            // Only release if there's a match (should always match).
            if (cmenu == Zeal::EqGame::Windows->ContextMenuManager->Menus[index])             {
                // The manager should call the custom destructor and release the resources.
                Zeal::EqGame::Windows->ContextMenuManager->RemoveMenu(index, true);
            }
        }
    }
    menu = 0;
    spellset_menu = 0;
    MenuMap.clear();
}

void SpellSets::set_ini()
{
    std::stringstream ss;
    ss << ".\\" << Zeal::EqGame::get_char_info()->Name << "_spellsets.ini";
    ini->set(ss.str());
}

static Zeal::EqUI::ContextMenu* InitializeMenu(void* notificationFunc = nullptr) {
    auto menu = Zeal::EqUI::ContextMenu::Create(0, 0, { 100, 100, 100, 100 });
    if (!menu)
        throw std::bad_alloc();

    menu->HasChildren = true;  // Note: Evaluate if these are still necessary.
    menu->HasSiblings = true;
    menu->Unknown0x015 = 0;
    menu->Unknown0x016 = 0;
    menu->Unknown0x017 = 0;
    menu->fnTable->WndNotification = notificationFunc;
    return menu;
}

int SpellSets::add_menu_to_manager(Zeal::EqUI::ContextMenu* new_menu)
{
    int addedindex = Zeal::EqGame::Windows->ContextMenuManager->AddMenu(new_menu);
    MenuMap[addedindex] = new_menu;
    return addedindex;
}

void SpellSets::create_context_menus(bool force)
{
    if (!Zeal::EqGame::is_new_ui()) { return; } // prevent callback crashing oldui

    if (force)
        destroy_context_menus();

    if (!menu && Zeal::EqGame::get_self() && Zeal::EqGame::is_in_game())
    {
        ZealService* zeal = ZealService::get_instance();
        set_ini();

        if (zeal->hooks->hook_map.count("SpellGemWnd_Book_HandleRButtonUp")==0)
            zeal->hooks->Add("SpellGemWnd_Book_HandleRButtonUp", &Zeal::EqGame::Windows->SpellGems->SpellBook->vtbl->HandleRButtonUp, SpellGemWnd_Book_HandleRButtonUp, hook_type_vtable);

        Zeal::EqStructures::EQCHARINFO* self_char = Zeal::EqGame::get_self()->CharInfo;
        std::vector<Zeal::EqStructures::SPELL*> spells;
        for (int N = 0; N < EQ_NUM_SPELL_BOOK_SPELLS; N++) {
            if (Zeal::EqStructures::SPELL* pSpell = GetSpell(N))
            {
                spells.push_back(pSpell);
            }
        }
        std::sort(spells.begin(), spells.end(), compareBySpellLevel);
        SpellCategory.clear();
        for (auto& s : spells) 
        {
            SpellCat SpellCatData = getSpellCategoryAndSubcategory(s->ID);
            menudata md;
            md.ID = s->ID;
            int Level = s->ClassLevel[self_char->Class];
            std::stringstream ss;
            if (SpellCatData.NewName.length())
                ss << Level << " - " << SpellCatData.NewName;
            else
                ss << Level << " - " << s->Name;
            md.Name = ss.str();
            
            std::string category = GetSpellCategoryName(SpellCatData.Category);
            std::string subcategory = GetSpellSubCategoryName(SpellCatData.SubCategory);
            SpellCategory[category][subcategory].push_back(md);
        }
        if (!menu)
            menu = InitializeMenu(SpellsMenuNotification);
            
        for (auto& [cat, sub] : SpellCategory)
        {
            Zeal::EqUI::ContextMenu* SpellSubCategory = InitializeMenu();
            for (auto& [subcat, spells] : sub)
            {
                Zeal::EqUI::ContextMenu* SpellMenu = InitializeMenu();
                for (auto& sp : spells)
                    SpellMenu->AddMenuItem(sp.Name, 0x10000 + sp.ID);
                SpellSubCategory->AddMenuItem(subcat, add_menu_to_manager(SpellMenu), false, true);
            }
            menu->AddMenuItem(cat, add_menu_to_manager(SpellSubCategory), false, true);
        }
        SpellMenuIndex = add_menu_to_manager(menu);

        if (!spellset_menu)
            spellset_menu = InitializeMenu(SpellSetMenuNotification);

        spellsets.clear();
        spellsets = ini->getSectionNames();
        std::sort(spellsets.begin(), spellsets.end());
 
        int header_index = spellset_menu->AddMenuItem("Spell Sets", 0x30000, false);
        spellset_menu->EnableLine(header_index, false);
        //spellset_menu->SetItemColor(header_index, { 255,255,255,255 });
        for (int i = 0; auto & s : spellsets)
        {
            spellset_menu->AddMenuItem(s, 0x20000 + i);
            spellset_map[0x20000 + i] = s;
            i++;
        }
        spellset_menu->AddSeparator();
        Zeal::EqUI::ContextMenu* DeleteSubCatMenu = InitializeMenu();
        for (int i = 0; auto & s : spellsets)
        {
            DeleteSubCatMenu->AddMenuItem(s, 0x22000 + i);
            i++;
        }
        spellset_menu->AddMenuItem("Delete", add_menu_to_manager(DeleteSubCatMenu), false, true);
        spellset_menu->AddSeparator();
        spellset_menu->AddMenuItem("Save New", 0x21000);
        SpellSetMenuIndex = add_menu_to_manager(spellset_menu);
    }
}

void SpellSets::CleanUI()
{
    destroy_context_menus();
}

SpellSets::SpellSets(ZealService* zeal)
{
    menu = 0;
    original_stance = Stand;
    spellset_menu = 0;
    ini = std::shared_ptr<IO_ini>(new IO_ini(".\\spellsets.ini"));
    zeal->callbacks->AddGeneric([this]() { callback_main();  }, callback_type::Render);
    zeal->callbacks->AddGeneric([this]() { CleanUI();  }, callback_type::CleanUI);
    //zeal->callbacks->AddGeneric([this]() { callback_characterselect();  }, callback_type::CharacterSelect);
    zeal->hooks->Add("FinishMemorizing", 0x434b38, FinishMemorizing, hook_type_detour);
    zeal->hooks->Add("FinishScribing", 0x43501f, FinishScribing, hook_type_detour);
    zeal->hooks->Add("SpellGemRbutton", 0x5A67B0, SpellGemWnd_HandleRButtonUp, hook_type_detour);
    // wrap it for now to prevent users form crashing themselves on oldui until functionality potentially gets added.
    if (Zeal::EqGame::is_new_ui())
    {
        zeal->commands_hook->Add("/spellset", {"/ss"}, "Load, save, delete or list your spellsets.",
            [this, zeal](std::vector<std::string>& args) {
                if (args.size() < 3)
                {
                    Zeal::EqGame::print_chat("usage: /spellset save/load/list [name]");
                }
                else
                {
                    if (Zeal::String::compare_insensitive(args[1], "test"))
                    {
                        create_context_menus(true);
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
                        set_ini();
                        std::vector<std::string> sets = ini->getSectionNames();
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
    };
}
SpellSets::~SpellSets()
{
    destroy_context_menus();
}
