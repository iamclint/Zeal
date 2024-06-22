#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "directx.h"

enum DxStateType_
{
	DxStateType_Render,
	DxStateType_Texture
};
	
struct RenderState
{
	DWORD value;
	DWORD state;
	DxStateType_ type;
	RenderState(IDirect3DDevice8* device, DWORD state, DxStateType_ type);
	RenderState() = default;
};

class TargetRing
{
public:
	void callback_render();
	void set_enabled(bool enable);
	void render_ring(Vec3 position, float size, DWORD color);
	bool enabled;
	TargetRing(class ZealService* zeal, class IO_ini* ini);
	~TargetRing();
private:
	std::vector<RenderState> render_states;
	void store_render_states();
	void reset_render_states();
};


