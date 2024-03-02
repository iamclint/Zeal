#include "buff_timers.h"
#include "Zeal.h"

BuffDetails::BuffDetails(int index, Zeal::EqStructures::_EQBUFFINFO info) :
  BuffSlot(index), Buff(info)
{
}

// An improvement we could have would be having a pointer/hook to the function that provides
// the spell name based on the spell id.
void BuffTimers::print_timers(void) {
  std::vector<BuffDetails> activeBuffs;
  std::ostringstream oss;

  auto CharInfo = Zeal::EqGame::get_self()->CharInfo;
  for (int i = 0; i < EQ_NUM_BUFFS; ++i) {
    WORD BuffId = CharInfo->Buff[i].SpellId;
    if (BuffId != USHRT_MAX) {
      activeBuffs.push_back(BuffDetails({ i, CharInfo->Buff[i] }));
    }
  }

  if (activeBuffs.size() != 0) {
    for (size_t i = 0; i <  activeBuffs.size(); ++i) {
      BuffDetails details = activeBuffs[i];
      if (details.Buff.SpellId != USHRT_MAX) {
        int Mins = ((details.Buff.Ticks) * 6) / 60;
        int Secs = ((details.Buff.Ticks) * 6) % 60;

        oss << "(" << details.BuffSlot + 1 << ")" << " " << Mins << "m" << Secs << "s";
        if ((i+1) != activeBuffs.size()) { oss << ", "; }
      }
    }
  }
  else
  {
    oss << "[Buffs] None";
  }

  Zeal::EqGame::print_chat(oss.str());
}

BuffTimers::BuffTimers(ZealService* zeal)
{
  is_OldUI = ZealService::get_instance()->ini->getValue<bool>("Defaults", "OldUI");

  if (is_OldUI) {
    zeal->commands_hook->add("/buffs", {},
      [this](std::vector<std::string>& args) {
        print_timers();
        return true;
      }
    );
  }
}
