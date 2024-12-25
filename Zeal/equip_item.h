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
};

