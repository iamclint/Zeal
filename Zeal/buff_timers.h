#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class BuffTimers
{
public:
  BuffTimers(class ZealService* zeal);
  ~BuffTimers() {};
private:
  bool is_OldUI;
  void print_timers(void);
};
