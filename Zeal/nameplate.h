#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"

class NamePlate
{
public:
	bool nameplateColors = false;
	bool nameplateconColors = false;
	bool colors_is_enabled() const { return nameplateColors; }
	bool con_colors_is_enabled() const { return nameplateconColors; }
	void colors_set_enabled(bool enabled);	
	void con_colors_set_enabled(bool enabled);
	NamePlate(class ZealService* zeal, class IO_ini* ini);
	~NamePlate();
private:
};

