#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "directx.h"
#include "ZealSettings.h"
#include "EqFunctions.h"

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

struct BaseVertex {
	float x, y, z;  // Position coordinates
	BaseVertex() = default;
	BaseVertex(float x, float y, float z) : x(x), y(y), z(z) {}
};
struct TexturedVertex : public BaseVertex {
	float u, v;     // Texture coordinates
	D3DCOLOR color; // Color for solid vertices
	TexturedVertex() = default;
	TexturedVertex(float x, float y, float z, float u, float v)
		: BaseVertex(x, y, z), u(u), v(v) {}
};

struct SolidVertex : public BaseVertex {
	D3DCOLOR color; // Color for solid vertices
	SolidVertex() = default;
	SolidVertex(const TexturedVertex& tv) { x = tv.x; y = tv.y, z = tv.z, color = tv.color; }
	SolidVertex(const TexturedVertex& tv, float _z) { x = tv.x; y = tv.y, z = _z, color = tv.color; }
	SolidVertex(float x, float y, float z, D3DCOLOR color)
		: BaseVertex(x, y, z), color(color) {}
};


class TargetRing
{
public:
	void callback_render();
	void callback_initui();

	std::vector<std::string> get_available_textures() const;
	void load_texture(const std::string& filename);
	void render_ring(Vec3 pos, float size, DWORD color, IDirect3DTexture8* texture, float rotationAngle);
	TargetRing(class ZealService* zeal, class IO_ini* ini);
	~TargetRing();

	ZealSetting<bool> enabled = { false, "TargetRing", "Enabled", true, [](bool val) { Zeal::EqGame::print_chat("Target ring is %s", val ? "Enabled" : "Disabled"); } };
	ZealSetting<bool> attack_indicator = { false, "TargetRing", "AttackIndicator", true };
	ZealSetting<bool> rotate_match_heading = { false, "TargetRing", "MatchHeading", true };
	ZealSetting<bool> use_cone = { false, "TargetRing", "Cone", true };
	ZealSetting<float> inner_percent = { 0.50f, "TargetRing", "InnerSize", true };
	ZealSetting<float> outer_size = { 10.0f, "TargetRing", "Size", true };
	ZealSetting<float> rotation_speed = { 1.0f, "TargetRing", "RotateSpeed", true };
	ZealSetting<float> flash_speed = { 1.0f, "TargetRing", "FlashSpeed", true };
	ZealSetting<float> transparency = { 0.7f, "TargetRing", "Opacity", true };
	ZealSetting<int> num_segments = { 128, "TargetRing", "Segments", true };
	ZealSetting<std::string> texture_name = { "None", "TargetRing", "Texture", true, [this](std::string name) { load_texture(name); }};

	void setup_render_states();
	void reset_render_states();

private:
	void drawVertices(Vec3 pos, DWORD vertex_count, IDirect3DTexture8* texture, D3DXMATRIX worldMatrix, SolidVertex* solid_vertices, TexturedVertex* texture_vertices);
	std::vector<RenderState> render_states;
	IDirect3DTexture8* targetRingTexture=nullptr;
};


