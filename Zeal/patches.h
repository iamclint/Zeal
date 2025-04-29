#pragma once
#include "ZealSettings.h"
class patches
{
public:
	ZealSetting<bool> BrownSkeletons = { false, "Zeal", "BrownSkeletons", false, [this](bool val) { SetBrownSkeletons(); } };
	ZealSetting<bool> AutoFollowEnable = { false, "AutoFollow", "Enable", false, [this](bool val) { SyncAutoFollow(); } };
	ZealSetting<float> AutoFollowDistance = { 15.f, "AutoFollow", "Distance", false, [this](bool val) { SyncAutoFollow(); } };
	patches();
	void fonts();
private:
	void SetBrownSkeletons();
	void SyncAutoFollow(bool first_boot = false);
};

