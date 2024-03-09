#include "spellsets.h"
#include "Zeal.h"
#include "StringUtil.h"
#include "EqAddresses.h"
#include "SpellCategories.h"
#include <algorithm>

void SpellSets::save(const std::string& name)
{
    std::stringstream ss;
    ss << ".\\" << Zeal::EqGame::get_self()->Name << "_spellsets.ini";
    this->ini->set(ss.str());
    Zeal::EqGame::print_chat("Saving spellset [%s]", name.c_str());
    for (size_t i = 0; i < EQ_NUM_SPELL_GEMS; i++)
    {
        ini->setValue(name, std::to_string(i), Zeal::EqGame::get_self()->CharInfo->MemorizedSpell[i]);
    }
    destroy_context_menus();
    create_context_menus(true);
}
void SpellSets::load(const std::string& name)
{
    std::stringstream ss;
    ss << ".\\" << Zeal::EqGame::get_self()->Name << "_spellsets.ini";
    ini->set(ss.str());
    mem_buffer.clear();


    if (!ini->exists(name, "0"))
    {
        Zeal::EqGame::print_chat("The spellset [%s] does not exist in [%s]", name.c_str(), ss.str().c_str());
        return;
    }
    Zeal::EqGame::print_chat("Loading spellset [%s]", name.c_str());

    for (size_t gem_index = 0; gem_index < EQ_NUM_SPELL_GEMS; gem_index++)
    {
      short spell_id = ini->getValue<WORD>(name, std::to_string(gem_index));
      if (spell_id == 0)
      {
          Zeal::EqGame::print_chat("Error loading spellset [%s] spell id at index [%i] is 0", name.c_str(), gem_index);
          break;
      }
      short memmed_spell = Zeal::EqGame::get_self()->CharInfo->MemorizedSpell[gem_index];
      if (memmed_spell != spell_id && spell_id!=-1)
      {
          if (memmed_spell != -1) 
              Zeal::EqGame::Spells::Forget(gem_index);
                
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
        original_stance = (Stance)Zeal::EqGame::get_self()->StandingState;
        Zeal::EqGame::Spells::Memorize(mem_buffer.back().first, mem_buffer.back().second);
    }
}


void SpellSets::finished_scribing(int a1, int a2)
{
    destroy_context_menus();
    create_context_menus(true);
}

void SpellSets::finished_memorizing(int a1, int a2)
{
    if (!Zeal::EqGame::Windows->SpellBook)
        return;
    if (Zeal::EqGame::Windows->SpellBook && !Zeal::EqGame::Windows->SpellBook->IsVisible)
        mem_buffer.clear();
    if (mem_buffer.size())
    {
        mem_buffer.pop_back();
        if (mem_buffer.size())
            Zeal::EqGame::Spells::Memorize(mem_buffer.back().first, mem_buffer.back().second);
        else if (Zeal::EqGame::Windows->SpellBook->IsVisible)
        {
            Zeal::EqGame::change_stance(original_stance);
            Zeal::EqGame::Windows->SpellBook->IsVisible = false;
        }
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
    if (Zeal::EqGame::Windows->SpellBook && !Zeal::EqGame::Windows->SpellBook->IsVisible && mem_buffer.size())
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
void SpellSets::handle_menu_mem(int book_index, int gem_index)
{
    original_stance = (Stance)Zeal::EqGame::get_self()->StandingState;
    mem_buffer.push_back({ book_index,gem_index });
    Zeal::EqGame::Spells::Memorize(book_index, gem_index);
}
static int __fastcall SpellSetRButtonUp(Zeal::EqUI::EQWND* pWnd, unsigned int Message, void* data)
{
    return 1;
}
static int __stdcall SpellSetMenuNotification(Zeal::EqUI::EQWND* pWnd, unsigned int Message, void* data)
{
    Zeal::EqGame::print_chat("Message: %i  data: %i", Message, data);
    ZealService::get_instance()->spell_sets->load(ZealService::get_instance()->spell_sets->spellset_map[(int)data]);
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
       // int spellbook_info
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


int __fastcall SpellGemWnd_HandleRButtonUp(Zeal::EqUI::SpellGem* gem, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
    ZealService* zeal = ZealService::get_instance();
    if (gem->spellicon == -1)
    {
        zeal->spell_sets->last_gem_clicked = gem;
        Zeal::EqGame::Windows->ContextMenuManager->PopupMenu(zeal->spell_sets->SpellMenuIndex, pt, (Zeal::EqUI::EQWND*)zeal->spell_sets->menu);
    }
    return zeal->hooks->hook_map["SpellGemRbutton"]->original(SpellGemWnd_HandleRButtonUp)(gem, unused, pt, flag);
}

int __fastcall SpellGemWnd_Book_HandleRButtonUp(Zeal::EqUI::pInstWindows* btn, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
    ZealService* zeal = ZealService::get_instance();
    if (Zeal::EqGame::Windows->SpellBook && !Zeal::EqGame::Windows->SpellBook->IsVisible) //if the spell book is open it causes the menu to popup when you right click a spell
        Zeal::EqGame::Windows->ContextMenuManager->PopupMenu(zeal->spell_sets->SpellSetMenuIndex, pt, (Zeal::EqUI::EQWND*)zeal->spell_sets->spellset_menu);
    return zeal->hooks->hook_map["SpellGemBookRbutton"]->original(SpellGemWnd_Book_HandleRButtonUp)(btn, unused, pt, flag);
}


bool compareBySpellLevel(const Zeal::EqStructures::SPELL* a, const Zeal::EqStructures::SPELL* b) {
    Zeal::EqStructures::EQCHARINFO* self_char = Zeal::EqGame::get_self()->CharInfo;
    int aLevel = a->Level[self_char->Class - 1];
    int bLevel = b->Level[self_char->Class - 1];
    return aLevel > bLevel;
}

void SpellSets::destroy_context_menus()
{
    if (menu)
    {
        for (auto& [ind, men] : MenuMap)
            men->RemoveAllMenuItems();
        spellset_menu->RemoveAllMenuItems();
        //Zeal::EqGame::Windows->ContextMenuManager->RemoveMenu(SpellMenuIndex, true);
        //Zeal::EqGame::Windows->ContextMenuManager->RemoveMenu(SpellSetMenuIndex, false);
        MenuMap.clear();
    }
}
void SpellSets::create_context_menus(bool force)
{
    if ((!menu || force)  && Zeal::EqGame::get_self())
    {
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
            menudata md;
            md.ID = s->ID;
            int Level = s->Level[self_char->Class - 1];
            std::stringstream ss;
            ss << Level << " - " << s->Name;
            md.Name = ss.str();
            DWORD categoryID = GetSpellCategory(s->ID);
            DWORD subcategoryID = GetSpellSubCategory(s->ID);
            std::string category = GetSpellCategoryName(categoryID);
            std::string subcategory = GetSpellSubCategoryName(subcategoryID);
            SpellCategory[category][subcategory].push_back(md);
        }
        if (!menu)
            menu = new Zeal::EqUI::ContextMenu(0, 0, { 100,100,100,100 });
        menu->HasChildren = 1;
        menu->HasSiblings = 1;
        menu->Unknown0x015 = 0;
        menu->Unknown0x016 = 0;
        menu->Unknown0x017 = 0;
        menu->fnTable->basic.WndNotification = SpellsMenuNotification;
        for (auto& [cat, sub] : SpellCategory)
        {
            Zeal::EqUI::ContextMenu* SubCategoryMenu = new Zeal::EqUI::ContextMenu(0, 0, { 100,100,100,100 });
            for (auto& [subcat, spells] : sub)
            {
                Zeal::EqUI::ContextMenu* SpellMenu = new Zeal::EqUI::ContextMenu(0, 0, { 100,100,100,100 });

                for (auto& sp : spells)
                {
                    SpellMenu->AddMenuItem(sp.Name, 0x10000 + sp.ID);
                }
                int addedindex = Zeal::EqGame::Windows->ContextMenuManager->AddMenu(SpellMenu);
                MenuMap[addedindex] = SpellMenu;
                addedindex |= 0x80000000;
                SubCategoryMenu->AddMenuItem(subcat, addedindex);
            }
            int addedindex = Zeal::EqGame::Windows->ContextMenuManager->AddMenu(SubCategoryMenu);
            MenuMap[addedindex] = SubCategoryMenu;
            addedindex |= 0x80000000;
            menu->AddMenuItem(cat, addedindex);
        }
        SpellMenuIndex = Zeal::EqGame::Windows->ContextMenuManager->AddMenu(menu);
        MenuMap[SpellMenuIndex] = menu;

        spellset_menu = new Zeal::EqUI::ContextMenu(0, 0, { 100,100,100,100 });
        spellset_menu->HasChildren = 1;
        spellset_menu->fnTable->basic.WndNotification = SpellSetMenuNotification;
        spellset_menu->fnTable->basic.HandleRButtonUp = SpellSetRButtonUp;
        std::stringstream ss;
        ss << ".\\" << Zeal::EqGame::get_self()->Name << "_spellsets.ini";
        ini->set(ss.str());
        std::vector<std::string> sets = ini->getSectionNames();
        int header_index = spellset_menu->AddMenuItem("Spell Sets", 0x30000, false);
        spellset_menu->EnableLine(header_index, false);
        //spellset_menu->SetItemColor(header_index, { 255,255,255,255 });
        spellset_menu->AddSeparator();
        for (int i = 0; auto & s : sets)
        {
            spellset_map[0x20000 + i] = s;
            spellset_menu->AddMenuItem(s, 0x20000 + i);
            i++;
        }
        SpellSetMenuIndex = Zeal::EqGame::Windows->ContextMenuManager->AddMenu(spellset_menu);
    }
}

SpellSets::SpellSets(ZealService* zeal)
{
    ini = std::shared_ptr<IO_ini>(new IO_ini(".\\spellsets.ini"));
    zeal->main_loop_hook->add_callback([this]() { callback_main();  });
    zeal->hooks->Add("FinishMemorizing", 0x434b38, FinishMemorizing, hook_type_detour);
    zeal->hooks->Add("FinishScribing", 0x43501f, FinishScribing, hook_type_detour);
    zeal->hooks->Add("SpellGemRbutton", 0x5A67B0, SpellGemWnd_HandleRButtonUp, hook_type_detour);
    zeal->hooks->Add("SpellGemBookRbutton", 0x5955c0, SpellGemWnd_Book_HandleRButtonUp, hook_type_detour);

    

    zeal->commands_hook->add("/spellset", {},
        [this, zeal](std::vector<std::string>& args) {
            if (args.size() < 2)
            {
                Zeal::EqGame::print_chat("usage: /spellset save/load/list [name]");
            }
            else
            {
                if (StringUtil::caseInsensitive(args[1], "test"))
                {
                    destroy_context_menus();
                    create_context_menus(true);
                }
                if (StringUtil::caseInsensitive(args[1], "save"))
                {
                    save(args[2]);
                }
                if (StringUtil::caseInsensitive(args[1], "load"))
                {
                    load(args[2]);
                }
                if (StringUtil::caseInsensitive(args[1], "list"))
                {
                    std::stringstream ss;
                    ss << ".\\" << Zeal::EqGame::get_self()->Name << "_spellsets.ini";
                    ini->set(ss.str());
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
}
SpellSets::~SpellSets()
{
    destroy_context_menus();
}