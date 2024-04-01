#include "alarm.h"
#include "Zeal.h"

void Alarm::Set(int minutes, int seconds)
{
  if (!enabled)
  {
    enabled = true;
    duration = ((minutes * 60) + seconds) * 1000;
    std::ostringstream oss;
    oss << "[Alarm] SET! (" << minutes << "m" << seconds << "s)" << std::endl;
    Zeal::EqGame::print_chat(oss.str());
  }
  else
  {
    Zeal::EqGame::print_chat("[Alarm] Please halt the active alarm before attempting to set a new one.");
  }
}

void Alarm::Halt(void)
{
  if (enabled) {
    enabled = false;
    Zeal::EqGame::print_chat("[Alarm] HALT!");
  }
  else
  {
    Zeal::EqGame::print_chat("[Alarm] No active alarm.");
  }
}

void Alarm::callback_main()
{
  if (enabled)
  {
    if (!start_time)
      start_time = GetTickCount64();

    if (GetTickCount64() - start_time > duration)
    {
      Zeal::EqGame::print_chat("[Alarm] COMPLETED!");
      enabled = false;
    }
  }
  else
  {
    start_time = 0;
  }
}

Alarm::Alarm(ZealService* zeal)
{
	zeal->callbacks->add_callback([this]() { callback_main(); });
}
