#pragma once
#include "EqStructures.h"
#include "EqUI.h"
#include "IO_ini.h"
#include <vector>

class EquipItem
{
public:
	EquipItem(class ZealService* pHookWrapper, class IO_ini* ini);
	~EquipItem();
	void HandleRButtonUp(Zeal::EqUI::InvSlotWnd* wnd, int mouse_x, int mouse_y);
	void SetEnabled(bool in_enabled);
	bool enabled;
private:
	bool ClickInventoryWindowSlot(int invSlot, unsigned int flags = 0);
	int EQUIP_PRIORITY_ORDER[21] = {
		12, 13, 10,           // Primary, Secondary, Range
		16, 17, 1, 6, 11, 18, // Chest, Legs, Head, Arms, Hands, Feet
		5, 7, 4, 2, 19, 8, 9, // Shoulders, Back, Neck, Face, Waist, WristLeft, WristRight,
		0, 3, 14, 15, 20      // EarLeft, EarRight, RingLeft, RingRight, Ammo
	};
};

