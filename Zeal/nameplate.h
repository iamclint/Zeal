#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"

class NamePlate
{
public:
	bool nameplateColors = false;
	bool is_enabled() const { return nameplateColors; }
	void set_enabled(bool enabled);	
	NamePlate(class ZealService* zeal, class IO_ini* ini);
	~NamePlate();
private:
};

