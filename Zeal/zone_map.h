#pragma once
#include "hook_wrapper.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "directx.h"
#include "vectors.h"
#include "zone_map_data.h"
#include "bitmap_font.h"
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>


class ZoneMap
{
public:
	struct MapVertex {
		float x, y, z;  // Position coordinates and rhw (D3DFVF).
		D3DCOLOR color; // Color from the D3DFVF_DIFFUSE flag.
	};

	struct AlignmentType { enum e : int { kLeft = 0, kCenter, kRight, kFirst = kLeft, kLast = kRight }; };
	struct BackgroundType { enum e : int { kClear = 0, kDark, kLight, kTan, kFirst = kClear, kLast = kTan }; };
	struct LabelsMode { enum e : int { kOff = 0, kSummary, kAll, kMarkerOnly, kFirst = kOff, kLast = kMarkerOnly }; };
	struct MapDataMode { enum e : int { kInternal = 0, kBoth, kExternal, kFirst = kInternal, kLast = kExternal }; };
	struct ShowGroupMode { enum e : int { kOff = 0, kMarkers, kNumbers, kNames, kFirst = kOff, kLast = kNames }; };

	static constexpr int kMaxNameLength = 20;  // Name buffer is >= 30.
	static constexpr int kMaxGridPitch = 2500;
	static constexpr float kMaxPositionSize = 0.05f;  // In fraction of screen size.
	static constexpr float kMaxMarkerSize = 0.05f;

	ZoneMap(class ZealService* zeal, class IO_ini* ini);
	~ZoneMap();

	// UI control interface.
	// Rect and sizes are in percentages of screen dimensions (0 to 100).
	// Setting update_default stores to the ini. All trigger a ui_options update.
	bool is_enabled() const { return enabled; }
	void set_enabled(bool enable, bool update_default = false);
	void set_external_enable(bool enabled, bool update_default = false);
	void set_interactive_enable(bool enable, bool update_default = true);
	bool set_show_group_mode(int new_mode, bool update_default = true);
	void set_show_raid(bool enable, bool update_default = true);
	void set_show_all_names_override(bool flag);  // Override to enable showing group and raid names.
	void set_show_grid(bool enable, bool update_default = true);
	bool set_grid_pitch(int new_pitch, bool update_default = true);
	bool set_ring_radius(int new_radius, bool update_default = true);
	bool set_name_length(int new_length, bool update_default = true);
	bool set_map_data_mode(int new_mode, bool update_default = true);
	bool set_background(int new_state, bool update_default = true); // [clear, dark, light, tan]
	bool set_background_alpha(int percent, bool update_default = true);
	bool set_faded_zlevel_alpha(int percent, bool update_default = true);
	bool set_alignment(int new_state, bool update_default = true); // [left, center, right]
	bool set_labels_mode(int new_mode, bool update_default = true);  // [off, summary, all]
	bool set_map_top(int top_percent, bool update_default = true, bool preserve_height = true);
	bool set_map_left(int left_percent, bool update_default = true, bool preserve_width = true);
	bool set_map_bottom(int bottom_percent, bool update_default = true);
	bool set_map_height(int height_percent, bool update_default = true);
	bool set_map_right(int right_percent, bool update_default = true);
	bool set_map_width(int width_percent, bool update_default = true);
	bool set_position_size(int new_size_percent, bool update_default = true);
	bool set_marker_size(int new_size_percent, bool update_default = true);
	bool set_zoom(int zoom_percent);  // Note: 100% = 1x.
	bool set_font(std::string font_name, bool update_default = true);

	bool is_external_enabled() const { return external_enabled; }
	bool is_show_raid_enabled() const { return map_show_raid; }
	bool is_show_grid_enabled() const { return map_show_grid; }
	bool is_show_all_names_override() const { return map_show_all_names_override;}
	bool is_interactive_enabled() const { return map_interactive_enabled; }
	int get_show_group_mode() const { return map_show_group_mode; }
	int get_name_length() const { return map_name_length; }
	int get_grid_pitch() const { return map_grid_pitch; }
	int get_map_data_mode() const { return static_cast<int>(map_data_mode); }
	int get_background() const { return static_cast<int>(map_background_state); }
	int get_background_alpha() const { return static_cast<int>(map_background_alpha * 100 + 0.5f); }
	int get_faded_zlevel_alpha() const { return static_cast<int>(map_faded_zlevel_alpha * 100 + 0.5f); }
	int get_alignment() const { return static_cast<int>(map_alignment_state); }
	int get_labels_mode() const { return static_cast<int>(map_labels_mode); }
	int get_map_top() const { return static_cast<int>(map_rect_top * 100 + 0.5f); }
	int get_map_left() const { return static_cast<int>(map_rect_left * 100 + 0.5f); }
	int get_map_bottom() const { return static_cast<int>(map_rect_bottom * 100 + 0.5f); }
	int get_map_height() const { return static_cast<int>((map_rect_bottom - map_rect_top) * 100 + 0.5f); }
	int get_map_right() const { return static_cast<int>(map_rect_right * 100 + 0.5f); }
	int get_map_width() const { return static_cast<int>((map_rect_right - map_rect_left) * 100 + 0.5f); }
	int get_position_size() const { return static_cast<int>(position_size / kMaxPositionSize * 100 + 0.5f); }
	int get_marker_size() const { return static_cast<int>(marker_size / kMaxMarkerSize * 100 + 0.5f); }
	int get_zoom() const { return static_cast<int>(zoom_factor * 100 + 0.5f); }
	std::string get_font() const { return font_filename; }
	std::vector<std::string> get_available_fonts() const;

	void toggle_background();
	void toggle_zoom();
	void toggle_labels();
	void toggle_level_up();
	void toggle_level_down();

	// Adds a temporary "dynamic" text label to the current map with optional timeout that can be
	// used to mark locations or track player names. Note that y and x are in world (loc) coordinates.
	void add_dynamic_label(const std::string& label, int loc_y, int loc_x,
		unsigned int duration_ms = 0, D3DCOLOR font_color = D3DCOLOR_XRGB(250, 250, 51));

	// Private methods exposed for callback use only.
	void process_mouse_wheel(int16_t mouse_delta, uint16_t flags, int16_t x, int16_t y);
	void process_left_mouse_button_down(int16_t x, int16_t y);
	void process_right_mouse_button_down(int16_t x, int16_t y);
	void process_wheel_mouse_button_down(int16_t x, int16_t y);

private:
	enum class LabelType { Normal, AddMarker, PositionLabel };

	// DynamicLabels are added using add_label() and are in addition to the static map data labels.
	struct DynamicLabel {
		std::string label;  // Label to display.
		int loc_y = 0;  // Game location y.
		int loc_x = 0;  // Game location x.
		ULONGLONG timeout = 0;  // 0 disables the timeout.
		D3DCOLOR color = 0;
	};

	struct Marker {
		int loc_y = 0;
		int loc_x = 0;
		std::string label;
	};

	struct CustomMapData {
		std::vector<ZoneMapLine> lines;  // Contains heap memory for zone_map_data.
		std::vector<ZoneMapLabel> labels;
		std::list<std::string> label_strings;  // Used as heap for const char* in labels.
		std::vector<ZoneMapLevel> levels;
		std::unique_ptr<ZoneMapData> zone_map_data;
	};

	static constexpr int kInvalidZoneId = 0;
	static constexpr int kInvalidScreenValue = 0x7fff;  // EQ game sets mouse abs to this when not focused.
	static constexpr int kInvalidPositionValue = 0x7fff;
	static constexpr int kDefaultGridPitch = 1000;
	static constexpr int kDefaultNameLength = 5;
	static constexpr float kDefaultBackgroundAlpha = 0.5f;
	static constexpr float kDefaultFadedZLevelAlpha = 0.2f;
	static constexpr float kDefaultRectTop = 0.1f;
	static constexpr float kDefaultRectLeft = 0.1f;
	static constexpr float kDefaultRectBottom = 0.4f;
	static constexpr float kDefaultRectRight = 0.4f;
	static constexpr float kDefaultPositionSize = 0.01f;
	static constexpr float kDefaultMarkerSize = 0.02f;

	// UI and parser methods.
	// Rect and sizes are in fractions of screen dimensions(0.f to 1.f).
	bool parse_command(const std::vector<std::string>& args);
	bool parse_shortcuts(const std::vector<std::string>& args);
	void parse_external(const std::vector<std::string>& args);
	void parse_rect(const std::vector<std::string>& args);
	void parse_size(const std::vector<std::string>& args);
	void parse_alignment(const std::vector<std::string>& args);
	void parse_marker(const std::vector<std::string>& args);
	void parse_background(const std::vector<std::string>& args);
	void parse_zoom(const std::vector<std::string>& args);
	void parse_labels(const std::vector<std::string>& args);
	void parse_level(const std::vector<std::string>& args);
	void parse_map_data_mode(const std::vector<std::string>& args);
	void parse_show_group(const std::vector<std::string>& args);
	void parse_show_raid(const std::vector<std::string>& args);
	void parse_show_zone(const std::vector<std::string>& args);
	void parse_grid(const std::vector<std::string>& args);
	void parse_ring(const std::vector<std::string>& args);
	void parse_font(const std::vector<std::string>& args);
	void parse_poi(const std::vector<std::string>& args);
	bool search_poi(const std::string& search);
	void set_marker(int y, int x, const char* label = nullptr);
	void clear_markers(bool erase_list = true);
	bool set_map_rect(float top, float left, float bottom, float right, bool update_default = false);
	bool set_level(int level);  // Set to 0 to show all levels.
	bool set_show_zone_id(const std::string& zone_name);
	void update_ui_options();

	void load_ini(class IO_ini* ini);
	void save_ini();
	void dump();

	void callback_zone();
	void callback_render();
	void callback_dx_reset();
	void reset_zone_state();

	// The following methods execute as part of callback_render().
	void render_release_resources(bool release_font = true);
	void render_update_viewport(IDirect3DDevice8& device);
	void render_update_transforms(const ZoneMapData& zone_map_data);
	void render_load_map(IDirect3DDevice8& device, const ZoneMapData& zone_map_data);
	void render_load_font(IDirect3DDevice8& device);
	void render_load_labels(IDirect3DDevice8& device, const ZoneMapData& zone_map_data);
	void render_map(IDirect3DDevice8& device);
	void render_background(IDirect3DDevice8& device);
	void render_grid(IDirect3DDevice8& device);
	void render_markers(IDirect3DDevice8& device);
	void render_positions(IDirect3DDevice8& device);
	void render_group_member_labels(IDirect3DDevice8& device);
	void render_raid_member_labels(IDirect3DDevice8& device);
	void render_update_marker_buffer(IDirect3DDevice8& device);
	void render_labels(IDirect3DDevice8& device);
	void render_label_text(const char* label, int map_y, int map_x, D3DCOLOR font_color,
		LabelType label_type = LabelType::Normal, Vec2 offset_pixels = { 0,0 });
	void render_handle_cursor(IDirect3DDevice8& device);
	void enable_autocenter();
	bool is_autocenter_enabled() const;
	void update_mouse();
	void reset_mouse();
	void hide_cursor();
	void restore_cursor();
	std::vector<ZoneMap::MapVertex> calculate_grid_vertices(const ZoneMapData& zone_map_data) const;
	void add_position_marker_vertices(float map_y, float map_x, float heading, float size,
		D3DCOLOR color, std::vector<MapVertex>& vertices) const;
	void add_group_member_position_vertices(std::vector<MapVertex>& vertices) const;
	void add_raid_member_position_vertices(std::vector<MapVertex>& vertices) const;
	void add_raid_marker_vertices(const Vec3& position_loc, float size,
		D3DCOLOR color, std::vector<MapVertex>& vertices) const;
	void add_ring_vertices(std::vector<MapVertex>& vertices) const;
	float convert_size_fraction_to_model(float sizes_fraction) const;
	float scale_pixels_to_model(float pixels) const;
	Vec3 transform_matrix(const D3DXMATRIX& matrix, const Vec3& vec) const;
	Vec3 transform_model_to_world(const Vec3& model) const;
	Vec3 transform_model_to_screen(const Vec3& model) const;
	Vec3 transform_world_to_model(const Vec3& world) const;
	Vec3 transform_screen_to_model(float x, float y, float z = 1.f) const;
	D3DCOLOR get_background_color() const;
	D3DCOLOR render_get_line_color_and_opacity(const ZoneMapLine& line, int position_z, int level_id) const;
	void update_succor_label();
	bool is_zlevel_change() const;

	const ZoneMapData* get_zone_map(int zone_id);
	int find_zone_id(const std::string& zone_name) const;
	void add_map_data_from_internal(const ZoneMapData& internal_map, CustomMapData& map_data);
	bool add_map_data_from_file(const std::string& filename, CustomMapData& map_data);
	void assemble_zone_map(const char* zone_name, CustomMapData& map_data);

	// EQWND support methods
	bool ui_is_visible() const;
	void ui_hide();
	bool ui_show();
	void ui_clean();
	void ui_deactivate();
	void ui_init();
	void ui_synchronize_window();

	bool enabled = false;
	bool external_enabled = false;  // External map window enable and sizes.
	bool map_interactive_enabled = false;  // Supports some mouse/cursor operations.
	bool map_show_grid = false;
	int map_grid_pitch = kDefaultGridPitch;  // Pitch when grid is visible.
	int map_ring_radius = 0;
	int map_name_length = kDefaultNameLength;  // Number of characters in name labels.
	bool map_show_raid = false;
	bool map_show_all_names_override = false;  // Overrides modes to show names of all visible members.
	BackgroundType::e map_background_state = BackgroundType::kClear;
	float map_background_alpha = kDefaultBackgroundAlpha;
	float map_faded_zlevel_alpha = kDefaultFadedZLevelAlpha;
	AlignmentType::e map_alignment_state = AlignmentType::kFirst;
	LabelsMode::e map_labels_mode = LabelsMode::kOff;
	ShowGroupMode::e map_show_group_mode = ShowGroupMode::kOff;
	MapDataMode::e map_data_mode = MapDataMode::kInternal;
	int zone_id = kInvalidZoneId;
	int show_zone_id = kInvalidZoneId;  // Override to show zone indepent of current location.
	std::vector<Marker> markers_list;
	bool always_align_to_center = false;
	int map_level_index = 0;
	int dynamic_labels_zone_id = kInvalidZoneId;
	std::vector<DynamicLabel> dynamic_labels_list;  // Optional temporary labels.
	std::unordered_map<int, std::unique_ptr<CustomMapData>> map_data_cache;

	D3DVIEWPORT8 viewport = {};  // On-screen coordinates of viewport.
	LONG max_viewport_width = 0;  // Full game window (ignores /viewport) or screen size (external).
	LONG max_viewport_height = 0;
	D3DXMATRIX mat_model2world;  // Map model data to viewport centered pixel data.
	D3DXMATRIX mat_world2model;  // Inverse viewport centered pixel back to map model data.
	float zoom_factor = 1.f;
	float map_rect_top = kDefaultRectTop;
	float map_rect_left = kDefaultRectLeft;
	float map_rect_bottom = kDefaultRectBottom;
	float map_rect_right = kDefaultRectRight;
	float clip_max_x = 0;  // Visible (non-clipped) model data limits.
	float clip_min_x = 0;
	float clip_max_y = 0;
	float clip_min_y = 0;
	int clip_max_z = 0;
	int clip_min_z = 0;
	int zlevel_position_z = kInvalidPositionValue;
	float position_size = kDefaultPositionSize;
	float marker_size = kDefaultMarkerSize;
	bool cursor_hidden = false;
	Vec3 manual_position = Vec3(0, 0, 0);
	POINT manual_screen_pt = POINT({ .x = kInvalidScreenValue, .y = kInvalidScreenValue });
	POINT mouse_pt = POINT({ .x = kInvalidScreenValue, .y = kInvalidScreenValue });  // Latest mouse update.
	bool mouse_drag_enabled = false;

	std::vector<const ZoneMapLabel*> labels_list;  // List of pointers to visible map labels.
	ZoneMapLabel succor_label;  // Auto-generated succor label for safe coordinates.
	int line_count = 0;  // # of primitives in line buffer.
	int grid_line_count;  // # of primitives at end of line buffer.
	IDirect3DVertexBuffer8* line_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* position_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* marker_vertex_buffer = nullptr;
	std::string font_filename;
	std::unique_ptr<BitmapFont> bitmap_font;

	// Internal window support
	Zeal::EqUI::EQWND* wnd = nullptr;

	// External window support
	static constexpr wchar_t kExternalWindowClassName[] = L"ZealMapWindowClass";
	static constexpr wchar_t kExernalWindowTitle[] = L"Zeal Map";

	static LRESULT CALLBACK static_external_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK external_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void process_external_window_messages();
	bool create_external_window();
	bool show_external_window();
	void hide_external_window();
	void synchronize_external_window();
	void destroy_external_window();
	bool initialize_d3d_external_window();
	void release_d3d_external_window();
	void release_font();

	HINSTANCE external_hinstance = nullptr;
	HWND external_hwnd = nullptr;
	LPDIRECT3D8 external_d3d = nullptr; // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE8 external_d3ddev = nullptr; // the pointer to the device class
	int external_monitor_top_offset = 0;  // Virtual screen offsets of ext window monitor.
	int external_monitor_left_offset = 0;  // These are to top left corner of client rect.
	int external_monitor_height = 0;  // Client rect height and width.
	int external_monitor_width = 0;
};


