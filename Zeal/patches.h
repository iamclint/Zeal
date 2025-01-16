#pragma once
#include "ZealSettings.h"
class patches
{
public:
	ZealSetting<bool> BrownSkeletons = { false, "Zeal", "BrownSkeletons", false, [this](bool val) { SetBrownSkeletons(); } };
	patches();
	void fonts();
private:
	void SetBrownSkeletons();
};

