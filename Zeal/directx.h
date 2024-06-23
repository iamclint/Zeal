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
	IDirect3DDevice8* GetDevice();
	directx();
private:
	IDirect3DDevice8* device;
	void update_device();
};

