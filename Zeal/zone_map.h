#pragma once
#include "hook_wrapper.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "directx.h"

class ZoneMap
{
public:
	ZoneMap(class ZealService* zeal, class IO_ini* ini);
	~ZoneMap();

	// Settings that update the ini file defaults.
	void set_enabled(bool enable, bool update_default=false);
	// Rect and position are in fractions of screen dimensions (0.f to 1.f).
	bool set_map_rect(float top, float left, float bottom, float right);
	void set_position_default_size(float new_size);
	void set_marker_default_size(float new_size);

	void callback_render();

private:
	static constexpr int kInvalidZoneId = 0;
	static constexpr float kDefaultRectTop = 0.015f;
	static constexpr float kDefaultRectLeft = 0.015f;
	static constexpr float kDefaultRectBottom = 0.35f;
	static constexpr float kDefaultRectRight = 0.25f;
	static constexpr float kDefaultPositionSize = 0.01f;
	static constexpr float kDefaultMarkerSize = 0.02f;

	static constexpr DWORD MapVertexFvfCode = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	struct MapVertex {
		float x, y, z, rhw;  // Position coordinates and rhw (D3DFVF_XYZRHW).
		D3DCOLOR color; // Color from the D3DFVF_DIFFUSE flag.
	};

	bool parse_command(const std::vector<std::string>& args);
	bool parse_shortcuts(const std::vector<std::string>& args);
	void parse_rect(const std::vector<std::string>& args);
	void parse_marker(const std::vector<std::string>& args);
	void set_marker(int y, int x);
	void clear_marker();
	void load_ini(class IO_ini* ini);
	void reset();
	void release_resources();
	void load_map();
	void render_map();
	int render_update_position_buffer();

	bool enabled = false;
	int zone_id = kInvalidZoneId;
	Vec3 position = Vec3(0, 0, 0);
	float heading = 0;

	float scale_factor = 0;  // Conversion factors for map data to screen coordinates.
	float offset_x = 0;
	float offset_y = 0;
	float map_rect_top = kDefaultRectTop;
	float map_rect_left = kDefaultRectLeft;
	float map_rect_bottom = kDefaultRectBottom;
	float map_rect_right = kDefaultRectRight;
	float position_size = kDefaultPositionSize;
	float marker_size = kDefaultMarkerSize;

	int line_count = 0;  // # of primitives in line buffer.
	int line_buffer_size = 0;
	IDirect3DVertexBuffer8* line_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* position_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* marker_vertex_buffer = nullptr;
};


