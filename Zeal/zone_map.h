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

	void set_enabled(bool enable);
	void callback_render();

private:
	static constexpr int kInvalidZoneId = 0;
	static constexpr int kDefaultRectTop = 25;
	static constexpr int kDefaultRectLeft = 25;
	static constexpr int kDefaultRectBottom = 325;
	static constexpr int kDefaultRectRight = 325;

	static constexpr DWORD MapVertexFvfCode = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	struct MapVertex {
		float x, y, z, rhw;  // Position coordinates and rhw (D3DFVF_XYZRHW).
		D3DCOLOR color; // Color from the D3DFVF_DIFFUSE flag.
	};

	bool parse_command(const std::vector<std::string>& args);
	void parse_rect(const std::vector<std::string>& args);
	void parse_marker(const std::vector<std::string>& args);
	void load_ini(class IO_ini* ini);
	void reset();
	void release_resources();
	void load_map();
	void render_map();
	int render_update_position_buffer();
	bool set_map_rect(int top, int left, int bottom, int right);
	void set_marker(int y, int x, int size=-1);  // size == 0 disables, -1 == default.

	bool enabled = false;
	int zone_id = kInvalidZoneId;
	Vec3 position = Vec3(0, 0, 0);
	Vec3 delta_position = Vec3(0, 0, 0);
	float heading = 0;

	float scale_factor = 0;  // Conversion factors for map to window coordinates.
	float offset_x = 0;
	float offset_y = 0;
	int map_rect_top = kDefaultRectTop;
	int map_rect_left = kDefaultRectLeft;
	int map_rect_bottom = kDefaultRectBottom;
	int map_rect_right = kDefaultRectRight;

	int line_count = 0;  // # of primitives in line buffer.
	int line_buffer_size = 0;
	IDirect3DVertexBuffer8* line_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* position_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* marker_vertex_buffer = nullptr;
};


