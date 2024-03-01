#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqStructures.h"

class BuffDetails
{
public:
  BuffDetails(int index, Zeal::EqStructures::_EQBUFFINFO buff);
  ~BuffDetails() {};
  int BuffSlot;
  Zeal::EqStructures::_EQBUFFINFO Buff;
};

class BuffTimers
{
public:
  BuffTimers(class ZealService* zeal);
  ~BuffTimers() {};
private:
  bool is_OldUI;
  void print_timers(void);
};
