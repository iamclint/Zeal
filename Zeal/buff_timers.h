#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "EqStructures.h"

class BuffDetails
{
public:
  BuffDetails(size_t index, Zeal::EqStructures::_EQBUFFINFO buff);
  ~BuffDetails() {};
  size_t BuffSlot;
  Zeal::EqStructures::_EQBUFFINFO Buff;
};

class BuffTimers
{
public:
  BuffTimers(class ZealService* zeal);
  ~BuffTimers() {};
private:
  void print_timers(void);
};
