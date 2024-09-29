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
	void set_rotation_speed(float speed);
	void set_rotation_match(bool enable);
	void set_segments(int segments);
	void set_texture(std::string name);
	void set_size(float size);
	void options_opened();

	void load_texture(const std::string& filename);
	void render_ring(Vec3 pos, float size, DWORD color, IDirect3DTexture8* texture, float rotationAngle);
	TargetRing(class ZealService* zeal, class IO_ini* ini);
	~TargetRing();

	bool get_enabled();
	bool get_indicator();
	bool get_rotation_match();
	float get_pct();
	float get_rotation_speed();
	float get_size();
	int get_segments();
	std::string get_texture();
	
private:
	float inner_percent=0.50f;
	bool enabled = false;
	bool attack_indicator = false;
	float rotation_speed = 1.0f;
	bool rotate_match_heading = false;
	float outer_size = 10.f;
	int num_segments = 128;  // Adjust for smoothness of the ring
	std::string texture_name;
	std::vector<RenderState> render_states;
	void save_ini();
	void load_ini();
	void setup_render_states();
	void reset_render_states();


	IDirect3DTexture8* targetRingTexture;

};


