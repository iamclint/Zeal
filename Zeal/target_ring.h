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
	void callback_initui();
	void set_enabled(bool enable);
	void set_pct(float pct);
	void set_indicator(bool enable);
	void render_ring(Vec3 position, float size, DWORD color);
	bool enabled;
	bool attack_indicator;
	void render_ring_with_texture(Vec3 pos, float size, DWORD color, IDirect3DTexture8* texture, float rotationAngle);
	TargetRing(class ZealService* zeal, class IO_ini* ini);
	~TargetRing();

	// New getter method for ring_pct
	float get_ring_pct() const { return ring_pct; }

private:
	float ring_pct;
	std::vector<RenderState> render_states;
	void load_ini(class IO_ini* ini);
	void store_render_states();
	void reset_render_states();
	IDirect3DTexture8* targetRingTexture;

};


