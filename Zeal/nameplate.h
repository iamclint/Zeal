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
	bool nameplateHideSelf = false;
	bool nameplateX = false;
	bool nameplateHideRaidPets = false;
	bool nameplateCharSelect = false;
	bool nameplateTargetColor = false;
	bool nameplateTargetMarker = false;
	bool nameplateTargetHealth = false;

	bool colors_is_enabled() const { return nameplateColors; }
	bool con_colors_is_enabled() const { return nameplateconColors; }
	bool hide_self_is_enabled() const { return nameplateHideSelf; }
	bool x_is_enabled() const { return nameplateX; }
	bool hide_raidpets_is_enabled() const { return nameplateHideRaidPets; }
	bool charselect_is_enabled() const { return nameplateCharSelect; }
	bool target_color_is_enabled() const { return nameplateTargetColor; }
	bool target_marker_is_enabled() const { return nameplateTargetMarker; }
	bool target_health_is_enabled() const { return nameplateTargetHealth; }
	void colors_set_enabled(bool enabled);	
	void con_colors_set_enabled(bool enabled);
	void hide_self_set_enabled(bool enabled);
	void x_set_enabled(bool enabled);
	void hide_raidpets_set_enabled(bool enabled);
	void charselect_set_enabled(bool enabled);
	void target_color_set_enabled(bool enabled);
	void target_marker_set_enabled(bool enabled);
	void target_health_set_enabled(bool enabled);
	void HandleState(void* this_ptr, void* not_used, Zeal::EqStructures::Entity* spawn);
	void HandleTint(Zeal::EqStructures::Entity* spawn);
	NamePlate(class ZealService* zeal, class IO_ini* ini);
	~NamePlate();
private:
};
