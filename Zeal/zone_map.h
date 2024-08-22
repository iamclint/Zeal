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

	// UI control interface.
	// Rect and position are in fractions of screen dimensions (0.f to 1.f).
	bool is_enabled() const { return enabled; }
	void set_enabled(bool enable, bool update_default = false);
	bool set_background(int new_state, bool update_default = false);
	bool set_map_rect(float top, float left, float bottom, float right, bool update_default = true);
	void set_position_default_size(float new_size);
	void set_marker_default_size(float new_size);
	void toggle_background();

	void callback_render();

private:
	static constexpr int kInvalidZoneId = 0;
	static constexpr int kBackgroundStates = 4;  // 0 = clear.
	static constexpr float kDefaultRectTop = 0.1f;
	static constexpr float kDefaultRectLeft = 0.1f;
	static constexpr float kDefaultRectBottom = 0.4f;
	static constexpr float kDefaultRectRight = 0.4f;
	static constexpr float kDefaultPositionSize = 0.01f;
	static constexpr float kDefaultMarkerSize = 0.02f;

	// UI and parser methods.
	bool parse_command(const std::vector<std::string>& args);
	bool parse_shortcuts(const std::vector<std::string>& args);
	void parse_rect(const std::vector<std::string>& args);
	void parse_marker(const std::vector<std::string>& args);
	void parse_background(const std::vector<std::string>& args);
	void parse_zoom(const std::vector<std::string>& args);
	void set_marker(int y, int x);
	void clear_marker();
	bool set_zoom(int zoom_percent);
	void load_ini(class IO_ini* ini);
	void dump();

	// The following methods execute as part of callback_render().
	void render_release_resources();
	void render_load_map();
	void render_map();
	int render_update_position_buffer();
	void render_update_marker_buffer();

	bool enabled = false;
	int map_background_state = 0;
	int zone_id = kInvalidZoneId;
	int marker_zone_id = kInvalidZoneId;
	int marker_x = 0;
	int marker_y = 0;

	float scale_factor = 0;  // Conversion factors for map data to screen coordinates.
	float zoom_factor = 1.f;
	float offset_x = 0;
	float offset_y = 0;
	float map_rect_top = kDefaultRectTop;
	float map_rect_left = kDefaultRectLeft;
	float map_rect_bottom = kDefaultRectBottom;
	float map_rect_right = kDefaultRectRight;
	float position_size = kDefaultPositionSize;
	float marker_size = kDefaultMarkerSize;

	int line_count = 0;  // # of primitives in line buffer.
	IDirect3DVertexBuffer8* line_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* position_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* marker_vertex_buffer = nullptr;
};


