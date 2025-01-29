#pragma once
#include "EqStructures.h"
#include "EqUI.h"
#include "ZealSettings.h"
#include <vector>

class EquipItem
{
public:
	EquipItem(class ZealService* pHookWrapper);
	~EquipItem();

	bool HandleRButtonUp(Zeal::EqUI::InvSlot* inv_slot);

	ZealSetting<bool> Enabled = { false, "Zeal", "RightClickToEquip", false };

private:
	Zeal::EqUI::InvSlot* GetInventorySlot(int inv_slot_id);
	int EQUIP_PRIORITY_ORDER[21] = {
		12, 13, 10,           // Primary, Secondary, Range
		16, 17, 1, 6, 11, 18, // Chest, Legs, Head, Arms, Hands, Feet
		5, 7, 4, 2, 19, 8, 9, // Shoulders, Back, Neck, Face, Waist, WristLeft, WristRight,
		0, 3, 14, 15, 20      // EarLeft, EarRight, RingLeft, RingRight, Ammo
	};
};

