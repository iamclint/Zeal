#include "spellsets.h"
#include "Zeal.h"
#include "StringUtil.h"
#include "EqAddresses.h"

//class CMQMenuWnd : public CCustomMenu
//{
//public:
//    CMQMenuWnd(CXRect rect) :CCustomMenu(rect)
//    {
//        SetWndNotification(CMQMenuWnd);
//    }
//
//    ~CMQMenuWnd()
//    {
//    }
//    int WndNotification(CXWnd* pWnd, unsigned int Message, void* data)
//    {
//        bool bFound = false;
//        int slot = 0;
//        PEQCASTSPELLWINDOW addr = (PEQCASTSPELLWINDOW)pCastSpellWnd;
//        for (; slot < 8; slot++) {
//            PEQCASTSPELLGEM address = addr->SpellSlots[slot];
//            if (address == LastSpellGemClicked) {
//                bFound = true;
//                break;
//            }
//        }
//        if (bFound) {
//            std::string SpellName = GetSpellNameByID((DWORD)data - 0x10000);
//            if (SpellName == "Unknown Spell") {
//                WriteChatf("ERROR: Spell not found in struct, has there been a patch?");
//                return 0;
//            }
//            CHAR szTemp[256] = { 0 };
//            sprintf_s(szTemp, "%d \"%s\"", slot + 1, SpellName.c_str());
//            MemSpell(GetCharInfo()->pSpawn, szTemp);
//        }
//        else {
//            WriteChatf("ERROR: Slot not found in struct, has there been a patch?");
//        }
//        return 1;
//    };
//};


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
        if (Zeal::EqGame::Windows->CSpellBook && !Zeal::EqGame::Windows->CSpellBook->IsVisible)
            Zeal::EqGame::Spells::OpenBook();

        Zeal::EqGame::Spells::Memorize(mem_buffer.back().first, mem_buffer.back().second);
    }
}

void SpellSets::finished_memorizing(int a1, int a2)
{
    if (Zeal::EqGame::Windows->CSpellBook && !Zeal::EqGame::Windows->CSpellBook->IsVisible)
        mem_buffer.clear();
    if (mem_buffer.size())
    {
        mem_buffer.pop_back();
        if (mem_buffer.size())
            Zeal::EqGame::Spells::Memorize(mem_buffer.back().first, mem_buffer.back().second);
        else if (Zeal::EqGame::Windows->CSpellBook->IsVisible)
        {
            Zeal::EqGame::change_stance(original_stance);
            Zeal::EqGame::Windows->CSpellBook->IsVisible = false;
        }
    }
}

void __fastcall FinishMemorizing(int t, int u, int a1, int a2)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->spell_sets->finished_memorizing(a1, a2);
    zeal->hooks->hook_map["FinishMemorizing"]->original(FinishMemorizing)(t, u, a1, a2);
}


void SpellSets::callback_main()
{
    if (Zeal::EqGame::Windows->CSpellBook && !Zeal::EqGame::Windows->CSpellBook->IsVisible && mem_buffer.size())
        mem_buffer.clear();
}
SpellSets::SpellSets(ZealService* zeal)
{
    zeal->main_loop_hook->add_callback([this]() { callback_main();  });
    zeal->hooks->Add("FinishMemorizing", 0x434b38, FinishMemorizing, hook_type_detour);
    ini = std::shared_ptr<IO_ini>(new IO_ini(".\\spellsets.ini"));

    zeal->commands_hook->add("/spellset", {},
        [this, zeal](std::vector<std::string>& args) {
            if (args.size() < 2)
            {
                Zeal::EqGame::print_chat("usage: /spellset save [name]");
            }
            else
            {
                if (StringUtil::caseInsensitive(args[1], "save"))
                {
                    save(args[2]);
                }
                if (StringUtil::caseInsensitive(args[1], "load"))
                {
                    load(args[2]);
                }
            }
            return true;
        }
    );
}
SpellSets::~SpellSets()
{

}