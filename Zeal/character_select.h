#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "d3dx8/d3d8.h"
#include "bitmap_font.h"
#include "ZealSettings.h"

class CharacterSelect
{
public:
	CharacterSelect(class ZealService* zeal);
	~CharacterSelect();
	ZealSetting<int> ZoneIndex = { -1, "CharacterSelect", "ZoneIndex", false };
};

