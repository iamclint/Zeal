#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
class AutoFire
{
public:
	bool autofire = false;
	bool HandleDoAttack(Zeal::EqStructures::Entity* player, uint8_t type, uint8_t p2, Zeal::EqStructures::Entity* target);
	void SetAutoFire(bool enabled, bool do_print=false);
	void Main();
	AutoFire(class ZealService* zeal, class IO_ini* ini);
	~AutoFire();
private: 
	bool was_autoattacking = false;
	bool do_autofire = false;
};

