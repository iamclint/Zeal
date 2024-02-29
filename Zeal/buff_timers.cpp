#include "buff_timers.h"
#include "Zeal.h"

// An improvement we could have would be having a pointer/hook to the function that provides
// the spell name based on the spell id.
void BuffTimers::print_timers(void) {
  std::stringstream ss;
  bool apply_delimeter[EQ_NUM_BUFFS]{};

  auto CharInfo = Zeal::EqGame::get_self()->CharInfo;
  for (size_t i = 0; i < EQ_NUM_BUFFS; ++i) {
    WORD BuffId = CharInfo->Buff[i].SpellId;
    apply_delimeter[i] = (BuffID != USHRT_MAX);
  }
  apply_delimeter[0] = false;

  for (size_t i = 0; i < EQ_NUM_BUFFS; ++i) {
    WORD BuffId = CharInfo->Buff[i].SpellId;
    if (BuffId != USHRT_MAX) {
      int Mins = ((CharInfo->Buff[i].Ticks) * 6) / 60;
      int Secs = ((CharInfo->Buff[i].Ticks) * 6) % 60;

      if (apply_delimeter[i]) { ss << ", "; }
      ss << "(" << i + 1 << ")" << " " << Mins << "m" << Secs << "s";
  }

  Zeal::EqGame::print_chat(ss.str());
}

BuffTimers::BuffTimers(ZealService* zeal)
{
  is_OldUI = ZealService::get_instance()->ini->getValue<bool>("Defaults", "OldUI");

  zeal->commands_hook->add("/buffs", {},
    [this](std::vector<std::string>& args) {
      if (is_OldUI)
        print_timers();
      Zeal::EqGame::do_say(true, "");
      return true;
    }
  );
}
