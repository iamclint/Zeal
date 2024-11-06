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
	bool nameplateSelf = false;
	bool nameplateX = false;
	bool nameplateRaidPets = false;
	bool nameplateCharSelect = false;

	bool colors_is_enabled() const { return nameplateColors; }
	bool con_colors_is_enabled() const { return nameplateconColors; }
	bool self_is_enabled() const { return nameplateSelf; }
	bool x_is_enabled() const { return nameplateX; }
	bool raidpets_is_enabled() const { return nameplateRaidPets; }
	bool charselect_is_enabled() const { return nameplateCharSelect; }
	void colors_set_enabled(bool enabled);	
	void con_colors_set_enabled(bool enabled);
	void self_set_enabled(bool enabled);
	void x_set_enabled(bool enabled);
	void raidpets_set_enabled(bool enabled);
	void charselect_set_enabled(bool enabled);
	void HandleState(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn);
	void HandleTint(Zeal::EqStructures::Entity* spawn);
	NamePlate(class ZealService* zeal, class IO_ini* ini);
	~NamePlate();
private:
};
