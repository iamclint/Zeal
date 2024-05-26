#pragma once
#include "vectors.h"
#include "d3dx8/d3d8.h"
#include "d3dx8/d3d8types.h"
#include "d3dx8/d3dx8math.h"
class directx
{
public:
	bool WorldToScreen(Vec3 worldPos, Vec2& screenPos);
	Vec2 GetScreenRect();
	IDirect3DDevice8* device;
	directx();
private:
	void update_device();
};

