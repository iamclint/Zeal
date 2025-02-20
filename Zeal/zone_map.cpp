#include "zone_map.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include "zone_map_data.h"
#include "bitmap_font.h"
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#include <fstream>
#include <string>


// Possible enhancements and issues:
// - Look into intermittent z-depth clipping due to walls or heads/faces
// - External map window issues:
//   - Was unable to transfer focus to the main window for keyboard input, so using an internal
//     command router for hotkeys. This does not allow typing chat messages currently.
// - Interactive mode:
//   - TAKP eqgame.exe code appears to ignore the middle mouse button even though callbacks exist
//   - Ideally would render map into the window background at the proper time, but that
//     requires more investigation around the deferred calls. As a result, the border can
//     be on top while the map render is hidden behind other UI elements.
//   - Dragging a window off the left edge ignores the ui_synchronize() move and invalidate and
//     thus keeps setting a large negative client Left, causing flashing and ugly sizing
//   - Dragging a window up above top limit causes it to jump to lower right corner. Also 
//     happens to chat windows, so it's an EQ client issue.
//   - ContextMenu: Disable or support Background Texture and Tint Color options
//   - Add zone name to the window title. Maybe level or other info as well.
//   - Consider adding an option for minimize to go to a 320x320 instead of fully minimizing
// - Old (Stone) UI: Not supported. Map remains disabled since wnd is nullptr.

// Implementation notes:
//
// Coordinate systems:
//  - The EQ game world coordinate system has "x" as up and "y" as left on the screen.
//  - The map data has the same scale as the game world but "x" is right and "y" is down.
//  - The output screen (post viewport transformation) has "x" as right and "y" as down.
//  - Since the map data matches the output screen and it is more intuitive to use, the
//    EQ game world coordinates (Position, location values) are translated to it when
//    used by flipping and negating (polarity) the y and x values.
//    - Variables called "position" or "loc" are typically in EQ coordinates (unconverted)
//  - The map world coordinate system is pixel scaled and viewport centered (see below)
//
// DirectX pipeline:
//  - The map size variables (map_rect_*) are used to calculate the viewport, which is
//    the on-screen draw window (clips to within it and has the screen coordinates)
//  - When using transformed vertices (RHW), the values are final screen x,y coordinates
//    (not relative to viewport offsets)
//  - When using the 3D rendering pipeline, there are four vertex transformations:
//     - world: map data in "model" space to zoomed and centered pixel world space
//       - scales and offsets map data coordinates to pixel scale with the (0,0)
//         region in the center of the viewport
//     - view: use identity (combined into the world matrix)
//     - projection: ortho that just pre-warps by 2/width and 2/height for viewport
//     - viewport: fixed transformation to fill viewport
//       - Maps input +1 and -1 x and y scales to viewport width/2 and height/2
//       - Maps 0,0 to the center of the viewport
//
//  SidlScreenWnd / EQWND tie-in:
//   - callback_zone: Called to reset resource state before entering a new zone
//     - character is known at this point
//   - callback_init_ui: Called after character selection before entering the first zone.
//       - Note: callback_zone happens before callback_init_ui for the first zone
//     - Allocates UI resources (including from sidl files)
//   - callback_deactivate_ui: Called upon exiting zone (and elsewhere)
//     - Performs Deactivate callback on all UI pieces
//   - callback_clean_ui: Called before returning to character selection (and elsewhere)
//     - Releases / deconstructs and nulls UI resources

namespace {

// Constants used in DirectX resource allocation.
static constexpr int kBackgroundCount = 2;  // Two triangles in split mode.
static constexpr int kBackgroundVertices = 3 + (kBackgroundCount - 1);  // Two triangles using split.
static constexpr int kMarkerCount = 4;  // Four triangles per marker.
static constexpr int kPositionCount = 2;  // Two triangles.
static constexpr int kPositionVertices = kPositionCount * 3; // Fixed triangle list.
static constexpr int kRaidMaxMembers = Zeal::EqStructures::RaidInfo::kRaidMaxMembers;
static constexpr int kRaidPositionVertices = 3; // Simple single triangle default option.
static constexpr int kMaxDynamicLabels = 10;
static constexpr int kRingLineSegments = 72;  // Every 5 degrees.
static constexpr int kRingVertices = kRingLineSegments + 1; // N + 1 for D3DPT_LINESTRIP
static constexpr int kMaxNonAllyTriangles = 500;  // Markers use 1 or 2 triangles each.
static constexpr int kPositionBufferSize = sizeof(ZoneMap::MapVertex) * (kPositionVertices
       * (EQ_NUM_GROUP_MEMBERS + 1) + kPositionVertices * kRaidMaxMembers +
        kRaidPositionVertices * kMaxNonAllyTriangles + kRingVertices);

static constexpr DWORD kMapVertexFvfCode = (D3DFVF_XYZ | D3DFVF_DIFFUSE);


static constexpr int kWinMinSize = 160;  // Minimum size for window dimensions.

// Note: Zoom factors are hard-coded in options combobox. Must keep in sync manually.
static constexpr int kNumDefaultZoomFactors = 4;
static constexpr float kDefaultZoomFactors[kNumDefaultZoomFactors] = { 1.f, 2.f, 4.f, 8.f };

static D3DCOLOR get_target_color() {
    const int kTargetColorIndex = 18; // NamePlate::ColorIndex::Target
    return ZealService::get_instance()->ui->options->GetColor(kTargetColorIndex);
}

}  // namespace


// Releases the manually managed DirectX related resources.
void ZoneMap::render_release_resources(bool flush_font) {
    if (line_vertex_buffer) {
        line_vertex_buffer->Release();
        line_vertex_buffer = nullptr;
    }
    if (position_vertex_buffer) {
        position_vertex_buffer->Release();
        position_vertex_buffer = nullptr;
    }
    if (marker_vertex_buffer) {
        marker_vertex_buffer->Release();
        marker_vertex_buffer = nullptr;
    }
    if (flush_font)
        release_font();
}

// Configure the viewport and max dimensions based on render target and mode.
void ZoneMap::render_update_viewport(IDirect3DDevice8& device) {
    // Use a 'custom' viewport for the map so that it ignores the games /viewport
    // settings and can draw outside of it. The rect/size scalefactors are relative
    // to the entire game window (render target, ignores /viewport).
    IDirect3DSurface8* surface;
    device.GetRenderTarget(&surface);
    D3DSURFACE_DESC description;
    if (surface) {
        surface->GetDesc(&description);
        surface->Release();  // Decrements a reference counter.
    }
    else {
        description.Width = kWinMinSize;  // "safe" fallback values.
        description.Height = kWinMinSize;
    }

    // Enable full-window mode in external window mode. Set the viewport to match
    // the render target size. Note max_viewport_width & height are updated elsewhere.
    if (external_enabled) {
        viewport = D3DVIEWPORT8{ .X = 0, .Y = 0,
            .Width = description.Width, .Height = description.Height,
            .MinZ = 0.0f, .MaxZ = 1.0f };
        return;
    }

    max_viewport_width = description.Width;  // Used for constant scaling of markers.
    max_viewport_height = description.Height;
    
    // In internal interactive (windowed) mode, update the viewport to match the window but
    // constrain it to all fit within the render surface.
    if (map_interactive_enabled) {
        Zeal::EqUI::CXRect rect;
        Zeal::EqGame::EqGameInternal::CXWndGetClientRect(wnd, 0, &rect);
        int viewport_x = max(0, min(max_viewport_width - kWinMinSize, rect.Left));
        int viewport_y = max(0, min(max_viewport_height - kWinMinSize, rect.Top));
        if (manual_on_move_trigger) {
            manual_on_move_trigger = false;
            // Negated and swapped. Modify position since it is floating point and reduces drift.
            manual_position.y += scale_pixels_to_model(static_cast<float>(viewport_x - static_cast<int>(viewport.X)));
            manual_position.x += scale_pixels_to_model(static_cast<float>(viewport_y - static_cast<int>(viewport.Y)));
        }

        viewport.X = viewport_x;
        viewport.Y = viewport_y;
        // Handle an issue where a dragged window will continue to have the client rect go << -1
        // even after the synchronize_window()'s move call. In that case, just decrement the 
        // width by 1 each iteration. Height isn't an issue since can't drag it above screen.
        if (rect.Left < -1)
            viewport.Width = max(kWinMinSize,
                min(max_viewport_width - static_cast<int>(viewport.X), rect.Right - rect.Left - 1));
        else
            viewport.Width = max(kWinMinSize, min(max_viewport_width, rect.Right) - static_cast<int>(viewport.X));
        viewport.Height = max(kWinMinSize, min(max_viewport_height, rect.Bottom) - static_cast<int>(viewport.Y));
        ui_synchronize_window();  // Force the window to resize if necessary.
        return;
    }
   
    // Internal non-active mode.
    // Handle startup and generally just continue to keep in sync if wnd client size is known.
    if (!wnd->IsMinimized)
        Zeal::EqGame::EqGameInternal::CXWndGetClientRect(wnd, 0, &map_client_rect);

    // Sanity clamp and apply to viewport.
    viewport.X = max(0, min(max_viewport_width - kWinMinSize, map_client_rect.Left));
    viewport.Y = max(0, min(max_viewport_height - kWinMinSize, map_client_rect.Top));
    viewport.Width = max(kWinMinSize, min(max_viewport_width, map_client_rect.Right) - static_cast<int>(viewport.X));
    viewport.Height = max(kWinMinSize, min(max_viewport_height, map_client_rect.Bottom) - static_cast<int>(viewport.Y));
    viewport.MinZ = 0.0f;
    viewport.MaxZ = 1.0f;
}

// Synchronize the UI window client rect with the viewport.
void ZoneMap::ui_synchronize_window() {
    if (external_enabled || !map_interactive_enabled)
        return;

    Zeal::EqUI::CXRect rect;
    Zeal::EqGame::EqGameInternal::CXWndGetClientRect(wnd, 0, &rect);
    // Keep the internal window size in sync by setting it to the target client rect size.
    map_client_rect.Top = viewport.Y;
    map_client_rect.Left = viewport.X;
    map_client_rect.Bottom = viewport.Y + viewport.Height;
    map_client_rect.Right = viewport.X + viewport.Width;
    if (viewport.X == rect.Left && viewport.Y == rect.Top && viewport.Width == (rect.Right - rect.Left) &&
            viewport.Height == rect.Bottom - rect.Top)
        return;

    // Add the border padding.
    rect.Left = rect.Left - wnd->Location.Left;
    rect.Top = rect.Top - wnd->Location.Top;
    rect.Right = rect.Right - wnd->Location.Right;
    rect.Bottom = rect.Bottom - wnd->Location.Bottom;
    Zeal::EqGame::EqGameInternal::CXWndMoveAndInvalidate(wnd, 0, viewport.X - rect.Left, viewport.Y - rect.Top,
        viewport.X + viewport.Width - rect.Right, viewport.Y + viewport.Height - rect.Bottom);
}

// Sets the primary mat_model2world matrix and x,y clip limits in model space.
void ZoneMap::render_update_transforms(const ZoneMapData& zone_map_data) {

    // Set the default 100% zoom scale factor (model to screen) so that the zone
    // map data fits within the viewport. The map will be constrained by either
    // the height or width depending on aspect ratio.

    // Reduce the viewport width and height by 1 as extra padding for the edge lines.
    const float viewport_width = static_cast<float>(viewport.Width - 1);
    const float viewport_height = static_cast<float>(viewport.Height - 1);
    const float rect_left = -viewport_width * 0.5f;
    const float rect_right = +viewport_width * 0.5f;
    const float rect_top = -viewport_height * 0.5f;
    const float rect_bottom = +viewport_height * 0.5f;

    const float kPadding = 1.f;  // Small map data padding to prevent edge clipping.
    const float map_width = (zone_map_data.max_x - zone_map_data.min_x + 2 * kPadding);
    const float map_height = (zone_map_data.max_y - zone_map_data.min_y + 2 * kPadding);
    const float scale_factor_x = (rect_right - rect_left) / map_width;
    const float scale_factor_y = (rect_bottom - rect_top) / map_height;
    const float scale_factor = min(scale_factor_x, scale_factor_y) * zoom_factor;

    const Vec3 position = (show_zone_id == kInvalidZoneId) ? Zeal::EqGame::get_self()->Position :
        Vec3(-static_cast<float>(zone_map_data.min_y + zone_map_data.max_y) / 2,  // Center on map, flipping y and x.
            -static_cast<float>(zone_map_data.min_x + zone_map_data.max_x) / 2,
            static_cast<float>(zone_map_data.min_z + zone_map_data.max_z) / 2);
    const float position_y = -position.x; // Note position is y,x,z.
    const bool full_height = map_height * scale_factor < viewport_height;
    const bool align_top = full_height ||
        (position_y - zone_map_data.min_y) * scale_factor < viewport_height * 0.5f;
    const bool align_bottom = (zone_map_data.max_y - position_y) * scale_factor < viewport_height * 0.5f;

    float offset_y = 0;
    if (align_top && !always_align_to_center)
        offset_y = rect_top - (zone_map_data.min_y - kPadding) * scale_factor;
    else if (align_bottom && !always_align_to_center)
        offset_y = rect_bottom - (zone_map_data.max_y + kPadding) * scale_factor;
    else {
        float screen_position_y = (rect_bottom + rect_top) * 0.5;
        offset_y = screen_position_y - position_y * scale_factor;
    }

    const float position_x = -position.y;
    const bool full_width = map_width * scale_factor < viewport_width;
    const bool align_left = full_width ? (map_alignment_state == AlignmentType::kLeft) :
        (position_x - zone_map_data.min_x) * scale_factor < viewport_width * 0.5f;
    const bool align_right = full_width ? (map_alignment_state == AlignmentType::kRight) :
        (zone_map_data.max_x - position_x) * scale_factor < viewport_width * 0.5f;
    const bool center_on_position_x = always_align_to_center ||
        (!full_width && !align_left && !align_right);

    float offset_x = 0;
    if (center_on_position_x) {
        float screen_position_x = (rect_left + rect_right) * 0.5;
        offset_x = screen_position_x - position_x * scale_factor;
    }
    else if (align_left)
        offset_x = rect_left - (zone_map_data.min_x - kPadding) * scale_factor;
    else if (align_right)
        offset_x = rect_right - (zone_map_data.max_x + kPadding) * scale_factor;
    else
        offset_x = -(zone_map_data.max_x + zone_map_data.min_x) * 0.5f * scale_factor;

    // If manual mode is enabled, overwrite all the calcs we just did.
    if (!is_autocenter_enabled()) {
        // Note the positions are swapped and negated in the calculation.
        float manual_world_x = manual_screen_pt.x - static_cast<float>(viewport.X + viewport.Width * 0.5f);
        float manual_world_y = manual_screen_pt.y - static_cast<float>(viewport.Y + viewport.Height * 0.5f);
        offset_x = manual_world_x + manual_position.y * scale_factor;
        offset_y = manual_world_y + manual_position.x * scale_factor;
    }

    D3DXMATRIX mat_scale;
    D3DXMatrixScaling(&mat_scale, scale_factor, scale_factor, 1.f);
    D3DXMATRIX mat_offset;
    D3DXMatrixTranslation(&mat_offset, offset_x, offset_y, 0.0f);
    mat_model2world = mat_scale * mat_offset;

    D3DXMatrixScaling(&mat_scale, 1.f/scale_factor, 1.f/scale_factor, 1.f);
    D3DXMatrixTranslation(&mat_offset, -offset_x, -offset_y, 0.0f);
    mat_world2model = mat_offset * mat_scale;

    // Back-calculate and store the clip limits in model space.
    clip_min_x = max(static_cast<float>(zone_map_data.min_x),
                        (-viewport_width * 0.5f - offset_x) / scale_factor);
    clip_max_x = min(static_cast<float>(zone_map_data.max_x),
                        (+viewport_width * 0.5f - offset_x) / scale_factor);
    clip_min_y = max(static_cast<float>(zone_map_data.min_y),
                        (-viewport_height * 0.5f - offset_y) / scale_factor);
    clip_max_y = min(static_cast<float>(zone_map_data.max_y),
                        (+viewport_height * 0.5f - offset_y) / scale_factor);
}


// Select the line opacity and color
D3DCOLOR ZoneMap::render_get_line_color_and_opacity(const ZoneMapLine& line, int position_z, int level_id) const {
    uint8_t alpha = 255;
    int line_max_z = max(line.z0, line.z1);
    int line_min_z = min(line.z0, line.z1);

    if (level_id != kZoneMapInvalidLevelId) {
        if (line.level_id == kZoneMapInvalidLevelId) {
            // Filter all unknown lines based on z height range.
            if (line_max_z < clip_min_z || line_min_z > clip_max_z)
                alpha = static_cast<uint8_t>(map_faded_zlevel_alpha * 255);
        }
        else if (line.level_id != level_id)
            alpha = static_cast<uint8_t>(map_faded_zlevel_alpha * 255);
    }
    else if (position_z != kInvalidPositionValue) {
        // Set alpha based on current position height.
        uint8_t faded_alpha = static_cast<uint8_t>(map_faded_zlevel_alpha * 255);
        int max_delta = max(line_max_z - position_z, position_z - line_max_z);  // Absolute value.
        int min_delta = max(line_min_z - position_z, position_z - line_min_z);
        int min_distance = min(max_delta, min_delta);
        if (min_distance >= 2 * zlevel_height_scale)
            alpha = static_cast<uint8_t>(map_faded_zlevel_alpha * 255);
        else if (min_distance > zlevel_height_scale) {
            float fraction = static_cast<float>(min_distance) / zlevel_height_scale;  // value between 1.f and 2.f.
            alpha = static_cast<uint8_t>((map_faded_zlevel_alpha + (1 - map_faded_zlevel_alpha) * (2 - fraction)) * 255);
        }
    }

    auto color = D3DCOLOR_ARGB(alpha, line.red, line.green, line.blue);
    if ((!external_enabled || map_background_state == BackgroundType::kDark) && 
        line.red == 0 && line.green == 0 && line.blue == 0) {
        color = D3DCOLOR_ARGB(alpha, 64, 64, 64);  // Increase visibility of black lines.
    }

    return color;
}

// Populates the "static" per zone line_vertex_buffer and labels list.
void ZoneMap::render_load_map(IDirect3DDevice8& device, const ZoneMapData& zone_map_data) {
    render_release_resources(false);  // Forces update of all graphics but leave font.

    // Need position_z only in auto z-level fade mode (map_level_index == -1).
    zlevel_height_scale = get_zlevel_scale();
    auto* self = Zeal::EqGame::get_self();
    zlevel_position_z = (self && map_level_index == -1) ? static_cast<int>(self->Position.z) : kInvalidPositionValue;
    int level_id = (map_level_index > 0) ? zone_map_data.levels[map_level_index].level_id : kZoneMapInvalidLevelId;
    if (level_id == kZoneMapInvalidLevelId) {
        bool no_z_fade = (zlevel_position_z == kInvalidPositionValue || zone_map_data.num_levels < 2);
        clip_max_z = no_z_fade ? zone_map_data.max_z : zlevel_position_z + zlevel_height_scale;
        clip_min_z = no_z_fade ? zone_map_data.min_z : zlevel_position_z - zlevel_height_scale;
    }
    else {
        clip_max_z = zone_map_data.levels[map_level_index].max_z;
        clip_max_z += 10;  // Pad up a bit more for player height and other labels.
        clip_min_z = zone_map_data.levels[map_level_index].min_z;
    }

    line_count = zone_map_data.num_lines;
    std::vector<ZoneMap::MapVertex> line_vertices;
    line_vertices.reserve(max(0, line_count * 2));
    for (int i = 0; i < line_count; ++i) {
        const ZoneMapLine& line = zone_map_data.lines[i];
        auto color = render_get_line_color_and_opacity(line, zlevel_position_z, level_id);
        line_vertices.push_back({ .x = static_cast<float>(line.x0), .y = static_cast<float>(line.y0), .z = 0.5f, .color = color });
        line_vertices.push_back({ .x = static_cast<float>(line.x1), .y = static_cast<float>(line.y1), .z = 0.5f, .color = color });
    }

    // Create the background as two triangles using 4 vertices.
    auto background_color = get_background_color();

    MapVertex background_vertices[kBackgroundVertices] = {
        {  static_cast<float>(zone_map_data.min_x), static_cast<float>(zone_map_data.min_y),
                0.5f, background_color },  // x, y, z,color
        {  static_cast<float>(zone_map_data.max_x), static_cast<float>(zone_map_data.min_y),
                0.5f, background_color },
        {  static_cast<float>(zone_map_data.min_x), static_cast<float>(zone_map_data.max_y),
                0.5f, background_color },
        {  static_cast<float>(zone_map_data.max_x), static_cast<float>(zone_map_data.max_y),
                0.5f, background_color },
    };

    auto grid_vertices = calculate_grid_vertices(zone_map_data);
    grid_line_count = grid_vertices.size() / 2;

    const int line_buffer_size = sizeof(MapVertex) * line_count * 2;
    const int background_buffer_size = sizeof(MapVertex) * kBackgroundVertices;
    const int grid_buffer_size = sizeof(MapVertex) * grid_line_count * 2;
    // Create a Vertex buffer and copy the map line, background, and grid segments over.
    if (FAILED(
        device.CreateVertexBuffer(line_buffer_size + background_buffer_size + grid_buffer_size,
        D3DUSAGE_WRITEONLY,
        kMapVertexFvfCode,
        D3DPOOL_MANAGED,
        &line_vertex_buffer))) {
        return;
    }

    BYTE* data = nullptr;
    if (FAILED(line_vertex_buffer->Lock(0, 0, &data, D3DLOCK_DISCARD))) {
        set_enabled(false);  // Disable and release resources.
        return;
    }
    memcpy(data, background_vertices, background_buffer_size);
    memcpy(data + background_buffer_size, (const void*)line_vertices.data(), line_buffer_size);
    memcpy(data + background_buffer_size + line_buffer_size, (const void*)grid_vertices.data(),
            grid_buffer_size);
    line_vertex_buffer->Unlock();

    render_load_labels(device, zone_map_data);

    std::string description = std::format("{0}: {1}", zone_map_data.name,
        Zeal::EqGame::get_full_zone_name(zone_id));
    set_window_title(description.c_str());
}

std::vector<ZoneMap::MapVertex> ZoneMap::calculate_grid_vertices(
                                            const ZoneMapData& zone_map_data) const {
    const auto grid_color = (map_background_state == BackgroundType::kDark) ||
        (map_background_state == BackgroundType::kClear && !external_enabled) ?
        D3DCOLOR_XRGB(85, 90, 96) : D3DCOLOR_XRGB(48, 48, 48);
    const auto axis_color = D3DCOLOR_XRGB(0xfd, 0xa1, 0x72);  // Cantaloupe orange

    // Create the background grid with horizontal and vertical lines aligned to pitch.
    const int grid_x_ceil = (zone_map_data.min_x < 0) ? 0 : map_grid_pitch - 1;
    const int grid_x_min =
        ((zone_map_data.min_x + grid_x_ceil) / map_grid_pitch) * map_grid_pitch;
    const int grid_x_count = (zone_map_data.max_x - grid_x_min) / map_grid_pitch + 1;
    const int grid_y_ceil = (zone_map_data.min_y < 0) ? 0 : map_grid_pitch - 1;
    const int grid_y_min =
        ((zone_map_data.min_y + grid_y_ceil) / map_grid_pitch) * map_grid_pitch;
    const int grid_y_count = (zone_map_data.max_y - grid_y_min) / map_grid_pitch + 1;

    std::vector<ZoneMap::MapVertex> vertices;
    vertices.reserve(max(0, grid_x_count + grid_y_count));

    // Add vertical lines.
    for (int i = 0; i < grid_x_count; ++i) {
        float x = static_cast<float>(grid_x_min + i * map_grid_pitch);
        auto color = (x == 0) ? axis_color : grid_color;
        vertices.push_back({
            .x = x,
            .y = static_cast<float>(zone_map_data.min_y),
            .z = 0.5f,
            .color = color });
        vertices.push_back({
            .x = x,
            .y = static_cast<float>(zone_map_data.max_y),
            .z = 0.5f,
            .color = color });
    }

    // Add horizontal lines
    for (int i = 0; i < grid_y_count; ++i) {
        float y = static_cast<float>(grid_y_min + i * map_grid_pitch);
        auto color = (y == 0) ? axis_color : grid_color;
        vertices.push_back({
            .x = static_cast<float>(zone_map_data.min_x),
            .y = y,
            .z = 0.5f,
            .color = color });
        vertices.push_back({
            .x = static_cast<float>(zone_map_data.max_x),
            .y = y,
            .z = 0.5f,
            .color = color });
    }
    return vertices;
}


D3DCOLOR ZoneMap::get_background_color() const {
    // Alpha doesn't do much in external_enabled window mode, so set opaque.
    unsigned int alpha = external_enabled ? 255 :
                         static_cast<int>(map_background_alpha * 255 + 0.5f);
    auto background_color = D3DCOLOR_ARGB(alpha, 0, 0, 0);  // Black or clear.
    if (map_background_state == BackgroundType::kLight) {
        background_color = D3DCOLOR_ARGB(alpha, 160, 160, 160);  // Light grey.
    }
    else if (map_background_state == BackgroundType::kTan) {
        background_color = D3DCOLOR_ARGB(alpha, 190, 180, 150);  // Tan.
    }
    else if (external_enabled && map_background_state == BackgroundType::kClear) {
        background_color = D3DCOLOR_ARGB(alpha, 85, 90, 96);  // Default gray.
    }
    return background_color;
}

// Updates the succor_label with the current zone's safe coordinates.
void ZoneMap::update_succor_label() {
    static constexpr char kSuccorString[] = "Succor";
    static constexpr char kSuccorUnknownString[] = "SuccorUnknown";
    bool valid_succor = (show_zone_id == kInvalidZoneId);
    const auto* zone_info = Zeal::EqGame::ZoneInfo;
    succor_label.x = valid_succor ? -static_cast<int16_t>(zone_info->SafeCoordsX) : 0;  // Negate to map data.
    succor_label.y = valid_succor ? -static_cast<int16_t>(zone_info->SafeCoordsY) : 0;
    succor_label.z = valid_succor ? static_cast<int16_t>(zone_info->SafeCoordsZ) : 0;
    succor_label.red = 0;
    succor_label.green = 255;
    succor_label.blue = 0;
    succor_label.label = valid_succor ? kSuccorString : kSuccorUnknownString;
}

// Loads the POI labels from the ZoneMapData with some level-based filtering.
void ZoneMap::render_load_labels(IDirect3DDevice8& device, const ZoneMapData& zone_map_data) {
    labels_list.clear();

    update_succor_label();  // Ensure valid for poi searches.
    int num_labels_to_scan = zone_map_data.num_labels;
    if (map_labels_mode == LabelsMode::kOff)
        num_labels_to_scan = 0;  // Disable the scan below.
    else if (show_zone_id == kInvalidZoneId) {
        // Always insert succor if labels are active and not showing a different zone.
        labels_list.push_back(&succor_label);
    }

    // Scan through POI's adding to the list if appropriate.
    for (int i = 0; i < num_labels_to_scan; ++i) {
        const ZoneMapLabel& label = zone_map_data.labels[i];

        // First do a quick vertical z-axis check.
        if (label.z < clip_min_z || label.z > clip_max_z)
            continue;  // Not on visible level, skip it.

        // Then filter if required (kOff bailed out above and kAll doesn't need it).
        if (map_labels_mode == LabelsMode::kSummary) {
            std::string label_string = std::string(label.label);
            constexpr std::string_view zone_prefix{ "to_" };
            if (label_string.compare(0, zone_prefix.size(), zone_prefix) != 0) {
                constexpr std::string_view circle_string{ "druid_circle" };
                constexpr std::string_view ring_string{ "druid_ring" };
                constexpr std::string_view portal_string{ "_portal" };
                constexpr std::string_view spires_string{ "_spire" };
                std::transform(label_string.begin(), label_string.end(), label_string.begin(), ::tolower);
                if (label_string.find(circle_string) == std::string::npos &&
                    label_string.find(ring_string) == std::string::npos &&
                    label_string.find(portal_string) == std::string::npos &&
                    label_string.find(spires_string) == std::string::npos)
                    continue;  // Skip label.
            }
        }
        labels_list.push_back(&label);
    }
}

std::vector<std::string> ZoneMap::get_available_fonts() const {
    return BitmapFont::get_available_fonts();
}

// Loads the bitmap font for real-time text rendering to screen.
void ZoneMap::render_load_font(IDirect3DDevice8& device) {
    if (bitmap_font)
        return;

    if (font_filename.empty())  // Used to signal error state below.
        return;

    bitmap_font = BitmapFont::create_bitmap_font(device, font_filename);
    if (bitmap_font)
        return;  // Success

    if (font_filename != BitmapFont::kDefaultFontName) {
        font_filename = BitmapFont::kDefaultFontName;
        bitmap_font = BitmapFont::create_bitmap_font(device, font_filename);
        if (bitmap_font)
            return;  // Backup plan worked.
    }

    font_filename = "";  // Clear to indicate invalid and do not auto-retry.
    Zeal::EqGame::print_chat("Error initializing map font");
}

// Primary render callback that executes all components.
void ZoneMap::render_map(IDirect3DDevice8& device)
{
    if (!line_vertex_buffer)
        return;

    // Stash the current viewport and set to the map's viewport.
    D3DVIEWPORT8 original_viewport;
    device.GetViewport(&original_viewport);
    device.SetViewport(&viewport);

    // Configure for 2D drawing with alpha blending.
    D3DRenderStateStash render_state(device);
    render_state.store_and_modify({ D3DRS_CULLMODE, D3DCULL_NONE });
    render_state.store_and_modify({ D3DRS_ALPHABLENDENABLE, TRUE });
    render_state.store_and_modify({ D3DRS_SRCBLEND, D3DBLEND_SRCALPHA });
    render_state.store_and_modify({ D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA });
    render_state.store_and_modify({ D3DRS_ZENABLE, FALSE });  // Rely on render order.
    render_state.store_and_modify({ D3DRS_ZWRITEENABLE, FALSE });
    render_state.store_and_modify({ D3DRS_LIGHTING, FALSE });  // Disable lighting

    D3DMATRIX projection_original, world_original, view_original;
    device.GetTransform(D3DTS_WORLD, &world_original);
    device.GetTransform(D3DTS_VIEW, &view_original);
    device.GetTransform(D3DTS_PROJECTION, &projection_original);

    // Bypass the view (handled by world).
    D3DXMATRIX mat_view;
    D3DXMatrixIdentity(&mat_view);

    // The output of the projection matrix goes into the viewport transformation,
    // which expects input values scaled to +/- 1 for x and y. We use an orthogonal
    // matrix to just pre-scale the pixel-scale vertices to the viewport input
    // range.  We also perform an extra y-flip here.
    D3DXMATRIX mat_project;
    D3DXMatrixOrthoLH(&mat_project, static_cast<float>(viewport.Width),
        -static_cast<float>(viewport.Height), 0.f, 100.f);  // Z may need tweaking.

    device.SetTransform(D3DTS_WORLD, &mat_model2world);
    device.SetTransform(D3DTS_VIEW, &mat_view);
    device.SetTransform(D3DTS_PROJECTION, &mat_project);

    device.SetTexture(0, NULL);  // Ensure no texture is bound
    device.SetVertexShader(kMapVertexFvfCode);

    if (map_background_state != BackgroundType::kClear && !external_enabled)
        render_background(device);

    if (map_show_grid)
        render_grid(device);

    if (line_count) {
        device.SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
        device.DrawPrimitive(D3DPT_LINELIST, kBackgroundVertices, line_count);
    }

    render_markers(device);

    render_labels(device);

    if (show_zone_id == kInvalidZoneId)
        render_positions(device);

    render_handle_cursor(device);

    device.SetStreamSource(0, NULL, 0);  // Unbind final vertex buffer.
    device.SetTransform(D3DTS_PROJECTION, &projection_original);
    device.SetTransform(D3DTS_WORLD, &world_original);
    device.SetTransform(D3DTS_VIEW, &view_original);
    render_state.restore_state();
    device.SetViewport(&original_viewport);
}

// Handles the rendering of the map background tinting.
void ZoneMap::render_background(IDirect3DDevice8& device) {
    // Background vertices are stored at the start of the line_vertex_buffer.
    device.SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
    device.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, kBackgroundCount);
}

void ZoneMap::render_grid(IDirect3DDevice8& device) {
    if (!grid_line_count)
        return;

    // Grid lines are stored at the end of the line_vertex_buffer.
    device.SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
    device.DrawPrimitive(D3DPT_LINELIST, kBackgroundVertices + line_count * 2, grid_line_count);
}

void ZoneMap::render_markers(IDirect3DDevice8& device) {
    if (markers_list.empty())
        return;

    // Buffer is cleared when the markers list changes.
    if (marker_vertex_buffer == nullptr) {
        render_update_marker_buffer(device);
    }

    if (marker_vertex_buffer == nullptr)
        return;  // Failed to update above.

    // Render the marker target triangles.
    device.SetStreamSource(0, marker_vertex_buffer, sizeof(MapVertex));
    device.DrawPrimitive(D3DPT_TRIANGLELIST, 0, markers_list.size() * kMarkerCount);

    // Add the text labels centered above the marker.
    render_load_font(device);
    if (!bitmap_font)
        return;

    float size = max(5.f, marker_size * 0.5f * min(max_viewport_width, max_viewport_height));
    Vec2 offset_pixels = { 0, -(size + bitmap_font->get_line_spacing() * 0.5f) };
    auto color = D3DCOLOR_XRGB(255, 0, 0);
    for (const auto& marker : markers_list) {
        render_label_text(marker.label.c_str(), -marker.loc_y, -marker.loc_x, color,
            LabelType::PositionLabel, offset_pixels);
    }

    bitmap_font->flush_queue_to_screen();  // Flush to screen.
    device.SetVertexShader(kMapVertexFvfCode);  // Restore assumed shader.
}

// Handles the rendering of the labels_list and dynamic_labels_list.
void ZoneMap::render_labels(IDirect3DDevice8& device) {

    std::string label;
    if (show_zone_id != kInvalidZoneId && !ui_is_visible() && !external_enabled) {
        const ZoneMapData* zone_map_data = get_zone_map(zone_id);
        label += (zone_map_data) ? zone_map_data->name : "Unknown";
        label += " ";
    }
    else if (setting_add_loc_text.get()) {
        const Vec3& position = Zeal::EqGame::get_self()->Position;
        label += std::format("{0:.0f}, {1:.0f}\n", position.x, position.y);
    }
    if (map_level_index == -1 && !default_to_zlevel_autofade)
        label += "Auto";
    else if (map_level_index == 0 && default_to_zlevel_autofade)
        label += "All";
    else if (map_level_index > 0)
        label += "Level: " + std::to_string(map_level_index);

    if (label.empty() && labels_list.empty() && dynamic_labels_list.empty())
        return;

    render_load_font(device);
    if (!bitmap_font)
        return;

    if (!label.empty()) {
        const float indent_x = scale_pixels_to_model(5);
        const float indent_y = scale_pixels_to_model(5);
        render_label_text(label.c_str(), static_cast<int16_t>(clip_min_y + indent_y),
            static_cast<int16_t>(clip_min_x + indent_x), D3DCOLOR_XRGB(255, 255, 255),
            LabelType::LeftJustified);
    }

    for (const ZoneMapLabel* label : labels_list) {
        bool marker_only = (map_labels_mode == LabelsMode::kMarkerOnly);
        const char* text_label = marker_only ? "+" : label->label;
        render_label_text(text_label, label->y, label->x,
            D3DCOLOR_XRGB(label->red, label->green, label->blue),
            marker_only ? LabelType::Normal : LabelType::AddMarker);
    }

    ULONGLONG timestamp = GetTickCount64();
    for (auto it = dynamic_labels_list.begin(); it != dynamic_labels_list.end();) {
        if (it->timeout == 0 || it->timeout >= timestamp) {
            render_label_text(it->label.c_str(), -it->loc_y, -it->loc_x, it->color, LabelType::AddMarker);
            it++;
        }
        else {
            it = dynamic_labels_list.erase(it);  // Drop timed out labels.
        }
    }
    bitmap_font->flush_queue_to_screen();  // Flush to screen.
    device.SetVertexShader(kMapVertexFvfCode);  // Restore assumed shader.
}

// Handles writing a text label at map coordinates y and x to the screen.
void ZoneMap::render_label_text(const char * label, int map_y, int map_x, D3DCOLOR font_color,
                                LabelType label_type, Vec2 offset_pixels) {
    if (!bitmap_font || !label || !(*label))  // Defensive programming and empty string check.
        return;

    // Then check if the label is visible on the clipped map rect.
    // Text rendering is slow. Perform some manual clipping to skip processing
    // if the label is off screen.
    const float xf = static_cast<float>(map_x);
    const float yf = static_cast<float>(map_y);
    if ((xf < clip_min_x) || (xf > clip_max_x) ||
        (yf < clip_min_y) || (yf > clip_max_y))
        return;
    
    const int view_left = viewport.X;
    const int view_top = viewport.Y;
    const int view_right = viewport.X + viewport.Width - 1;
    const int view_bottom = viewport.Y + viewport.Height - 1;

    // Use colorful labels but override the black ones on most map backgrounds.
    if (font_color == D3DCOLOR_XRGB(0, 0, 0))
        font_color = D3DCOLOR_XRGB(192, 192, 192);  // Flip to light grey.

    // Calculate and clip the on-screen coordinate position of the text.
    const int kMaxLabelLength = 20;
    int length = strlen(label);
    if (length > kMaxLabelLength) {
        length = kMaxLabelLength;  // Truncate it.
        for (int i = 1; i < length - 1; ++i) {
            if (label[i] == '_' && label[i + 1] == '(')  // Extra info in () to drop.
                length = i;  // Truncates and breaks loop.
        }
    }
    char short_label[kMaxLabelLength + 1];
    memcpy(short_label, label, length);
    short_label[length] = 0;

    Vec3 label_screen = transform_model_to_screen(Vec3(xf, yf, 0));

    // Calculate center position that keeps the label within the viewport.
    float label_x = label_screen[0];
    float label_y = label_screen[1];
    if (label_type == LabelType::AddMarker)
        label_y -= bitmap_font->get_line_spacing() * 0.5f;  // '+' is half-sized.
    else if (label_type == LabelType::PositionLabel) {
        label_x += offset_pixels.x;
        label_y += offset_pixels.y;
    }
    bool center = (label_type != LabelType::LeftJustified);
    if (center && label_type != LabelType::Normal) {
        Vec3 size = bitmap_font->measure_string(short_label);
        const float half_width = size.x * 0.5f;
        const float half_height = size.y * 0.5f;
        label_x = max(view_left + half_width, min(view_right - half_width, label_x));
        label_y = max(view_top + half_height, min(view_bottom - half_height, label_y));
    }
    bitmap_font->queue_string(short_label, Vec3(label_x, label_y, 0), center, font_color);
    if (label_type == LabelType::AddMarker)
        bitmap_font->queue_string("+", Vec3(label_screen[0], label_screen[1], 0), center, font_color);
}

// Resets the mouse state to an idle state.
void ZoneMap::reset_mouse() {
    restore_cursor();
    mouse_drag_enabled = false;
}

// Continuously track the mouse to keep position up to date and handle dragging.
void ZoneMap::update_mouse() {
    POINT new_mouse_pt =
        POINT({ .x = *Zeal::EqGame::mouse_client_x, .y = *Zeal::EqGame::mouse_client_y });
    if (external_enabled) {
        POINT cursor;
        GetCursorPos(&cursor);  // This returns absolute screen x, y.
        POINT corner = { 0 };
        ClientToScreen(external_hwnd, &corner);
        new_mouse_pt.x = cursor.x - corner.x;  // Translate to client relative x, y.
        new_mouse_pt.y = cursor.y - corner.y;  // (the viewport is client relative)
    }

    int delta_screen_x = new_mouse_pt.x - mouse_pt.x;
    int delta_screen_y = new_mouse_pt.y - mouse_pt.y;
    mouse_pt = new_mouse_pt;

    // Clear state and ignore mouse if it is has left our viewport.
    if (mouse_pt.x < viewport.X || mouse_pt.x >= (viewport.X + viewport.Width) ||
        mouse_pt.y < viewport.Y || mouse_pt.y >= (viewport.Y + viewport.Height)) {
        mouse_drag_enabled = false;
        restore_cursor();
        return;
    }

    bool lmb = external_enabled ? (GetAsyncKeyState(VK_LBUTTON) & 0x8000) : *Zeal::EqGame::is_left_mouse_down;
    mouse_drag_enabled = mouse_drag_enabled && lmb;  // Reset when button is released.
    if (mouse_drag_enabled) {
        // Polish: Clamp these moves so the map can't be completely pushed off screen.
        manual_position.x += scale_pixels_to_model(static_cast<float>(delta_screen_y));  // Swap x,y.
        manual_position.y += scale_pixels_to_model(static_cast<float>(delta_screen_x));
    }
}


void ZoneMap::render_handle_cursor(IDirect3DDevice8& device) {
    if (!external_enabled && !(map_interactive_enabled && wnd && wnd->IsMouseOver)) {
        restore_cursor();
        return;
    }

    auto xyz = transform_screen_to_model(static_cast<float>(mouse_pt.x), static_cast<float>(mouse_pt.y));

    const int x = static_cast<int>(xyz[0] + 0.5f);
    const int y = static_cast<int>(xyz[1] + 0.5f);
    const float xf = static_cast<float>(x);
    const float yf = static_cast<float>(y);
    if (xf > clip_min_x && xf < clip_max_x && yf > clip_min_y && yf < clip_max_y)
    {
        render_load_font(device);
        if (!bitmap_font)
            return;

        hide_cursor();
        char loc_cursor[20];
        snprintf(loc_cursor, sizeof(loc_cursor), "(%i, %i)", -y, -x);
        render_label_text(loc_cursor, y, x, D3DCOLOR_XRGB(240, 240, 128), LabelType::AddMarker);
        bitmap_font->flush_queue_to_screen();
        device.SetVertexShader(kMapVertexFvfCode);  // Restore assumed shader.
    }
    else {
        restore_cursor();

        // Do some logic to overcomb the eqgame.dll's constant decrementing of ShowCursor().
        // We aren't over the visible map region, but check if the cursor is over the map window.
        RECT win_rect;
        if (external_enabled && external_hwnd && GetClientRect(external_hwnd, &win_rect) &&
            AdjustWindowRectEx(&win_rect, WS_OVERLAPPEDWINDOW, false, NULL)) {
            bool in_window = (mouse_pt.x >= win_rect.left && mouse_pt.x < win_rect.right &&
                mouse_pt.y >= win_rect.top && mouse_pt.y < win_rect.bottom);
            CURSORINFO ci = { sizeof(CURSORINFO) };
            if (in_window && GetCursorInfo(&ci) && !(ci.flags & CURSOR_SHOWING))
                while (ShowCursor(true) < 1);  // Increment until visible.
        }
    }
}

void ZoneMap::hide_cursor() {
    // In external mode, the eqgame.dll logic is typically setting the thread's ShowCursor() counter
    // with either true or false depending on whether the mouse is over the primary game window. We
    // need to override that here. We are assuming this call only happens within the clip region.
    if (external_enabled) {
        CURSORINFO ci = { sizeof(CURSORINFO) };
        if (GetCursorInfo(&ci) && (ci.flags & CURSOR_SHOWING))
            while (ShowCursor(false) > 0);
        return;
    }

    // In internal mode, we use the same patching approach the camera mods use.
    if (cursor_hidden)
        return;
    mem::write<byte>(0x53edef, 0xEB);
    cursor_hidden = true;
}

void ZoneMap::restore_cursor() {
    if (cursor_hidden) {
        cursor_hidden = false;
        mem::write<byte>(0x53edef, 0x75);
    }
}

void ZoneMap::process_left_mouse_button_down(int16_t mouse_x, int16_t mouse_y) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if ((!map_interactive_enabled && !external_enabled) || !self || zone_id == kInvalidZoneId)
        return;

    manual_position = self->Position;
    auto xyz = transform_model_to_screen(Vec3(-manual_position.y, -manual_position.x, manual_position.z));
    manual_screen_pt = POINT({ .x = static_cast<int>(xyz.x), .y = static_cast<int>(xyz.y) });
    mouse_drag_enabled = true;
}

void ZoneMap::process_right_mouse_button_down(int16_t mouse_x, int16_t mouse_y) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if ((!map_interactive_enabled && !external_enabled) || !self || zone_id == kInvalidZoneId)
        return;

    enable_autocenter();
    mouse_drag_enabled = false;
}

void ZoneMap::process_wheel_mouse_button_down(int16_t mouse_x, int16_t mouse_y) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if ((!map_interactive_enabled && !external_enabled) || !self || zone_id == kInvalidZoneId)
        return;

    auto xyz = transform_screen_to_model(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
    const int x = static_cast<int>(xyz[0] + 0.5f);
    const int y = static_cast<int>(xyz[1] + 0.5f);
    set_marker(-y, -x, nullptr);  // Swap and negate x and y for location coordinates.
}


void ZoneMap::process_mouse_wheel(int16_t mouse_delta, uint16_t flags, int16_t mouse_x, int16_t mouse_y) {
    int mouse_steps = mouse_delta / 120;  // Mouse standard is 120/step.
    float scaled_factor = static_cast<float>(static_cast<int>(zoom_factor * 0.25f));  // std::floorf
    float zoom_delta = max(0.5f, scaled_factor) * mouse_steps;
    int zoom_percent = static_cast<int>((zoom_factor + zoom_delta) * 100);
    set_zoom(zoom_percent);

    // If zone data is available, perform a more advanced zoom to center the zoom at the mouse locs.
    if (zone_id == kInvalidZoneId)
        return;

    POINT corner_pt = { 0 };
    if (external_enabled)
        ClientToScreen(external_hwnd, &corner_pt);
    POINT wheel_pt = { .x = mouse_x - corner_pt.x, .y = mouse_y - corner_pt.y };
    auto xyz = transform_screen_to_model(static_cast<float>(wheel_pt.x), static_cast<float>(wheel_pt.y));

    const int x = static_cast<int>(xyz[0] + 0.5f);
    const int y = static_cast<int>(xyz[1] + 0.5f);
    const float xf = static_cast<float>(x);
    const float yf = static_cast<float>(y);
    if ((xf < clip_min_x) || (xf > clip_max_x) || (yf < clip_min_y) || (yf > clip_max_y))
        return;

    manual_position = Vec3(-xyz[1], -xyz[0], xyz[2]);  // Swap and negate x and y
    manual_screen_pt = wheel_pt;
}

void ZoneMap::process_on_move(const Zeal::EqUI::CXRect& rect) {
    if (external_enabled || !map_interactive_enabled)  // Shouldn't happen but protect against.
        return;

    if (!is_autocenter_enabled())
        manual_on_move_trigger = true;
}

void ZoneMap::process_on_resize(int width, int height) {
    // Note: This is called with 0,0 by the external wndproc.
    enable_autocenter();  // Not ideal but keeps map visible during resizing.
}


void ZoneMap::enable_autocenter() {
    // Auto-center is enabled by disabling manual mode with invalid values.
    manual_screen_pt = POINT({ .x = kInvalidScreenValue, .y = kInvalidScreenValue });
    manual_position = Vec3(0, 0, 0);
    manual_on_move_trigger = false;
}

bool ZoneMap::is_autocenter_enabled() const {
    return (manual_screen_pt.x == kInvalidScreenValue || manual_screen_pt.y == kInvalidScreenValue);
}

// Adds vertices to mark a position at the map coordinates.
void ZoneMap::add_position_marker_vertices(float map_y, float map_x, float heading, float size,
                                    D3DCOLOR color, std::vector<MapVertex>& vertices) const {

    // Heading: 0 = N = -y, 128 = W = -x, 256 = S = +y, 384 = E = +x.
    // So: Screen x tracks -sin(heading) and y tracks -cos(heading).
    float direction = static_cast<float>(heading * M_PI / 256); // In radians.
    const float rotation = static_cast<float>(135 * M_PI / 180);
    float cos_theta0 = cosf(direction);
    float sin_theta0 = sinf(direction);
    float cos_theta1 = cosf(direction - rotation);  // Rotated clockwise.
    float sin_theta1 = sinf(direction - rotation);
    float cos_theta2 = cosf(direction + rotation);  // Rotated CCW.
    float sin_theta2 = sinf(direction + rotation);

    Vec3 vertex0 = { -sin_theta0 * size, -cos_theta0 * size, 0 };
    Vec3 vertex1 = { -sin_theta1 * size, -cos_theta1 * size, 0 };
    Vec3 vertex2 = { -sin_theta2 * size, -cos_theta2 * size, 0 };
    Vec3 vertex3 = { -vertex0.x / 8, -vertex0.y / 8, 0 };

    Vec3 symbol[kPositionVertices] = { vertex0, vertex1, vertex3,
        vertex0, vertex3, vertex2 };

    for (int i = 0; i < kPositionVertices; ++i) {
        vertices.push_back(MapVertex{
                .x = symbol[i].x + map_x,
                .y = symbol[i].y + map_y,
                .z = 0.5f,
                .color = color
            });
    }
}

// Goes through group member list adding their position vertices if enabled.
void ZoneMap::add_group_member_position_vertices(std::vector<MapVertex>& vertices) const {
    const auto* group_info = Zeal::EqGame::GroupInfo;
    if (map_show_group_mode == ShowGroupMode::kOff || !group_info->is_in_group())
        return;

    const float kShrinkFactor = 0.8f;  // Make group members 20% smaller.
    const float size = convert_size_fraction_to_model(position_size * kShrinkFactor);

    const D3DCOLOR kGroupColorLut[EQ_NUM_GROUP_MEMBERS] = {
        D3DCOLOR_XRGB(255, 0, 0),  // Red
        D3DCOLOR_XRGB(255, 165, 0), // Orange
        D3DCOLOR_XRGB(102, 255, 102),  // Green
        D3DCOLOR_XRGB(153, 204, 255),  // Blue
        D3DCOLOR_XRGB(104, 153, 255),  // Purple (violet)
    };

    for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; ++i) {
        Zeal::EqStructures::Entity* member = group_info->EntityList[i];
        if ((strlen(group_info->Names[i]) == 0) || !member || member->Type != Zeal::EqEnums::EntityTypes::Player)
            continue;  // Not a valid group member, out of zone (nullptr), or corpse.

        // Position is y,x,z.
        auto color = (member == Zeal::EqGame::get_target()) ? get_target_color() : kGroupColorLut[i];
        add_position_marker_vertices(-member->Position.x, -member->Position.y, member->Heading, size,
            color, vertices);
    }
}

void ZoneMap::add_self_pet_position_vertices(std::vector<MapVertex>& vertices) const {
    if (map_show_group_mode == ShowGroupMode::kOff)
        return;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (self && self->ActorInfo && self->ActorInfo->PetID) {
        auto pet_entity = Zeal::EqGame::get_entity_by_id(self->ActorInfo->PetID);
        if (pet_entity) {
            const float kShrinkFactor = 0.7f;  // Make pet 30% smaller.
            const float size = convert_size_fraction_to_model(position_size * kShrinkFactor);

            auto pet_color = D3DCOLOR_XRGB(195, 176, 145); // Lemon khaki
            add_position_marker_vertices(-pet_entity->Position.x, -pet_entity->Position.y,
                pet_entity->Heading, size, pet_color, vertices);
        }
    }
}

// Adds a label for each group member if enabled.
void ZoneMap::render_group_member_labels(IDirect3DDevice8& device) {
    const auto* group_info = Zeal::EqGame::GroupInfo;
    if ((map_show_group_mode == ShowGroupMode::kOff) ||
        (!map_show_all_names_override && (map_show_group_mode == ShowGroupMode::kMarkers)) ||
        !group_info->is_in_group())
        return;

    render_load_font(device);
    if (!bitmap_font)
        return;

    float offset_y = max(5.f, position_size * min(max_viewport_width, max_viewport_height));
    Vec2 offset_pixels = { 0, -offset_y };

    D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
    if (ZealService::get_instance()->ui && ZealService::get_instance()->ui->options
        && ZealService::get_instance()->ui->options.get())
        color = ZealService::get_instance()->ui->options.get()->GetColor(5);  // GroupColor

    const int short_name_length = min(map_name_length, kMaxNameLength);  // Paranoia limit.
    for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; ++i) {
        Zeal::EqStructures::Entity* member = group_info->EntityList[i];
        if ((strlen(group_info->Names[i]) == 0) || !member || member->Type != Zeal::EqEnums::EntityTypes::Player)
            continue;  // Not a valid group member, out of zone (nullptr), or corpse.

        // Writes the character name or group number (F2 - F6) centered at the character position.
        int loc_y = static_cast<int>(member->Position.x + 0.5f);  // Position is y,x,z.
        int loc_x = static_cast<int>(member->Position.y + 0.5f);  // Also need to negate it below.
        char label[kMaxNameLength + 1] = { 0 };  // Ensure null-terminated.
        if (!map_show_all_names_override && map_show_group_mode == ShowGroupMode::kNumbers)
            label[0] = static_cast<uint8_t>(i) + '2';
        else
            for (int j = 0; j < short_name_length; ++j)
                label[j] = group_info->Names[i][j];
        render_label_text(label,-loc_y, -loc_x, color, LabelType::PositionLabel, offset_pixels);
    }
    bitmap_font->flush_queue_to_screen();
    device.SetVertexShader(kMapVertexFvfCode);  // Restore assumed shader.
}

// Adds a label for each raid member if enabled.
void ZoneMap::render_raid_member_labels(IDirect3DDevice8 & device) {
    const Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
    if (!map_show_raid || !map_show_all_names_override || !raid_info->is_in_raid())
        return;

    render_load_font(device);
    auto entity_manager = ZealService::get_instance()->entity_manager.get();  // Short-term ptr.
    if (!bitmap_font || !entity_manager)
        return;

    float offset_y = max(5.f, position_size * min(max_viewport_width, max_viewport_height));
    Vec2 offset_pixels = { 0, -offset_y };

    D3DCOLOR color = D3DCOLOR_XRGB(240, 240, 240);
    if (ZealService::get_instance()->ui && ZealService::get_instance()->ui->options
        && ZealService::get_instance()->ui->options.get())
        color = ZealService::get_instance()->ui->options.get()->GetColor(4);  // Raidcolor

    // Just add a label for members, including self, since this is intended to be for transient use.
    const int short_name_length = min(map_name_length, kMaxNameLength);  // Paranoia limit.
    for (int i = 0; i < kRaidMaxMembers; ++i) {
        const auto& member = raid_info->MemberList[i];
        if (strlen(member.Name) == 0)
            continue;
        auto entity = entity_manager->Get(member.Name);
        if (!entity || entity->Type != Zeal::EqEnums::EntityTypes::Player)
            continue;  // Could be out of zone or a corpse.

        int loc_y = static_cast<int>(entity->Position.x + 0.5f);  // Position is y,x,z.
        int loc_x = static_cast<int>(entity->Position.y + 0.5f);  // Also need to negate it below.
        char label[kMaxNameLength + 1] = { 0 };  // Null-terminate.
        for (int j = 0; j < short_name_length; ++j)
            label[j] = entity->Name[j];
        render_label_text(label, -loc_y, -loc_x, color, LabelType::PositionLabel, offset_pixels);
    }
    bitmap_font->flush_queue_to_screen();
    device.SetVertexShader(kMapVertexFvfCode);  // Restore assumed shader.
}

// Adds a label for each non_ally if enabled.
void ZoneMap::render_non_ally_labels(IDirect3DDevice8& device,
                            const std::vector<Zeal::EqStructures::Entity*>& entities) {
    if (!map_show_all_names_override || entities.empty())
        return;

    render_load_font(device);
    if (!bitmap_font || entities.empty())
        return;

    const float offset_y = max(5.f, position_size * min(max_viewport_width, max_viewport_height));
    const Vec2 offset_pixels = { 0, -offset_y };
    
    const auto player_color = D3DCOLOR_XRGB(255, 0, 0);  // red
    const auto npc_color = D3DCOLOR_XRGB(255, 102, 0);  // orange
    auto target = Zeal::EqGame::get_target();

    const int short_name_length = min(map_name_length, kMaxNameLength);  // Paranoia limit.
    for (const auto& entity : entities) {
        int loc_y = static_cast<int>(entity->Position.x + 0.5f);  // Position is y,x,z.
        int loc_x = static_cast<int>(entity->Position.y + 0.5f);  // Also need to negate it below.
        auto color = (entity == target) ? get_target_color() :
                (entity->Type == Zeal::EqEnums::Player) ? player_color : npc_color;
        if (entity->Position.z < clip_min_z || entity->Position.z > clip_max_z)
            color = color & 0x80ffffff;  // Set alpha to 50% to fade for z.
        // Future: Append the class type for player characters.
        char label[kMaxNameLength + 1] = { 0 };  // Null-terminate.
        for (int j = 0; j < short_name_length; ++j)
            label[j] = entity->Name[j];
        render_label_text(label, -loc_y, -loc_x, color, LabelType::PositionLabel, offset_pixels);
    }
    bitmap_font->flush_queue_to_screen();
    device.SetVertexShader(kMapVertexFvfCode);  // Restore assumed shader.
}

// Adds simple position markers for raid members.
void ZoneMap::add_raid_marker_vertices(const Vec3& position, float size,
    D3DCOLOR color, std::vector<MapVertex>& vertices) const {

    // Add a simple equilateral triangle centered at the position pointing up.
    const float kFactor1 = 0.577350f; // 1 / sqrt(3)
    const float kFactor2 = 0.288675f; // 1 / (2 * sqrt(3))
    Vec3 vertex0 = { 0, -size * kFactor1, 0 };
    Vec3 vertex1 = { +size * 0.5f, size * kFactor2, 0 };
    Vec3 vertex2 = { -size * 0.5f, size * kFactor2, 0 };
    Vec3 symbol[kRaidPositionVertices] = { vertex0, vertex1, vertex2};

    // Allow the position marker to exceed the rect limits by size but must stay on screen.
    for (int i = 0; i < kRaidPositionVertices; ++i) {
        vertices.push_back(MapVertex{
                .x = symbol[i].x + -position.y,  // Note y,x,z and negation.
                .y = symbol[i].y + -position.x,
                .z = 0.5f,
                .color = color
            });
    }
}

// Adds simple position markers for non ally players.
void ZoneMap::add_non_ally_player_marker_vertices(const Vec3& position, float size,
    D3DCOLOR color, std::vector<MapVertex>& vertices) const {

    // Add a simple equilateral triangle centered at the position pointing down.
    const float kFactor1 = 0.577350f; // 1 / sqrt(3)
    const float kFactor2 = 0.288675f; // 1 / (2 * sqrt(3))
    Vec3 vertex0 = { 0, size * kFactor1, 0 };
    Vec3 vertex1 = { +size * 0.5f, -size * kFactor2, 0 };
    Vec3 vertex2 = { -size * 0.5f, -size * kFactor2, 0 };
    Vec3 symbol[kRaidPositionVertices] = { vertex0, vertex1, vertex2 };

    // Allow the position marker to exceed the rect limits by size but must stay on screen.
    for (int i = 0; i < kRaidPositionVertices; ++i) {
        vertices.push_back(MapVertex{
                .x = symbol[i].x + -position.y,  // Note y,x,z and negation.
                .y = symbol[i].y + -position.x,
                .z = 0.5f,
                .color = color
            });
    }
}

// Adds simple position markers for NPCs.
void ZoneMap::add_npc_marker_vertices(const Vec3& position, float size,
    D3DCOLOR color, std::vector<MapVertex>& vertices) const {
    // Add a simple square made of two triangles centered at the position.
    size *= 0.4f;  // Shrink by 80%.
    Vec2 vertex0 = { -size, -size };
    Vec2 vertex1 = { +size, -size };
    Vec2 vertex2 = { +size, +size };
    Vec2 vertex3 = { -size, +size };
    Vec2 symbol[2 * kRaidPositionVertices] = { vertex0, vertex1, vertex2, vertex0, vertex2, vertex3 };

    for (int i = 0; i < 2 * kRaidPositionVertices; ++i) {
        vertices.push_back(MapVertex{
                .x = symbol[i].x + -position.y,  // Note y,x,z and negation.
                .y = symbol[i].y + -position.x,
                .z = 0.5f,
                .color = color
         });
    }
}

// Goes through raid member list adding their position vertices if enabled.
void ZoneMap::add_raid_member_position_vertices(std::vector<MapVertex>& vertices) const {
    const Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
    if (!map_show_raid || !raid_info->is_in_raid())
        return;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    auto entity_manager = ZealService::get_instance()->entity_manager.get();  // Short-term ptr.
    if (!entity_manager || !self)
        return;

    const float size = convert_size_fraction_to_model(position_size);

    // Polish item: Review color coding to be more distinct. Possibly by class.
    const DWORD kUngrouped = Zeal::EqStructures::RaidMember::kRaidUngrouped;
    const D3DCOLOR kColorLeader = D3DCOLOR_XRGB(255, 153, 153);
    const D3DCOLOR kColorUngrouped = D3DCOLOR_XRGB(240, 240, 240);

    // Disable the markers for the same group if show_group is active.
    DWORD self_group_number = kUngrouped - 1;  // Unique default number.
    if (map_show_group_mode != ShowGroupMode::kOff) {
        // Consider optimizing this with a cache if there's a way to detect raid member changes.
        for (int i = 0; i < kRaidMaxMembers; ++i) {
            const auto& member = raid_info->MemberList[i];
            if (strcmp(member.Name, self->Name) == 0) {
                if (member.GroupNumber != kUngrouped)
                    self_group_number = member.GroupNumber;
                break;
            }
        }
    }
    for (int i = 0; i < kRaidMaxMembers; ++i) {
        const auto& member = raid_info->MemberList[i];
        if ((member.GroupNumber == self_group_number) || (strlen(member.Name) == 0)
            || (strcmp(member.Name, self->Name) == 0))
            continue;  // In same group or no raid member or it is self.
        auto entity = entity_manager->Get(member.Name);
        if (!entity || entity->Type != Zeal::EqEnums::EntityTypes::Player)
            continue;  // Could be out of zone or a corpse.

        auto color = (entity == Zeal::EqGame::get_target()) ? get_target_color() :
            member.IsGroupLeader ? kColorLeader :
            (member.GroupNumber == kUngrouped ? kColorUngrouped :
                D3DCOLOR_XRGB(224, 224, 128 + member.GroupNumber * 8));
        // Position is y,x,z.
        if (setting_show_all_player_headings.get())
            add_position_marker_vertices(-entity->Position.x, -entity->Position.y, entity->Heading,
                size * 0.7f, color, vertices);
        else
            add_raid_marker_vertices(entity->Position, size, color, vertices);
    }
}

// Returns entities not in our raid or group. This could possibly be
// handled in EntityManager so we don't have to brute force this every render call.
std::vector<Zeal::EqStructures::Entity*> ZoneMap::get_non_ally_entities() const {
    std::vector<Zeal::EqStructures::Entity*> entities;
    if (!map_show_all || (!map_show_raid && !Zeal::EqGame::get_target()))
        return entities;  // Empty list.

    // First we make a list of the allied entities.
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    auto entity_manager = ZealService::get_instance()->entity_manager.get();  // Short-term ptr.
    if (!entity_manager || !self)
        return entities;  // Empty list.

    const auto group_info = Zeal::EqGame::GroupInfo;
    const auto raid_info = Zeal::EqGame::RaidInfo;
    std::vector<Zeal::EqStructures::Entity*> allies;
    if (raid_info->is_in_raid())
    {
        for (int i = 0; i < kRaidMaxMembers; ++i) {
            const auto& member = raid_info->MemberList[i];
            if (member.Name[0] == 0)
                continue;
            auto entity = entity_manager->Get(member.Name);
            if (entity && entity->Type == Zeal::EqEnums::EntityTypes::Player)
                allies.push_back(entity);
        }
    }
    else if (group_info->is_in_group()) {
        allies.push_back(self);
        for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; ++i) {
            Zeal::EqStructures::Entity* entity = group_info->EntityList[i];
            if (entity)
                allies.push_back(entity);
        }
    }
    else
        allies.push_back(self);

    // Support adding just the target when not showing all of raid.
    if (!map_show_raid) {
        auto target = Zeal::EqGame::get_target();
        if (std::none_of(allies.begin(), allies.end(), [target](auto entity) { return entity == target;}))
            entities.push_back(target);  // Non_ally target.
        return entities;
    }

    // Now scan the global entity list for npc and other players not in the list.
    Zeal::EqStructures::Entity* current = Zeal::EqGame::get_entity_list();
    while (current != nullptr) {
        if (current->Type == Zeal::EqEnums::NPC ||
            (current->Type == Zeal::EqEnums::Player &&
                std::none_of(allies.begin(), allies.end(), [current](auto entity)
                { return current == entity; }))) {
            entities.push_back(current);
        }
        current = current->Next;
    }

    return entities;
}


// PVP mode support to show non-allied other entities (players and NPCs).
void ZoneMap::add_non_ally_position_vertices(std::vector<MapVertex>&vertices, 
    const std::vector<Zeal::EqStructures::Entity*>& entities) const {
    if (entities.empty())
        return;
    const float size = convert_size_fraction_to_model(position_size);
    auto target = Zeal::EqGame::get_target();
    auto start = vertices.size();
    const int kVertexLimit = (kMaxNonAllyTriangles - 2) * kRaidPositionVertices;  // Room for two more.
    for (const auto& entity : entities)
    {
        D3DCOLOR color = (entity == target) ? get_target_color() :
                         (Zeal::EqGame::GetLevelCon(entity) | 0xff000000);
        if (entity->Position.z < clip_min_z || entity->Position.z > clip_max_z)
            color = color & 0x80ffffff;  // Set alpha to 50% to fade for z.
        if (entity->Type == Zeal::EqEnums::Player)
            if (setting_show_all_player_headings.get())
                add_position_marker_vertices(-entity->Position.x, -entity->Position.y, entity->Heading,
                    size * 0.7f, color, vertices);
            else
                add_non_ally_player_marker_vertices(entity->Position, size, color, vertices);
        else
            add_npc_marker_vertices(entity->Position, size * 0.5, color, vertices);
        if ((vertices.size() - start) > kVertexLimit)
            break;  // Note: Dropping markers, but names will still show up.
    }
}


void ZoneMap::add_ring_vertices(std::vector<MapVertex>& vertices) const {
    if (map_ring_radius <= 0 || !Zeal::EqGame::get_self())
        return;

    Vec3 position = Zeal::EqGame::get_self()->Position;

    // Note: The vertices below are not getting clipped to the map area and
    // are simply disappearing when out of the viewport.
    // Generate kRingLineSegments + 1 vertices.
    auto color = D3DCOLOR_XRGB(183, 225, 161);  // Light grey green.
    const float angle_increment = static_cast<float>(2 * M_PI / kRingLineSegments);
    float angle_rad = 0;
    for (int i = 0; i <= kRingLineSegments; ++i) {
        if (i == kRingLineSegments)
            angle_rad = 0;  // Ensure it closes precisely with final line point.
        float x0 = map_ring_radius * cosf(angle_rad);
        float y0 = map_ring_radius * sinf(angle_rad);
        vertices.push_back(MapVertex{
            .x = x0 + -position.y,  // Note y,x,z and negation.
            .y = y0 + -position.x,
            .z = 0.5f,
            .color = color
            });
        angle_rad += angle_increment;  // Advance to next point.
    }
}

// Handles updating and rendering of self, group, and raid positions.
void ZoneMap::render_positions(IDirect3DDevice8& device) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return;

    // Create a worst-case sized Vertex buffer for live position updates.
    if (!position_vertex_buffer) {
        if (FAILED(device.CreateVertexBuffer(kPositionBufferSize,
            D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
            kMapVertexFvfCode,
            D3DPOOL_DEFAULT,
            &position_vertex_buffer))) {
            position_vertex_buffer = nullptr;
            return;
        }
    }

    // Populate a vector with D3DPT_LINESTRIP and D3DPT_TRIANGLELIST vertices.
    std::vector<MapVertex> position_vertices;
    add_ring_vertices(position_vertices);
    const int ring_vertex_count = position_vertices.size();
    add_raid_member_position_vertices(position_vertices);
    std::vector<Zeal::EqStructures::Entity*> pvp_entities = get_non_ally_entities();
    add_non_ally_position_vertices(position_vertices, pvp_entities);
    add_group_member_position_vertices(position_vertices);
    add_self_pet_position_vertices(position_vertices);
    const int member_vertex_count = position_vertices.size() - ring_vertex_count;

    const float size = convert_size_fraction_to_model(position_size);

    // Use default cursor color if levels not active or within the z clipping range.
    Vec3 position = self->Position;
    bool use_default_color = (map_level_index <= 0) ||
                (position.z >= clip_min_z && position.z <= clip_max_z);
    auto color = use_default_color ? D3DCOLOR_XRGB(250, 250, 51) : // Lemon yellow
        D3DCOLOR_XRGB(195, 176, 145); // Lemon khaki.

    // Note the x and y swap along with polarity flip.
    add_position_marker_vertices(-position.x, -position.y, self->Heading, size,
        color, position_vertices);

    int copy_size = position_vertices.size() * sizeof(position_vertices[0]);
    if (copy_size > kPositionBufferSize)
        return; // Error. Don't overrun buffer, just hide the markers.

    BYTE* data = nullptr;
    if (FAILED(position_vertex_buffer->Lock(0, 0, &data, D3DLOCK_DISCARD))) {
        position_vertex_buffer->Release();
        position_vertex_buffer = nullptr;
        return;
    }
    memcpy(data, position_vertices.data(), copy_size);
    position_vertex_buffer->Unlock();

    device.SetStreamSource(0, position_vertex_buffer, sizeof(MapVertex));

    // First draw the distance ring if enabled.
    if (ring_vertex_count) {
        device.DrawPrimitive(D3DPT_LINESTRIP, 0, ring_vertex_count - 1);  // N - 1 strip lines.
    }

    // Then draw the "other" (raid, group) markers.
    const int member_triangle_count = member_vertex_count / 3; // D3DPT_TRIANGLELIST
    if (member_triangle_count) {
        device.SetStreamSource(0, position_vertex_buffer, sizeof(MapVertex));
        device.DrawPrimitive(D3DPT_TRIANGLELIST, ring_vertex_count, member_triangle_count);
    }

    // Then draw the raid and group labels (on top of markers but below self marker).
    render_raid_member_labels(device);
    render_non_ally_labels(device, pvp_entities);
    render_group_member_labels(device);

    // And finally draw the self marker. Three vertices per triangle in D3DPT_TRIANGLELIST.
    device.SetStreamSource(0, position_vertex_buffer, sizeof(MapVertex));
    const int other_vertex_count = ring_vertex_count + member_vertex_count;
    const int self_triangle_count = (position_vertices.size() - other_vertex_count) / 3;
    device.DrawPrimitive(D3DPT_TRIANGLELIST, other_vertex_count, self_triangle_count);
}

// Translate from pixels scale to model (EQ game & map) scale.
float ZoneMap::scale_pixels_to_model(float pixels) const {
    return pixels / mat_model2world(0, 0);  // Assuming isotropic scale_factor.
}

// Translate a fraction of the screen size to model (map data) coordinates.
float ZoneMap::convert_size_fraction_to_model(float size_fraction) const {
    // Clamp to at least 5 screen pixels so it is visible.
    float size = max(5.f, size_fraction * min(max_viewport_width, max_viewport_height));
    return scale_pixels_to_model(size);
}

// Applies the transform matrix to a vector to produce an output vector.
Vec3 ZoneMap::transform_matrix(const D3DXMATRIX& matrix, const Vec3& vec) const {
    const float w = 1.0f;  // Needed for translation.
    return Vec3(
        matrix(0, 0) * vec[0] + matrix(1, 0) * vec[1] + matrix(2, 0) * vec[2] + matrix(3, 0) * w,
        matrix(0, 1) * vec[0] + matrix(1, 1) * vec[1] + matrix(2, 1) * vec[2] + matrix(3, 1) * w,
        matrix(0, 2) * vec[0] + matrix(1, 2) * vec[1] + matrix(2, 2) * vec[2] + matrix(3, 2) * w
    );
}

// Perform a software (manual) vertex transformation from model (map data) to world.
Vec3 ZoneMap::transform_model_to_world(const Vec3& model) const {
    return transform_matrix(mat_model2world, model);
}

// Perform a software (manual) vertex transformation from model (map data) to absolute screen.
Vec3 ZoneMap::transform_model_to_screen(const Vec3& model) const {
    Vec3 screen = transform_model_to_world(model);

    screen[0] += static_cast<float>(viewport.X + viewport.Width * 0.5f);
    screen[1] += static_cast<float>(viewport.Y + viewport.Height * 0.5f);
    return screen;
}

// Perform a software (manual) vertex transformation from world (pixels) to model (map data).
Vec3 ZoneMap::transform_world_to_model(const Vec3& world) const {
    return transform_matrix(mat_world2model, world);
}

// Perform a software (manual) vertex transformation from absolute screen to model (map data).
Vec3 ZoneMap::transform_screen_to_model(float x, float y, float z) const {
    float world_x = x - static_cast<float>(viewport.X + viewport.Width * 0.5f);
    float world_y = y - static_cast<float>(viewport.Y + viewport.Height * 0.5f);

    return transform_world_to_model(Vec3(world_x, world_y, z));
}


// Updates the marker vertices if enabled. 
void ZoneMap::render_update_marker_buffer(IDirect3DDevice8& device) {
    if (marker_vertex_buffer) {  // Release any resources (should be redundant).
        marker_vertex_buffer->Release();
        marker_vertex_buffer = nullptr;
    }

    if (!line_vertex_buffer)
        return;

    // Generate the vertices for four triangles that makeup the target.
    // Calculate the individual triangle 'size' in world scale.
    const float size = convert_size_fraction_to_model(marker_size * 0.5f);
    const float short_size = size * 0.25f;
    const int kNumVertices = kMarkerCount * 3;  // Separate triangles with 3 vertices (CW direction).
    Vec3 marker_symbol[kNumVertices] = {{0, 0, 0}, {short_size, size, 0}, {-short_size, size, 0},
                                    {0, 0, 0}, {-short_size, -size, 0}, {short_size, -size, 0},
                                    {0, 0, 0}, {size, -short_size, 0}, {size, short_size, 0},
                                    {0, 0, 0}, {-size, short_size, 0}, {-size, -short_size, 0}
    };

    std::vector<MapVertex> marker_vertices;
    for (const auto& marker : markers_list) {
        for (int i = 0; i < kNumVertices; ++i) {
            marker_vertices.push_back({
                .x = marker_symbol[i].x - marker.loc_x,  // Note negation world to map.
                .y = marker_symbol[i].y - marker.loc_y,
                .z = 0.5f,
                .color = D3DCOLOR_XRGB(255, 0, 0) });
        }
    }

    // Create a Vertex buffer and copy the triangle vertices.
    int marker_buffer_size = sizeof(MapVertex) * marker_vertices.size();
    if (FAILED(device.CreateVertexBuffer(marker_buffer_size,
        D3DUSAGE_WRITEONLY,
        kMapVertexFvfCode,
        D3DPOOL_MANAGED,
        &marker_vertex_buffer))) {
        return;
    }

    BYTE* data = nullptr;
    if (FAILED(marker_vertex_buffer->Lock(0, 0, &data, D3DLOCK_DISCARD))) {
        marker_vertex_buffer->Release();
        marker_vertex_buffer = nullptr;
        return;
    }
    memcpy(data, marker_vertices.data(), marker_buffer_size);
    marker_vertex_buffer->Unlock();
}

// Get the monitor size for scaling the markers and a few other elements.
void ZoneMap::update_external_window_max_viewport() {
    if (!external_enabled)
        return;

    max_viewport_width = 640;  // Safe defaults.
    max_viewport_height = 480;
    if (external_hwnd) {
        // Window exists. Dynamically query the current monitor for resolution.
        HMONITOR hmonitor = MonitorFromWindow(external_hwnd, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO monitor_info = { 0 };
        monitor_info.cbSize = sizeof(monitor_info);
        if (GetMonitorInfoA(hmonitor, &monitor_info)) {
            // Note: Ignoring DPI scaling. Use application override (see README).
            max_viewport_width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
            max_viewport_height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
        }
    }
}

// The external window message processing queue is not entirely functional, so
// this function manually keeps the external d3d state (render target size) in
// sync with the external window.
void ZoneMap::synchronize_external_window() {

    if (!enabled || !external_enabled || !external_hwnd)
        return;

    update_external_window_max_viewport();

    // Keep offsets synchronized.
    POINT corner = { 0 };
    ClientToScreen(external_hwnd, &corner);
    external_monitor_left_offset = corner.x;
    external_monitor_top_offset = corner.y;

    // Then check if the Window itself needs to be resized.
    RECT win_rect = { 0 };
    GetClientRect(external_hwnd, &win_rect);
    external_monitor_height = win_rect.bottom - win_rect.top;
    external_monitor_width = win_rect.right - win_rect.left;

    // And then check if the existing d3d framebuffer needs to be resized.
    if (external_d3ddev && (viewport.Height != external_monitor_height || viewport.Width != external_monitor_width)) {
        external_d3ddev->Release();  // Note: Reset() didn't work, so just recreate.
        external_d3ddev = nullptr;
    }

    // Initialize the d3d external window resources if necessary.
    if (!external_d3ddev) {
        render_release_resources();  // Ensure completely flushed.
        zone_id = kInvalidZoneId;   // Triggers reload of map.
        external_enabled = initialize_d3d_external_window();  // Disable if fails.
    } 
}

int ZoneMap::get_zlevel_scale() const {
    if (zlevel_height_scale_override > 0)
        return zlevel_height_scale_override;

    switch (zone_id) {
        case 71:    // airplane
            return 50;
        case 103:   // chardok
        case 1103:  // chardok_instanced
        case 108:   // veeshan
        case 1108:  // veeshan_instanced
            return 30;
        case 76:    // hateplane
        case 1076:  // hate_instanced
        case 121:   // crystal
        case 89:    // sebilis
        case 1089:  // sebilis_instanced
            return 20;
        case 10:    // freporte
            return 15;
        case 63:    // unrest
            return 6;
        default:
            // Effectively disable for open outdoor zones (type 1).
            return (Zeal::EqGame::ZoneInfo->TimeType == 1) ? 2000 :  kDefaultZLevelHeightScale;
    }
}

// Returns true  if a map update is needed due to a z level change.
bool ZoneMap::is_zlevel_change() const {
    if (map_level_index != -1)
        return false;  // Not in auto-mode.

    auto* self = Zeal::EqGame::get_self();
    if (!self)
        return false;

    int abs_delta_z = abs(static_cast<int>(self->Position.z) - zlevel_position_z);
    return abs_delta_z > zlevel_height_scale / 4;
}

// System callback to execute the map rendering.
void ZoneMap::callback_render() {
    process_external_window_messages();  // Handle even when map disabled.

    if (!enabled || !Zeal::EqGame::is_in_game() || !wnd)
        return;

    // Ensure render is not out of sync with the UI window state (e.g. title bar close).
    if (!external_enabled && map_interactive_enabled && !ui_is_visible()) {
        set_enabled(false, false);
        return;
    }

    // Bail out early if the external window is minimized (iconic). 
    if (external_enabled && IsIconic(external_hwnd))
        return;

    // The external window message queue isn't reliable for resize messages.
    // The call below manages the external window (if active) and keeps the
    // D3D render objects in sync with the window size.
    synchronize_external_window();
    update_mouse();  // Track mouse movement (dragging, button release).

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    IDirect3DDevice8* device = external_enabled ?
        external_d3ddev : ZealService::get_instance()->dx->GetDevice();
    if (!self || !device)
        return;

    int target_zone_id = (show_zone_id != kInvalidZoneId) ? show_zone_id : self->ZoneId;
    const ZoneMapData* zone_map_data = get_zone_map(target_zone_id);
    if (!zone_map_data) {
        return;
    }

    if (zone_id != target_zone_id || is_zlevel_change()) {
        zone_id = target_zone_id;
        render_load_map(*device, *zone_map_data);
    }

    if (!external_enabled && map_interactive_enabled && wnd && wnd->IsMinimized)
        return;  // Allow load map but do not attempt to render when minimized.

    render_update_viewport(*device);  // Updates size and position of output viewport.
    render_update_transforms(*zone_map_data);  // Updates scaling, offsets, and clipping.

    // ZealMap is responsible for the updates when using the external d3d device.
    if (external_enabled) {
        auto background_color = get_background_color();
        device->Clear(0, NULL, D3DCLEAR_TARGET, background_color, 1.0f, 0);
        if (FAILED(device->BeginScene()))   // begins the 3D scene
            return;
    }

    render_map(*device);

    if (external_enabled) {
        device->EndScene();    // ends the 3D scene
        device->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
    }
}


void ZoneMap::add_dynamic_label(const std::string& label_text,
                    int loc_y, int loc_x, unsigned int duration_ms, D3DCOLOR font_color) {
    if (label_text.empty() || zone_id == kInvalidZoneId)
        return;

    ULONGLONG timestamp = GetTickCount64();
    int timeout = (duration_ms > 0) ? (timestamp + duration_ms) : 0;

    // Search through existing labels and update existing if there's a name match.
    for (auto& label : dynamic_labels_list) {
        if (label.label == label_text) {
            label.loc_y = loc_y;
            label.loc_x = loc_x;
            label.timeout = timeout;
            label.color = font_color;
            return;
        }
    }

    // Constrain the label spam to a reasonable number.
    if (dynamic_labels_list.size() == kMaxDynamicLabels) {
        dynamic_labels_list.erase(dynamic_labels_list.begin());  // Drop the first one.
    }

    // And finally add it to the list.
    dynamic_labels_list.emplace_back(label_text, loc_y, loc_x, timeout, font_color);
}

const ZoneMapData* ZoneMap::get_zone_map(int zone_id) {
    // Based on map_data_mode uses internal data and/or external data.
    const ZoneMapData* internal_map = get_zone_map_data(zone_id);
    if (map_data_mode == MapDataMode::kInternal)
        return internal_map;

    auto it = map_data_cache.find(zone_id);
    if (it != map_data_cache.end())
        if (!it->second || !it->second->zone_map_data || !it->second->zone_map_data->num_lines)
            return internal_map;  // Failed previous search so return default.
        else
            return it->second->zone_map_data.get();  // Note: Sharing a naked pointer here.

    // Not in cache, so try to load it. Need a name, so check zone map data or client world data.
    std::string short_name = internal_map ?
        std::string(internal_map->name) : Zeal::EqGame::get_zone_name_from_index(zone_id);
    if (short_name.empty())
        return internal_map;

    // Note the internal_map->name field is required.
    auto new_map = std::make_unique<CustomMapData>();

    // Primary file must exist.
    std::string filename = "map_files/" + short_name + ".txt";
    if (!add_map_data_from_file(filename, *new_map)) {
        map_data_cache[zone_id] = nullptr;  // Flag it as a failed load.
        return internal_map;
    }

    // Optional data from a second file (typically poi's).
    std::string filename_1 = "map_files/" + short_name + "_1.txt";
    add_map_data_from_file(filename_1, *new_map);

    if (map_data_mode == MapDataMode::kBoth && internal_map)
        add_map_data_from_internal(*internal_map, *new_map);
    else if (new_map->lines.size() == 0) {
        map_data_cache[zone_id] = nullptr;  // Flag it as a failed load.
        return internal_map;
    }

    // Analyzes all added data to populate the final ZoneMapData structure.
    assemble_zone_map(short_name.c_str(), *new_map);

    map_data_cache.emplace(zone_id, std::move(new_map));
    return map_data_cache[zone_id]->zone_map_data.get();
}


void ZoneMap::add_map_data_from_internal(const ZoneMapData& internal_map, CustomMapData& map_data) {
    for (int i = 0; i < internal_map.num_lines; ++i)
        map_data.lines.push_back(internal_map.lines[i]);
    for (int i = 0; i < internal_map.num_labels; ++i)
        map_data.labels.push_back(internal_map.labels[i]);  // Not modifying label pointers.
    for (int i = 0; i < internal_map.num_levels; ++i)
        map_data.levels.push_back(internal_map.levels[i]);
}


bool ZoneMap::add_map_data_from_file(const std::string& filename, CustomMapData& map_data) {
    // Not yet cached, so check if an external file is present.
    std::ifstream map_file;
    map_file.open(filename);
    if (map_file.fail())
        return false;

    // Parse the map_file. Use old-school sscanf_s for easy parsing of the csv.
    std::string line;
    while (std::getline(map_file, line))
    {
        float x0, y0, z0, x1, y1, z1;
        int dummy;
        unsigned int red, green, blue;
        char buffer[64];
        if (sscanf_s(line.c_str(), "L %f, %f, %f, %f, %f, %f, %u, %u, %u",
            &x0, &y0, &z0, &x1, &y1, &z1, &red, &green, &blue) == 9) {
    
            map_data.lines.emplace_back(
                static_cast<int>(x0 + 0.5f), static_cast<int>(y0 + 0.5f),
                static_cast<int>(z0 + 0.5f), static_cast<int>(x1 + 0.5f),
                static_cast<int>(y1 + 0.5f), static_cast<int>(z1 + 0.5f),
                static_cast<uint8_t>(red), static_cast<uint8_t>(green),
                static_cast<uint8_t>(blue), 0);
        }
        else if (sscanf_s(line.c_str(), "P %f, %f, %f, %u, %u, %u, %i, %s",
            &x0, &y0, &z1, &red, &green, &blue, &dummy, buffer, sizeof(buffer)) == 8) {
            buffer[sizeof(buffer) - 1] = 0; // Guarantee null-termination.
            map_data.label_strings.emplace_back(std::string(buffer));
            map_data.labels.emplace_back(static_cast<int>(x0 + 0.5f),
                static_cast<int>(y0 + 0.5f), static_cast<int>(z0 + 0.5f),
                static_cast<uint8_t>(red), static_cast<uint8_t>(green),
                static_cast<uint8_t>(blue),
                map_data.label_strings.back().c_str());
        }
    }

    // Note: map_data.levels not currently supported.
    return true;
}

void ZoneMap::assemble_zone_map(const char* zone_name, CustomMapData& map_data) {

    // Sort the lines by z so they are rendered from bottom to top.
    std::stable_sort(map_data.lines.begin(), map_data.lines.end(), [](const ZoneMapLine& lhs, const ZoneMapLine& rhs) {
        return lhs.z0 + lhs.z1 < rhs.z0 + rhs.z1; });

    // Pull out limits of all map data. Note that in the python script the floating point values
    // were used along with floor() and ceil(), while this is running on the already rounded ints.
    int max_x = map_data.lines[0].x0;
    int min_x = max_x;
    int max_y = map_data.lines[0].y0;
    int min_y = max_y;
    int max_z = map_data.lines[0].z0;
    int min_z = max_z;
    for (const auto& line : map_data.lines) {
        max_x = max(max_x, max(line.x0, line.x1));
        min_x = min(min_x, min(line.x0, line.x1));
        max_y = max(max_y, max(line.y0, line.y1));
        min_y = min(min_y, min(line.y0, line.y1));
        max_z = max(max_z, max(line.z0, line.z1));
        min_z = min(min_z, min(line.z0, line.z1));
    }

    max_x = max(max_x, min_x + 1); // Sanity clamp limits.
    max_y = max(max_y, min_y + 1);
    max_z = max(max_z, min_z + 1);

    if (!map_data.levels.size())
        map_data.levels.emplace_back(0, max_z, min_z);

    // Assemble and assign the zone_map_data.
    map_data.zone_map_data = std::make_unique<ZoneMapData>(ZoneMapData(
        {
            .name = zone_name,
            .max_x = max_x,
            .min_x = min_x,
            .max_y = max_y,
            .min_y = min_y,
            .max_z = max_z,
            .min_z = min_z,
            .num_lines = static_cast<int>(map_data.lines.size()),
            .num_labels = static_cast<int>(map_data.labels.size()),
            .num_levels = static_cast<int>(map_data.levels.size()),
            .lines = &map_data.lines[0],
            .labels = &map_data.labels[0],
            .levels = &map_data.levels[0]
        }));

}

void ZoneMap::set_enabled(bool _enabled, bool update_default) {
    _enabled = _enabled && (wnd != nullptr);  // Only allow enabling after init_ui.
    if (!_enabled) {
        render_release_resources();  // Also flush font for now.
        hide_external_window();
        ui_hide();
        restore_cursor();
    }
    else if (!enabled && external_enabled) {
        render_release_resources();  // Just in case cleanup.
        if (!show_external_window()) {
            Zeal::EqGame::print_chat("Falling back to internal map overlay");
            external_enabled = false;
        }
    }
    else if (!external_enabled && map_interactive_enabled) {
        ui_show();
    }
    if (!enabled && _enabled)
        zone_id = kInvalidZoneId;  // Triggers reload of map.
    enabled = _enabled;

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapEnabled", enabled);

    update_ui_options();
}

void ZoneMap::set_external_enable(bool _external_enabled, bool update_default) {
    if (external_enabled != _external_enabled) {
        bool was_enabled = enabled;
        set_enabled(false, false);
        external_enabled = _external_enabled;
        enable_autocenter();  // Sizes can be very different so reset.
        reset_mouse();
        if (was_enabled)
            set_enabled(true, false);
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapExternalEnabled", external_enabled);

    update_ui_options();
}

void ZoneMap::set_show_raid(bool enable, bool update_default) {
    if (map_show_raid != enable) {
        map_show_raid = enable;
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapShowRaid", map_show_raid);

    update_ui_options();
}

void ZoneMap::set_show_all_names_override(bool flag) {
    map_show_all_names_override = flag;
}

void ZoneMap::set_interactive_enable(bool enable, bool update_default) {
    map_interactive_enabled = enable;
    if (map_interactive_enabled) {
        if (enabled && !external_enabled)
            ui_show();
    }
    else {
        ui_hide();
        if (!external_enabled) {
            enable_autocenter();
            reset_mouse();
        }
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapInteractiveEnabled", map_interactive_enabled);

    update_ui_options();
}

void ZoneMap::set_default_to_zlevel_autofade(bool enable_autofade, bool update_default) {
    if (default_to_zlevel_autofade != enable_autofade) {
        default_to_zlevel_autofade = enable_autofade;
        map_level_index = default_to_zlevel_autofade ? -1 : 0;
        zone_id = kInvalidZoneId;  // Triggers reload.
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapDefaultToAutofade", default_to_zlevel_autofade);

    update_ui_options();
}

void ZoneMap::set_show_grid(bool enable, bool update_default) {
    map_show_grid = enable;

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapShowGrid", map_show_grid);

    update_ui_options();
}

bool ZoneMap::set_grid_pitch(int new_pitch, bool update_default) {
    new_pitch = max(100, min(kMaxGridPitch, new_pitch));  // Just clamp for now.
    map_grid_pitch = new_pitch;
    zone_id = kInvalidZoneId;  // Triggers reload with modified pitch.
    if (!map_show_grid)
        set_show_grid(true, false);  // Turn on the grid if off.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapGridPitch", map_grid_pitch);

    update_ui_options();
    return true;
}

// Helper function to calculate tracking distance.
static int get_tracking_distance() {
    auto* char_info = Zeal::EqGame::get_char_info();
    if (!char_info)
        return 0;

    // Assume non-tracking classes have zero skill.
    const int kTrackingIndex = 53; // (0xc12 - 0xba8) / sizeof(WORD) in client code.
    int skill_level = char_info->Skills[kTrackingIndex];
    if (skill_level <= 0)
        return 0;

    // Formulas are from GenerateTrackingList in client and zone type dependent.
    // Type 0: soldungb, gukbottom, sebilis (indoor dungeon)
    // Type 1: fearplane, commons, lavastorm, oot (outdoor)
    // Type 2: gfaydark, qeynos (outdoor city)
    // Type 3: akanon, kaladima, neriaka (indoor city)
    // Type 4: erudnint (only zone in db, outdoor city no sky?)
    // Type 5: veeshan, airplane, blackburrow (outdoor dungeon)
    const uint8_t zone_type = Zeal::EqGame::ZoneInfo->TimeType;
    const bool long_range = (zone_type == 1) || (zone_type == 2) || (zone_type == 5);
    return long_range ? (500 + skill_level * 10 + max(0, (skill_level - 100)) * 10) :
                        (500 + skill_level / 2 + max(0, (skill_level - 100)) * 4);
}

bool ZoneMap::set_ring_radius(int new_radius, bool update_default) {
    if (new_radius <= 0) {
        if (map_ring_radius > 0)
            new_radius = 0;  // If already on, toggles off.
        else
            new_radius = get_tracking_distance();  // Auto-range if class supports.
    } else
        new_radius = max(10, min(10000, new_radius));  // Just clamp for now.

    map_ring_radius = new_radius;

    // Skip storing preferences for now.
    // if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
    //    ZealService::get_instance()->ini->setValue<int>("Zeal", "MapGridPitch", map_grid_pitch);
    // update_ui_options();
    return true;
}

bool ZoneMap::set_name_length(int new_length, bool update_default) {
    new_length = max(3, min(kMaxNameLength, new_length));  // Just clamp for now.
    map_name_length = new_length;

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapNameLength", map_name_length);

    update_ui_options();
    return true;
}

bool ZoneMap::set_font(std::string font_name, bool update_default) {
    release_font();  // Triggers update / reload.  Note that reload could fail later.
    font_filename = font_name.empty() ? std::string(BitmapFont::kDefaultFontName) : font_name;

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<std::string>("Zeal", "MapFontFilename", font_filename);

    update_ui_options();
    return true;  // Always succeed for now.
}


void ZoneMap::toggle_background() {
    map_background_state = BackgroundType::e(static_cast<int>(map_background_state) + 1);
    if (map_background_state > BackgroundType::kLast) {
        map_background_state = BackgroundType::kFirst;
    }
    zone_id = kInvalidZoneId;  // Triggers reload with updated background color.
    update_ui_options();
}

void ZoneMap::toggle_labels() {
    map_labels_mode = LabelsMode::e(static_cast<int>(map_labels_mode) + 1);
    if (map_labels_mode > LabelsMode::kLast) {
        map_labels_mode = LabelsMode::kFirst;
    }
    zone_id = kInvalidZoneId;  // Triggers reload which includes updating labels.
    update_ui_options();
}


bool ZoneMap::set_background(int new_state_in, bool update_default) {
    BackgroundType::e new_state = BackgroundType::e(new_state_in);
    if ((new_state < BackgroundType::kFirst) || (new_state > BackgroundType::kLast)) {
        update_ui_options();  // Keep UI in sync when fails.
        return false;
    }

    map_background_state = new_state;
    zone_id = kInvalidZoneId;  // Triggers reload with updated background color.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapBackgroundState", new_state);

    update_ui_options();
    return true;
}

bool ZoneMap::set_background_alpha(int percent, bool update_default) {
    map_background_alpha = max(0, min(1.f, percent / 100.f));
    zone_id = kInvalidZoneId;  // Triggers reload with updated background color.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapBackgroundAlpha", map_background_alpha);

    update_ui_options();
    return true;
}

bool ZoneMap::set_faded_zlevel_alpha(int percent, bool update_default) {
    map_faded_zlevel_alpha = max(0, min(1.f, percent / 100.f));
    zone_id = kInvalidZoneId;  // Triggers reload with updated background color.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapFadedZLevelAlpha", map_faded_zlevel_alpha);

    update_ui_options();
    return true;
}

bool ZoneMap::set_alignment(int alignment_in, bool update_default) {
    AlignmentType::e alignment{ alignment_in };
    alignment = max(AlignmentType::kFirst, min(AlignmentType::kLast, alignment));

    map_alignment_state = alignment;

    set_zoom(100);  // Reset zoom on an alignment change to avoid confusion and corner cases.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
            ZealService::get_instance()->ini->setValue<int>("Zeal", "MapAlignment", alignment);
 
    update_ui_options();
    return true;
}

bool ZoneMap::set_labels_mode(int mode_in, bool update_default) {
    LabelsMode::e mode{ mode_in };
    mode = max(LabelsMode::kFirst, min(LabelsMode::kLast, mode));

    map_labels_mode = mode;
    zone_id = kInvalidZoneId;  // Triggers reload which includes updating labels.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapLabelsMode", mode);

    update_ui_options();
    return true;
}

bool ZoneMap::set_show_group_mode(int mode_in, bool update_default) {
    ShowGroupMode::e mode{ mode_in };
    mode = max(ShowGroupMode::kFirst, min(ShowGroupMode::kLast, mode));

    map_show_group_mode = mode;  // Dynamic labels so reload not needed.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapShowGroupMode", mode);

    update_ui_options();
    return true;
}

bool ZoneMap::set_map_data_mode(int mode_in, bool update_default) {
    MapDataMode::e mode{ mode_in };
    if (mode < MapDataMode::kFirst || mode > MapDataMode::kLast) {
        update_ui_options();  // Ensure in sync.
        return false;
    }

    map_data_mode = mode;
    map_data_cache.clear();  // Wipe cache clean.
    reset_zone_state();

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapDataMode", mode);

    update_ui_options();
    return true;
}

bool ZoneMap::set_position_size(int new_size_percent, bool update_default) {
    position_size = max(kMinPositionSize, min(kMaxPositionSize, new_size_percent / 100.f * kMaxPositionSize));

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapPositionSize", position_size);

    update_ui_options();
    return true;  // Just clamp and report success.
}

bool ZoneMap::set_marker_size(int new_size_percent, bool update_default) {
    marker_size = max(kMinMarkerSize, min(kMaxMarkerSize, new_size_percent / 100.f * kMaxMarkerSize));
    clear_markers(false);  // Release resources but keep validity state to trigger an update.
    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapMarkerSize", marker_size);

    update_ui_options();
    return true;  // Just clamp and report success.
}

void ZoneMap::clear_markers(bool erase_list) {
    if (erase_list)
        markers_list.clear();
    if (marker_vertex_buffer) {
        marker_vertex_buffer->Release();
        marker_vertex_buffer = nullptr;
    }
}

void ZoneMap::set_marker(int y, int x, const char* label) {
    // Only allow markers that are within the zone map data limits.
    // Note the y and x are in location (position) negated coords.
    const ZoneMapData* zone_map_data = get_zone_map(zone_id);
    if (!zone_map_data ||
        (-x < zone_map_data->min_x || -x > zone_map_data->max_x ||
        -y < zone_map_data->min_y || -y > zone_map_data->max_y))
        return;

    clear_markers(false);  // Reset visible markers but don't empty list.

    // Convert nullptr labels to position labels.
    char loc_label[20];
    if (!label) {
        snprintf(loc_label, sizeof(loc_label), "(%i, %i)", y, x);
        loc_label[sizeof(loc_label) - 1] = 0;
        label = loc_label;
    }
    markers_list.push_back({ .loc_y = y, .loc_x = x, .label = std::string(label)});
}

bool ZoneMap::set_map_zoom_default_index(int new_index, bool update_default) {
    map_zoom_default_index = max(0, min(kNumDefaultZoomFactors - 1, new_index));
    set_zoom(static_cast<int>(100 * kDefaultZoomFactors[map_zoom_default_index] + 0.5f));   // Also triggers update_ui_options();

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapZoomDefaultIndex", map_zoom_default_index);

    return true;  // Just clamp and report success.
}

void ZoneMap::toggle_zoom() {

    // Assumes zoom factors are monotonic and look for the first zoom higher
    // than the current state.  Wraps back to 100% if beyond max zoom.
    float new_zoom_factor = 1.f;
    for (int i = 0; i < kNumDefaultZoomFactors; ++i) {
        if (zoom_factor < kDefaultZoomFactors[i]) {
            new_zoom_factor = kDefaultZoomFactors[i];
            break;
        }
    }
    clear_markers(false);  // Marker size is dependent on scale_factor.
    if (!is_autocenter_enabled()) {  // Toggle back to auto-center.
        enable_autocenter();
    }
    zoom_factor = new_zoom_factor;
    enable_autocenter(); 
    update_ui_options();
}

bool ZoneMap::set_zoom(int zoom_percent) {
    clear_markers(false);  // Marker size is dependent on scale_factor.
    zoom_factor = zoom_percent * 0.01f;
    zoom_factor = min(100.f, max(1.f, zoom_factor));

    update_ui_options();
    return true;  // Just clamp and return success.
}

void ZoneMap::load_ini(IO_ini* ini)
{
    if (!ini->exists("Zeal", "MapEnabled"))
        ini->setValue<bool>("Zeal", "MapEnabled", false);
    if (!ini->exists("Zeal", "MapInteractiveEnabled"))
        ini->setValue<bool>("Zeal", "MapInteractiveEnabled", false);
    if (!ini->exists("Zeal", "MapExternalEnabled"))
        ini->setValue<bool>("Zeal", "MapExternalEnabled", false);
    if (!ini->exists("Zeal", "MapDefaultToAutofade"))
        ini->setValue<bool>("Zeal", "MapDefaultToAutofade", false);
    if (!ini->exists("Zeal", "MapExternalLeftOffset"))
        ini->setValue<int>("Zeal", "MapExternalLeftOffset", 16);
    if (!ini->exists("Zeal", "MapExternalTopOffset"))
        ini->setValue<int>("Zeal", "MapExternalTopOffset", 16);
    if (!ini->exists("Zeal", "MapExternalWidth"))
        ini->setValue<int>("Zeal", "MapExternalWidth", 640);
    if (!ini->exists("Zeal", "MapExternalHeight"))
        ini->setValue<int>("Zeal", "MapExternalHeight", 640);
    if (!ini->exists("Zeal", "MapZoomDefaultIndex"))
        ini->setValue<int>("Zeal", "MapZoomDefaultIndex", 0);
    if (!ini->exists("Zeal", "MapShowRaid"))
        ini->setValue<bool>("Zeal", "MapShowRaid", false);
    if (!ini->exists("Zeal", "MapShowGrid"))
        ini->setValue<bool>("Zeal", "MapShowGrid", false);
    if (!ini->exists("Zeal", "MapGridPitch"))
        ini->setValue<int>("Zeal", "MapGridPitch", kDefaultGridPitch);
    if (!ini->exists("Zeal", "MapNameLength"))
        ini->setValue<int>("Zeal", "MapNameLength", map_name_length);
    if (!ini->exists("Zeal", "MapDataMode"))
        ini->setValue<int>("Zeal", "MapDataMode", static_cast<int>(MapDataMode::kInternal));
    if (!ini->exists("Zeal", "MapBackgroundState"))
        ini->setValue<int>("Zeal", "MapBackgroundState", static_cast<int>(BackgroundType::kClear));
    if (!ini->exists("Zeal", "MapBackgroundAlpha"))
        ini->setValue<float>("Zeal", "MapBackgroundAlpha", kDefaultBackgroundAlpha);
    if (!ini->exists("Zeal", "MapFadedZLevelAlpha"))
        ini->setValue<float>("Zeal", "MapFadedZLevelAlpha", map_faded_zlevel_alpha);
    if (!ini->exists("Zeal", "MapAlignment"))
        ini->setValue<int>("Zeal", "MapAlignment", static_cast<int>(AlignmentType::kFirst));
    if (!ini->exists("Zeal", "MapLabelsMode"))
        ini->setValue<int>("Zeal", "MapLabelsMode", static_cast<int>(LabelsMode::kOff));
    if (!ini->exists("Zeal", "MapShowGroupMode"))
        ini->setValue<int>("Zeal", "MapShowGroupMode", static_cast<int>(ShowGroupMode::kOff));
    if (!ini->exists("Zeal", "MapPositionSize"))
        ini->setValue<float>("Zeal", "MapPositionSize", kDefaultPositionSize);
    if (!ini->exists("Zeal", "MapMarkerSize"))
        ini->setValue<float>("Zeal", "MapMarkerSize", kDefaultMarkerSize);
    if (!ini->exists("Zeal", "MapFontFilename"))
        ini->setValue<std::string>("Zeal", "MapFontFilename", std::string(BitmapFont::kDefaultFontName));

    // Note: Not protected against corrupted ini file (like a boolean instead of float).
    enabled = false;  // Disable and use reenable to make it a pending flag.
    reenable_on_zone = ini->getValue<bool>("Zeal", "MapEnabled");
    map_interactive_enabled = ini->getValue<bool>("Zeal", "MapInteractiveEnabled");
    external_enabled = ini->getValue<bool>("Zeal", "MapExternalEnabled");
    default_to_zlevel_autofade = ini->getValue<bool>("Zeal", "MapDefaultToAutofade");
    external_monitor_left_offset = ini->getValue<int>("Zeal", "MapExternalLeftOffset");
    external_monitor_top_offset = ini->getValue<int>("Zeal", "MapExternalTopOffset");
    external_monitor_width = ini->getValue<int>("Zeal", "MapExternalWidth");
    external_monitor_height = ini->getValue<int>("Zeal", "MapExternalHeight");
    map_zoom_default_index = ini->getValue<int>("Zeal", "MapZoomDefaultIndex");
    map_show_raid = ini->getValue<bool>("Zeal", "MapShowRaid");
    map_show_grid = ini->getValue<bool>("Zeal", "MapShowGrid");
    map_grid_pitch = ini->getValue<int>("Zeal", "MapGridPitch");
    map_name_length = ini->getValue<int>("Zeal", "MapNameLength");
    map_data_mode = MapDataMode::e(ini->getValue<int>("Zeal", "MapDataMode"));
    map_background_state = BackgroundType::e(ini->getValue<int>("Zeal", "MapBackgroundState"));
    map_background_alpha = ini->getValue<float>("Zeal", "MapBackgroundAlpha");
    map_faded_zlevel_alpha = ini->getValue<float>("Zeal", "MapFadedZLevelAlpha");
    map_alignment_state = AlignmentType::e(ini->getValue<int>("Zeal", "MapAlignment"));
    map_labels_mode = LabelsMode::e(ini->getValue<int>("Zeal", "MapLabelsMode"));
    map_show_group_mode = ShowGroupMode::e(ini->getValue<int>("Zeal", "MapShowGroupMode"));
    position_size = ini->getValue<float>("Zeal", "MapPositionSize");
    marker_size = ini->getValue<float>("Zeal", "MapMarkerSize");
    font_filename = ini->getValue<std::string>("Zeal", "MapFontFilename");

    // Sanity clamp ini values.
    map_grid_pitch = max(100, min(kMaxGridPitch, map_grid_pitch));
    map_name_length = max(3, min(kMaxNameLength, map_name_length));
    map_data_mode = max(MapDataMode::kFirst, min(MapDataMode::kLast, map_data_mode));
    map_background_state = max(BackgroundType::kFirst, min(BackgroundType::kLast, map_background_state));
    map_background_alpha = max(0, min(1.f, map_background_alpha));
    map_faded_zlevel_alpha = max(0, min(1.f, map_faded_zlevel_alpha));
    map_zoom_default_index = max(0, min(kNumDefaultZoomFactors, map_zoom_default_index));
    map_alignment_state = max(AlignmentType::kFirst, min(AlignmentType::kLast, map_alignment_state));
    map_labels_mode = max(LabelsMode::kFirst, min(LabelsMode::kLast, map_labels_mode));
    map_show_group_mode = max(ShowGroupMode::kFirst, min(ShowGroupMode::kLast, map_show_group_mode));
    position_size = max(kMinPositionSize, min(kMaxPositionSize, position_size));
    marker_size = max(kMinMarkerSize, min(kMaxMarkerSize, marker_size));

    // Not checking the external monitor left and top since virtual screens can be negative.
    external_monitor_width = max(kWinMinSize, external_monitor_width);
    external_monitor_height = max(kWinMinSize, external_monitor_height);
}

void ZoneMap::save_ini()
{
    if (!ZealService::get_instance())
        return;
    std::shared_ptr<IO_ini> ini = ZealService::get_instance()->ini;
    if (!ini)
        return;

    ini->setValue<bool>("Zeal", "MapEnabled", enabled);
    ini->setValue<bool>("Zeal", "MapInteractiveEnabled", map_interactive_enabled);
    ini->setValue<bool>("Zeal", "MapExternalEnabled", external_enabled);
    ini->setValue<bool>("Zeal", "MapDefaultToAutofade", default_to_zlevel_autofade);
    ini->setValue<int>("Zeal", "MapExternalLeftOffset", external_monitor_left_offset);
    ini->setValue<int>("Zeal", "MapExternalTopOffset", external_monitor_top_offset);
    ini->setValue<int>("Zeal", "MapExternalWidth", external_monitor_width);
    ini->setValue<int>("Zeal", "MapExternalHeight", external_monitor_height);
    ini->setValue<int>("Zeal", "MapZoomDefaultIndex", map_zoom_default_index);
    ini->setValue<bool>("Zeal", "MapShowRaid", map_show_raid);
    ini->setValue<bool>("Zeal", "MapShowGrid", map_show_grid);
    ini->setValue<int>("Zeal", "MapGridPitch", map_grid_pitch);
    ini->setValue<int>("Zeal", "MapNameLength", map_name_length);
    ini->setValue<int>("Zeal", "MapDataMode", static_cast<int>(map_data_mode));
    ini->setValue<int>("Zeal", "MapBackgroundState", static_cast<int>(map_background_state));
    ini->setValue<float>("Zeal", "MapBackgroundAlpha", map_background_alpha);
    ini->setValue<float>("Zeal", "MapFadedZLevelAlpha", map_faded_zlevel_alpha);
    ini->setValue<int>("Zeal", "MapAlignment", static_cast<int>(map_alignment_state));
    ini->setValue<int>("Zeal", "MapLabelsMode", static_cast<int>(map_labels_mode));
    ini->setValue<int>("Zeal", "MapShowGroupMode", static_cast<int>(map_show_group_mode));
    ini->setValue<float>("Zeal", "MapPositionSize", position_size);
    ini->setValue<float>("Zeal", "MapMarkerSize", marker_size); 
    ini->setValue<std::string>("Zeal", "MapFontFilename",
                        font_filename.empty() ? std::string("default") : font_filename);
}

void ZoneMap::update_ui_options() {
    if (wnd != nullptr && ZealService::get_instance() && ZealService::get_instance()->ui
                    && ZealService::get_instance()->ui->options)
       ZealService::get_instance()->ui->options->UpdateOptionsMap();
}

bool ZoneMap::set_map_rect(float top, float left, float bottom, float right) {
    if (!max_viewport_width || !max_viewport_height)
        return false;

    if (!wnd || wnd->IsMinimized || external_enabled)
        return false;

    int top_edge = static_cast<int>(max(0.f, min(1.f, top)) * max_viewport_height);
    int left_edge = static_cast<int>(max(0.f, min(1.f, left)) * max_viewport_width);
    int bottom_edge = static_cast<int>(max(0.f, min(1.f, bottom)) * max_viewport_height);
    int right_edge = static_cast<int>(max(0.f, min(1.f, right)) * max_viewport_width);
    if ((bottom_edge - top_edge < kWinMinSize) || (right_edge - left_edge < kWinMinSize)) {
        return false;  // Reject if not at least minimum size.
    }
    map_client_rect = Zeal::EqUI::CXRect(left_edge, top_edge, right_edge, bottom_edge);

    // Update the UI window's size to match (with padding).
    Zeal::EqUI::CXRect rect;
    Zeal::EqGame::EqGameInternal::CXWndGetClientRect(wnd, 0, &rect);
    rect.Left = rect.Left - wnd->Location.Left;
    rect.Top = rect.Top - wnd->Location.Top;
    rect.Right = rect.Right - wnd->Location.Right;
    rect.Bottom = rect.Bottom - wnd->Location.Bottom;
    Zeal::EqGame::EqGameInternal::CXWndMoveAndInvalidate(wnd, 0, left_edge - rect.Left, top_edge - rect.Top,
        right_edge - rect.Right, bottom_edge - rect.Bottom);

    enable_autocenter();  // Manual screen coordinates are invalid. Just flush.
    return true;
}

void ZoneMap::parse_external(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        set_external_enable(!external_enabled, false);
        return;
    }

    Zeal::EqGame::print_chat("Usage: /map external (toggles on and off)");
}

void ZoneMap::parse_rect(const std::vector<std::string>& args) {
    std::vector<int> tlbr;
    for (int i = 2; i < args.size(); i++)
    {
        int value = 0;
        if (Zeal::String::tryParse(args[i], &value))
            tlbr.push_back(value);
        else
            break;
    }

    if ((tlbr.size() != 4) || !set_map_rect(tlbr[0] / 100.f, tlbr[1] / 100.f, tlbr[2] / 100.f, tlbr[3] / 100.f))
    {
        Zeal::EqGame::print_chat("Usage: /map rect <top> <left> <bottom> <right> (percent of screen dimensions)");
        Zeal::EqGame::print_chat("Example: /map rect 10 20 50 65");
    }
}

// Alternative to the more direct rect that supports width and height.
void ZoneMap::parse_size(const std::vector<std::string>& args) {
    std::vector<int> tlhw;
    for (int i = 2; i < args.size(); i++)
    {
        int value = 0;
        if (Zeal::String::tryParse(args[i], &value))
            tlhw.push_back(value);
        else
            break;
    }

    if ((tlhw.size() != 4) || !set_map_rect(tlhw[0] / 100.f, tlhw[1] / 100.f,
        (tlhw[0] + tlhw[2]) / 100.f,    // Convert height to bottom (top + height).
        (tlhw[1] + tlhw[3]) / 100.f))   // And width to right (left + width).
    {
        Zeal::EqGame::print_chat("Usage: /map size <top> <left> <height> <width> (percent of screen dimensions)");
        Zeal::EqGame::print_chat("Example: /map size 10 20 40 45");
    }
}

void ZoneMap::parse_alignment(const std::vector<std::string>& args) {
    int alignment = AlignmentType::kFirst - 1;
    if (args.size() == 3) {
        if (args[2] == "left")
            alignment = AlignmentType::kLeft;
        else  if (args[2] == "center")
            alignment = AlignmentType::kCenter;
        else if (args[2] == "right")
            alignment = AlignmentType::kRight;
    }

    if ((alignment < AlignmentType::kFirst) || !set_alignment(alignment, false)) {
        Zeal::EqGame::print_chat("Usage: /map alignment left,center,right");
    }
}

void ZoneMap::parse_labels(const std::vector<std::string>& args) {
    // Backdoor support for setting a dynamic label.
    if (args.size() == 6) {
        int y, x, time_ms;
        if (Zeal::String::tryParse(args[3], &y) && Zeal::String::tryParse(args[4], &x) && Zeal::String::tryParse(args[5], &time_ms))
            add_dynamic_label(args[2], y, x, time_ms);
        return;
    }

    int labels = LabelsMode::kFirst - 1;
    if (args.size() == 3) {
        if (args[2] == "off")
            labels = LabelsMode::kOff;
        else if (args[2] == "summary")
            labels = LabelsMode::kSummary;
        else if (args[2] == "all")
            labels = LabelsMode::kAll;
        else if (args[2] == "marker")
            labels = LabelsMode::kMarkerOnly;
    }

    if ((labels < LabelsMode::kFirst) || !set_labels_mode(labels, false)) {
        Zeal::EqGame::print_chat("Usage: /map labels off,summary,all,marker");
    }
}

void ZoneMap::parse_map_data_mode(const std::vector<std::string>& args) {
    int mode = MapDataMode::kFirst - 1;
    if (args.size() == 3) {
        if (args[2] == "internal")
            mode = MapDataMode::kInternal;
        else if (args[2] == "both")
            mode = MapDataMode::kBoth;
        else if (args[2] == "external")
            mode = MapDataMode::kExternal;
    }

    if ((mode < MapDataMode::kFirst) || !set_map_data_mode(mode, false)) {
        Zeal::EqGame::print_chat("Usage: /map data_mode internal, both, external");
    }
}

void ZoneMap::parse_show_group(const std::vector<std::string>& args) {
    int length = 0;
    if (args.size() == 2) {
        set_show_group_mode((map_show_group_mode == ShowGroupMode::kOff) ?
            ShowGroupMode::kMarkers : ShowGroupMode::kOff, false);
        Zeal::EqGame::print_chat("Showing group members is %s", 
            (map_show_group_mode != ShowGroupMode::kOff) ? "ON" : "OFF");
    }
    else if (args.size() == 3 && args[2] == "markers") {
        set_show_group_mode(ShowGroupMode::kMarkers, false);
        Zeal::EqGame::print_chat("Showing group members");
    }
    else if (args.size() == 3 && args[2] == "numbers") {
        set_show_group_mode(ShowGroupMode::kNumbers, false);
        Zeal::EqGame::print_chat("Showing group members with numbers");
    }
    else if (args.size() == 3 && args[2] == "names") {
        set_show_group_mode(ShowGroupMode::kNames, false);
        Zeal::EqGame::print_chat("Showing group members with names");
    }
    else if (args.size() == 4 && args[2] == "length" && Zeal::String::tryParse(args[3], &length)
             && set_name_length(length, false)) {
        Zeal::EqGame::print_chat("Setting map name label length to: %i", length);
    }
    else {
        Zeal::EqGame::print_chat("Usage: /map show_group (toggles on/off), /map show_group [labels_off, numbers, names]");
        Zeal::EqGame::print_chat("Usage: /map show_group length <value (3 to 20)>");
    }
}

void ZoneMap::parse_show_raid(const std::vector<std::string>& args) {
    set_show_raid(!is_show_raid_enabled(), false);
    Zeal::EqGame::print_chat("Showing raid members is %s", is_show_raid_enabled() ? "ON" : "OFF");
}

void ZoneMap::parse_marker(const std::vector<std::string>& args) {
    if (args.size() <= 2) {
        clear_markers();  // Remove all markers from the list and screen.
        return;
    }

    int y = 0;
    int x = 0;
    if (args[2] == "size") {
        int new_size = 0;
        if (Zeal::String::tryParse(args[3], &new_size)) {
            set_marker_size(new_size, false);
            return;
        }
    }
    else if (args.size() >= 4 && args.size() < 6 && Zeal::String::tryParse(args[2], &y) && Zeal::String::tryParse(args[3], &x)) {
        const char* label = (args.size() == 5) ? args[4].c_str() : nullptr;
        set_marker(y, x, label);
        set_enabled(true);
        return;
    }

    Zeal::EqGame::print_chat("Usage: /map marker (clears all markers");
    Zeal::EqGame::print_chat("Usage: /map marker <y> <x> [label] (label is optional)");
    Zeal::EqGame::print_chat("Example: /map marker 500 -1000 (drops a marker at loc 500, -1000)");
}

bool ZoneMap::search_poi(const std::string& search_term) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return false;  // Not handled here.

    const ZoneMapData* zone_map_data = get_zone_map(zone_id);
    if (!zone_map_data || zone_map_data->num_labels < 0) {
        Zeal::EqGame::print_chat("No map POI data available for this zone");
        return false;
    }

    // Go through the labels looking for a match. Use lowercase for both.
    std::string search_lower = search_term;
    std::transform(search_lower.begin(), search_lower.end(), search_lower.begin(), ::tolower);
    int match_count = 0;
    for (int i = 0; i <= zone_map_data->num_labels; ++i) {
        const ZoneMapLabel& label = (i == 0) ? succor_label : zone_map_data->labels[i - 1];
        std::string label_lower = std::string(label.label);
        std::transform(label_lower.begin(), label_lower.end(), label_lower.begin(), ::tolower);
        if (label_lower.find(search_lower) != std::string::npos) {
            match_count++;
            const char* flag = "-";
            if (match_count == 1) {
                Zeal::EqGame::print_chat("Map poi search results for: %s:", search_term.c_str());
                flag = "+";  // Flag the POI that was used for the marker.
                // Note: Need to negate y and x to go from map data to game world coordinates.
                set_marker(-label.y, -label.x, label.label);
                set_enabled(true);
            }
            Zeal::EqGame::print_chat("%s[%i]: (%i, %i): %s", flag, i, -label.y, -label.x, label.label);
        }
    }
    if (match_count == 0) {
        Zeal::EqGame::print_chat("%s is not a poi match", search_term.c_str());
        return false;
    }
    return true;
}

bool ZoneMap::parse_shortcuts(const std::vector<std::string>& args) {
    // Support implicit shortcuts (/map 0 = clear marker, /map y x = set marker).
    int y = 0;
    int x = 0;
    if (args.size() == 2) {
        if (args[1] == "0") {
            clear_markers();
            return true;
        }
        if (args[1] != "help") {
            return search_poi(args[1]);
        }
    }
    else if (args.size() >= 3 && args.size() <= 4 && Zeal::String::tryParse(args[1], &y)
                && Zeal::String::tryParse(args[2], &x)) {
        const char* label = (args.size() == 4) ? args[3].c_str() : nullptr;
        set_marker(y, x, label);
        set_enabled(true);
        return true;
       }

    return false;
}

void ZoneMap::parse_background(const std::vector<std::string>& args) {
    int state = 0;
    if ((args.size() == 3 || args.size() == 4) && Zeal::String::tryParse(args[2], &state)
                                                        && set_background(state, false)) {
        if (args.size() == 3)
            return;
        int alpha = 100;
        if (Zeal::String::tryParse(args[3], &alpha) &&
                    set_background_alpha(alpha, false))
            return;
    }
    Zeal::EqGame::print_chat( "Usage: /map background <%i to %i> [alpha]",
                            BackgroundType::kFirst, BackgroundType::kLast);
}

void ZoneMap::parse_zoom(const std::vector<std::string>& args) {
    int zoom_percent = 0;
    if (args.size() != 3 || !Zeal::String::tryParse(args[2], &zoom_percent) || !set_zoom(zoom_percent))
        Zeal::EqGame::print_chat( "Usage: /map zoom <percent> (<= 100 disables)");
}

int ZoneMap::find_zone_id(const std::string& zone_name) const {
    // First search the client's available zones for a match.
    int index = Zeal::EqGame::get_index_from_zone_name(zone_name);
    if (index != Zeal::EqGame::kInvalidZoneId)
        return index;

    // This doesn't happen frequently, so just do a brute force search for the matching name.
    for (int i = 0; i < 1200; ++i) {
        const auto* zone_map_data = get_zone_map_data(i);
        if (zone_map_data && zone_name == zone_map_data->name)
            return i;
    }
    return kInvalidZoneId;
}

bool ZoneMap::set_show_zone_id(const std::string& zone_name) {
    int new_id = find_zone_id(zone_name);
    if (new_id == kInvalidZoneId) {
        Zeal::EqGame::print_chat("Invalid zone name: %s", zone_name.c_str());
        return false;
    }

    reset_zone_state();   // Call this before below since it resets show_zone_id.
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (self && self->ZoneId != new_id) {
        show_zone_id = new_id; // Will trigger a refresh with the new ID.
        Zeal::EqGame::print_chat("Map showing zone[%i]: %s", show_zone_id, zone_name.c_str());
    }

    return true;
}

void ZoneMap::parse_show_zone(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        reset_zone_state();  // Sets show_zone_id to kInvalidZoneId and trigger a full map refresh.
        return;
    }

    if (args.size() == 3 && set_show_zone_id(args[2]))
        return;
        
    Zeal::EqGame::print_chat("Usage: /map show_zone <short_name> (blank name resets to normal mode)");
}

void ZoneMap::parse_world_data(const std::vector<std::string>& args) {
    if (args.size() == 3 && args[2] == "dump") {
        for (int i = 0; i < Zeal::EqGame::kNumZoneIds; ++i) {
            std::string short_name = Zeal::EqGame::get_zone_name_from_index(i);
            std::string long_name = Zeal::EqGame::get_full_zone_name(i);
            if (!short_name.empty()) {
                Zeal::EqGame::print_chat("[%i]: %s, %s", i, short_name.c_str(), long_name.c_str());
                int reverse_lookup = Zeal::EqGame::get_index_from_zone_name(short_name.c_str());
                if (reverse_lookup != i)
                    Zeal::EqGame::print_chat("[%i]: Reverse mismatch %i", i, reverse_lookup);
            }
        }
        return;
    }
    if (args.size() == 4 && args[2] == "search") {
        Zeal::EqGame::print_chat("Searching for %s", args[3].c_str());
        for (int i = 0; i < Zeal::EqGame::kNumZoneIds; ++i) {
            std::string short_name = Zeal::EqGame::get_zone_name_from_index(i);
            std::string long_name = Zeal::EqGame::get_full_zone_name(i);
            transform(short_name.begin(), short_name.end(), short_name.begin(), ::tolower);
            transform(long_name.begin(), long_name.end(), long_name.begin(), ::tolower);
            std::string search = args[3];
            transform(search.begin(), search.end(), search.begin(), ::tolower);
            if (short_name.find(search) != std::string::npos ||
                long_name.find(search) != std::string::npos)
                Zeal::EqGame::print_chat("[%i]: %s, %s", i, short_name.c_str(), long_name.c_str());
        }
        return;
    }

    Zeal::EqGame::print_chat("Usage: /map world dump, /map world search <substring>");
}


void ZoneMap::parse_grid(const std::vector<std::string>& args) {
    int grid_pitch = 0;
    if (args.size() == 2) {
        set_show_grid(!map_show_grid, false);
    } else if (args.size() != 3 || !Zeal::String::tryParse(args[2], &grid_pitch) ||
                                            !set_grid_pitch(grid_pitch, false))
        Zeal::EqGame::print_chat("Usage: /map grid [pitch] (blank pitch toggles)");
}

void ZoneMap::parse_ring(const std::vector<std::string>& args) {
    int ring_radius = 0;
    if (args.size() == 2) {
        set_ring_radius(ring_radius, false);  // Toggles off or uses auto-range.
    }
    else if (args.size() != 3 || !Zeal::String::tryParse(args[2], &ring_radius) ||
        !set_ring_radius(ring_radius, false))
        Zeal::EqGame::print_chat("Usage: /map ring [radius] (blank disables or uses calculated track range)");
}


void ZoneMap::parse_font(const std::vector<std::string>& args) {
    if (args.size() == 3) {
        set_font(args[2]);
        Zeal::EqGame::print_chat("Setting map font to: %s", args[2].c_str());
        return;
    }
    Zeal::EqGame::print_chat("Usage: /map font <name> (%s/<name>%s)",
                              BitmapFont::kFontDirectoryPath, BitmapFont::kFontFileExtension);
    Zeal::EqGame::print_chat("Available fonts:");
    auto fonts = get_available_fonts();
    for (const auto& font : fonts)
        Zeal::EqGame::print_chat("  %s", font.c_str());
}

void ZoneMap::toggle_level_up() {
    set_level(map_level_index + 1);
}

void ZoneMap::toggle_level_down() {
    set_level(map_level_index - 1);
}

// Set the visible level index.  0 = all levels, -1 = auto z-level, > 0 = specific level.
bool ZoneMap::set_level(int level_index) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return false;

    const ZoneMapData* zone_map_data = get_zone_map(zone_id);
    if (zone_id == kInvalidZoneId || !zone_map_data || zone_map_data->num_levels < 2)
        return false;

    // Commit to making an update. Disable level filtering by default.
    int current_zone_id = zone_id;
    zone_id = kInvalidZoneId;  // Triggers refresh.
    map_level_index = 0;

    // Support wraparound of toggles up and down.
    int minimum_index = (show_zone_id == kInvalidZoneId) ? -1 : 0;  // No auto-zone in show_zone.
    if (level_index == zone_map_data->num_levels) {
        level_index = minimum_index;  // Auto-mode is level -1.
    }
    else if (level_index == minimum_index - 1) {
        level_index = zone_map_data->num_levels - 1;
    }

    if (level_index < minimum_index || level_index >= zone_map_data->num_levels)
        return false;  // Ineligible index.

    map_level_index = level_index;
    return true;
}

void ZoneMap::parse_level(const std::vector<std::string>& args) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return;

    const ZoneMapData* zone_map_data = get_zone_map(zone_id);
    if (!zone_map_data || zone_map_data->num_levels < 2) {
        Zeal::EqGame::print_chat("No map level data available for this zone");
        return;
    }

    int value = 0;
    if (args.size() == 2) {
        Zeal::EqGame::print_chat("Levels for zone: %s", zone_map_data->name);
        for (int i = 0; i < zone_map_data->num_levels; ++i) {
            const ZoneMapLevel& level = zone_map_data->levels[i];
            Zeal::EqGame::print_chat("[%i]: min_z: %i, max_z: %i, id: %i",
                i, level.min_z, level.max_z, level.level_id);
        }
    }
    else if (args.size() == 4 && args[2] == "alpha" && Zeal::String::tryParse(args[3], &value))
        set_faded_zlevel_alpha(value, false);
    else if (args.size() == 4 && args[2] == "autoz" && Zeal::String::tryParse(args[3], &value)) {
        zlevel_height_scale_override = value;
        zone_id = kInvalidZoneId;  // Triggers reload.
    }
    else if (args.size() != 3 || !Zeal::String::tryParse(args[2], &value) || !set_level(value)) {
        Zeal::EqGame::print_chat("Usage: /map level <index> (0: to show all levels, -1: auto z-level)");
        Zeal::EqGame::print_chat("Usage: /map level alpha <0-100>, /map level autoz <height>");
    }
}


void ZoneMap::parse_poi(const std::vector<std::string>& args) {

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return;

    const ZoneMapData* zone_map_data = get_zone_map(zone_id);
    if (!zone_map_data || zone_map_data->num_labels == 0) {
        Zeal::EqGame::print_chat("No map POI data available for this zone");
        return;
    }

    // Note: Need to negate y and x to go from map data to world coordinates.
    int poi = 0;
    if (args.size() == 2) {
        Zeal::EqGame::print_chat("Map POI list for zone: %s", zone_map_data->name);
        for (int i = 0; i <= zone_map_data->num_labels; ++i) {
            const ZoneMapLabel& label = (i == 0) ? succor_label : zone_map_data->labels[i - 1];
            Zeal::EqGame::print_chat("[%i]: (%i, %i): %s", i, -label.y, -label.x, label.label);
        }
    }
    else if (args.size() == 3 && Zeal::String::tryParse(args[2], &poi, true)) {
        if (poi < 0 || poi > zone_map_data->num_labels) {
            Zeal::EqGame::print_chat("Invalid selection index");
            return;
        }
        const ZoneMapLabel& label = (poi == 0) ? succor_label : zone_map_data->labels[poi - 1];
        set_marker(-label.y, -label.x, label.label);
        set_enabled(true);
    }
    else if (args.size() != 3 || !search_poi(args[2])) {
        Zeal::EqGame::print_chat("Usage: /map poi (displays list of all points of interest)");
        Zeal::EqGame::print_chat("Usage: /map poi <#> (drops a marker at numeric list [#])");
        Zeal::EqGame::print_chat("Usage: /map poi <phrase> (search poi labels for phrase)");
        Zeal::EqGame::print_chat("Example: /map poi 2 (drops a marker at list item [2])");
        Zeal::EqGame::print_chat("Example: /map poi cazic (searches poi labels for 'cazic')");
    }
}

void print_matrix(const D3DMATRIX& matrix, const char* name) {
    Zeal::EqGame::print_chat(name);
    for (int i = 0; i < 4; ++i) {
        Zeal::EqGame::print_chat("[%i]: %f, %f, %f, %f", i, matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
    }
}

void ZoneMap::dump() {
    Zeal::EqGame::print_chat("-----Map-----");
    Zeal::EqGame::print_chat("enabled: %i, inter_enable: %i, ext_enable: %i, reenable: %i, zone_id: %i",
        enabled, map_interactive_enabled, external_enabled, reenable_on_zone, zone_id);
    Zeal::EqGame::print_chat("background: %i (%.2f), grid: %i, align: %i, labels:%i, show_zone: %i",
        map_background_state, map_background_alpha, map_show_grid ? map_grid_pitch : 0,
        map_alignment_state, map_labels_mode, show_zone_id);
    Zeal::EqGame::print_chat("num_markers: %i, num_labels: %i, name_length: %i, ring: %i",
            markers_list.size(), labels_list.size(), map_name_length, map_ring_radius);
    Zeal::EqGame::print_chat("view: t: %i, l: %i, h: %i, w: %i, Max H: %i, W: %i", 
        viewport.Y, viewport.X, viewport.Height, viewport.Width, max_viewport_height, max_viewport_width);
    if (wnd) {
        const Zeal::EqUI::CXRect& loc = wnd->IsMinimized ? wnd->LocationPlaceholder : wnd->Location;
        Zeal::EqGame::print_chat("wnd: t: %i, l: %i, b: %i, r: %i", loc.Top, loc.Left, loc.Bottom, loc.Right);
    }
    Zeal::EqGame::print_chat("client: t: %i, l: %i, b: %i, r: %i", 
        map_client_rect.Top, map_client_rect.Left, map_client_rect.Bottom, map_client_rect.Right);
    Zeal::EqGame::print_chat("clip: t: %f, l: %f, b: %f, r: %f", clip_min_y, clip_min_x, clip_max_y, clip_max_x);
    Zeal::EqGame::print_chat("level: index: %i, z_max: %i, z_min: %i, faded_alpha: %.2f, z_scale: %i",
                                map_level_index, clip_max_z, clip_min_z, map_faded_zlevel_alpha, 
                                zlevel_height_scale_override > 0 ? zlevel_height_scale_override : zlevel_height_scale);
    Zeal::EqGame::print_chat("position_size: %f, marker_size: %f, show_group: %i, show_raid: %i",
        position_size, marker_size, static_cast<int>(map_show_group_mode), map_show_raid);
    Zeal::EqGame::print_chat("scale_factor: %f, offset_y: %f, offset_x: %f, zoom: %f",
        mat_model2world(0,0), mat_model2world(3,1), mat_model2world(3, 0), zoom_factor);
    Zeal::EqGame::print_chat("dyn_labels_size: %i, data_mode: %i, data_cache: %i", 
        dynamic_labels_list.size(), map_data_mode, map_data_cache.size());
    Zeal::EqGame::print_chat("line_count: %i, grid: %i, line: %i, position: %i, marker: %i, font: %i (%s)", line_count,
        grid_line_count, line_vertex_buffer != nullptr, position_vertex_buffer != nullptr, marker_vertex_buffer != nullptr,
        bitmap_font != nullptr, font_filename.c_str());
    Zeal::EqGame::print_chat("interactive: (%i, %i) = (%f, %f), mouse: (%i, %i)", manual_screen_pt.x,
                   manual_screen_pt.y, manual_position.x, manual_position.y, mouse_pt.x, mouse_pt.y);
    Zeal::EqGame::print_chat("external: enabled: %i, left: %i, top: %i, width: %i, height: %i",
        external_enabled, external_monitor_left_offset, external_monitor_top_offset, external_monitor_width, external_monitor_height);
    Zeal::EqGame::print_chat("hinstance: 0x%08x, hwnd: 0x%08x, d3d: 0x%08x, d3ddev: 0x%08x",
        reinterpret_cast<uint32_t>(external_hinstance), reinterpret_cast<uint32_t>(external_hwnd),
        reinterpret_cast<uint32_t>(external_d3d), reinterpret_cast<uint32_t>(external_d3ddev));

    if (external_hwnd) {
        RECT win_rect = { 0 };
        GetWindowRect(external_hwnd, &win_rect);
        Zeal::EqGame::print_chat("ext win rect top: %i, left: %i, bottom: %i, right %i",
            win_rect.top, win_rect.left, win_rect.bottom, win_rect.right);

        RECT client_rect = { 0 };
        GetClientRect(external_hwnd, &client_rect);
        Zeal::EqGame::print_chat("ext client rect top: %i, left: %i, bottom: %i, right %i",
            client_rect.top, client_rect.left, client_rect.bottom, client_rect.right);
    }

    //print_matrix(mat_model2world, "Model to World");  // Redundant with scale_factor and offsets.
}

bool ZoneMap::parse_command(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        set_enabled(!enabled);
    }
    else if (args[1] == "on") {
        set_enabled(true);
    }
    else if (args[1] == "off") {
        set_enabled(false);
    }
    else if (args[1] == "external") {
        parse_external(args);
    }
    else if (args[1] == "rect") {
        parse_rect(args);  // Set the Top, Left, Bottom, Right coordinates of window.
    }
    else if (args[1] == "size") {
        parse_size(args);  // Set the Top, Left, Height, Width of window.
    }
    else if (args[1] == "alignment") {
        parse_alignment(args);
    }
    else if (args[1] == "marker") {
        parse_marker(args);
    }
    else if (args[1] == "background") {
        parse_background(args);
    }
    else if (args[1] == "zoom") {
        parse_zoom(args);
    }
    else if (args[1] == "poi") {
        parse_poi(args);
    }
    else if (args[1] == "labels") {
        parse_labels(args);
    }
    else if (args[1] == "level") {
        parse_level(args);
    }
    else if (args[1] == "data_mode") {
        parse_map_data_mode(args);
    }
    else if (args[1] == "show_group") {
        parse_show_group(args);
    }
    else if (args[1] == "show_raid") {
        parse_show_raid(args);
    }
    else if (args[1] == "show_zone") {
        parse_show_zone(args);
    }
    else if (args[1] == "world") {
        parse_world_data(args);
    }
    else if (args[1] == "save_ini") {
        Zeal::EqGame::print_chat("Saving current map settings");
        save_ini();
    }
    else if (args[1] == "grid") {
        parse_grid(args);
    }
    else if (args[1] == "ring") {
        parse_ring(args);
    }
    else if (args[1] == "font") {
        parse_font(args);
    }
    else if (args[1] == "always_center") {
        always_align_to_center = !always_align_to_center;  // Experimental option.
    }
    else if (args[1] == "dump") {
        dump();
    }
    else if (!parse_shortcuts(args)) {
        Zeal::EqGame::print_chat("Usage: /map [on|off|size|alignment|marker|background|zoom|poi|labels|level|]");
        Zeal::EqGame::print_chat("Usage: /map [show_group|show_raid|show_zone|save_ini|grid|ring|font]");
        Zeal::EqGame::print_chat("Usage: /map [external|data_mode|world]");
        Zeal::EqGame::print_chat("Shortcuts: /map <y> <x>, /map 0, /map <poi_search_term>");
        Zeal::EqGame::print_chat("Examples: /map 100 -200 (drops a marker at loc 100, -200), /map 0 (clears marker)");
    }
    return true;
}

void ZoneMap::reset_zone_state() {
    zone_id = kInvalidZoneId;
    show_zone_id = kInvalidZoneId;
    clear_markers(true);
    dynamic_labels_list.clear();
    map_level_index = default_to_zlevel_autofade ? -1 : 0;
    zoom_factor = kDefaultZoomFactors[map_zoom_default_index];  // Reset for consistent behavior.
    map_ring_radius = 0;  // Disable since skill-based range is zone dependent.
    manual_on_move_trigger = false;
    reset_mouse();
    enable_autocenter();
}

void ZoneMap::callback_zone() {
    if (!wnd)
        return;  // Block calls until init_ui has been called.

    set_window_title();  // Reset the title to default until loaded.
    map_show_all = false;  // Disable marauders mode on a zone.
    reset_zone_state();
    if (enabled || reenable_on_zone)
        set_enabled(true, false);  // Calls ui_show() and syncs enable state.
    reenable_on_zone = false;
}

void ZoneMap::set_window_title(const char* title) {
    title = title ? title : "Zeal Map";
    if (wnd)
        wnd->Text.Set(title);
    if (external_hwnd)
        SetWindowTextA(external_hwnd, title);
}

void ZoneMap::release_font() {
    if (bitmap_font) {
        bitmap_font->release();  // Paranoid redundancy for destructor below.
        bitmap_font.reset();
    }
}

void ZoneMap::callback_dx_reset() {
    render_release_resources();
    release_d3d_external_window();
    set_enabled(false, false);  // Turn off the map to be safe.
}


// SidlScreenWnd vtable function definitions

// Disable drawing a background (make it transparent).
static int __fastcall DrawBackground(Zeal::EqUI::EQWND* wnd, int unusedEDX) {
    return 0;
}

// Use the mouse scroll wheel event since it has undergone proper zlayer window filtering.
static int __fastcall HandleWheelMove(Zeal::EqUI::EQWND* wnd, int unusedEDX, int32_t mouse_x, int32_t mouse_y, int32_t scroll_delta, uint32_t unused4) {
    ZealService* zeal = ZealService::get_instance();
    if (zeal && zeal->zone_map)
        zeal->zone_map->process_mouse_wheel(scroll_delta, 0x00, mouse_x, mouse_y);
    return 0;
}

// Use the mouse left button down event since it has undergone proper zlayer window filtering.
static int __fastcall HandleLButtonDown(Zeal::EqUI::EQWND* wnd, int unusedEDX, int32_t mouse_x, int32_t mouse_y, uint32_t unused3) {
    ZealService* zeal = ZealService::get_instance();
    if (zeal && zeal->zone_map)
        zeal->zone_map->process_left_mouse_button_down(mouse_x, mouse_y);
    return 0;
}

// Use the mouse right button down event since it has undergone proper zlayer window filtering.
static int __fastcall HandleRButtonDown(Zeal::EqUI::EQWND* wnd, int unusedEDX, int32_t mouse_x, int32_t mouse_y, uint32_t unused3) {
    // Need to manually do a simple hit test for a right click in the title bar region.
    Zeal::EqUI::CXRect rect;
    Zeal::EqGame::EqGameInternal::CXWndGetClientRect(wnd, 0, &rect);
    if (mouse_y < rect.Top)
        return Zeal::EqGame::EqGameInternal::CXWndHandleRButtonDown(wnd, unusedEDX, mouse_x, mouse_y, unused3);

    ZealService* zeal = ZealService::get_instance();
    if (zeal && zeal->zone_map)
        zeal->zone_map->process_right_mouse_button_down(mouse_x, mouse_y);
    return 0;
}

// Note: This callback does appear to be functional in the TAKP eqgame.exe code.
static int __fastcall HandleWheelButtonDown(Zeal::EqUI::EQWND * wnd, int unusedEDX, int32_t mouse_x, int32_t mouse_y, uint32_t unused3) {
    ZealService* zeal = ZealService::get_instance();
    if (zeal && zeal->zone_map)
        zeal->zone_map->process_wheel_mouse_button_down(mouse_x, mouse_y);
    return 0;
}

// Just suppress other mouse events since update_mouse() polling is more reliable.
static int __fastcall DropMouseUpdate(Zeal::EqUI::EQWND* wnd, int unusedEDX, int32_t mouse_x, int32_t mouse_y, uint32_t unused3) {
    return 0;
}

// Note: This is not called through the normal handler by the client infrastructure. Instead we call it
// explicitly for now in ui_deactivate().
static void __fastcall Deactivate(Zeal::EqUI::EQWND* wnd, int unusedEDX) {
    wnd->show(0, false);
    return;
}

// Could be used to detect the map getting closed by the title bar button.
static int __fastcall OnShow(Zeal::EqUI::EQWND* wnd, int unusedEDX) {
    return Zeal::EqGame::EqGameInternal::CSidlScreenWndOnShow(wnd, unusedEDX);
}

static int __fastcall OnMove(Zeal::EqUI::EQWND* wnd, int unusedEDX, Zeal::EqUI::CXRect rect) {
    //Zeal::EqGame::print_chat("OnMove: l: %i t: %i r: %i b: %i", rect.Left, rect.Top, rect.Right, rect.Bottom);
    ZealService* zeal = ZealService::get_instance();
    if (zeal && zeal->zone_map)
        zeal->zone_map->process_on_move(rect);
    return 0;  // The default CXWnd::OnMove() just returns 0.
}

// Constrain the size to reasonable minimums.  Also verified during rendering.
static int __fastcall OnResize(Zeal::EqUI::EQWND* wnd, int unusedEDX, int width, int height) {
    //Zeal::EqGame::print_chat("OnResize: %i x %i", width, height);
    width = max(width, kWinMinSize);
    height = max(height, kWinMinSize);
    ZealService* zeal = ZealService::get_instance();
    if (zeal && zeal->zone_map)
        zeal->zone_map->process_on_resize(width, height);
    return Zeal::EqGame::EqGameInternal::CXWndOnResize(wnd, unusedEDX, width, height);
}

// Hooked to characterize and to allow debug printing when desired.
static void __fastcall OnMinimizeBox(Zeal::EqUI::EQWND* wnd, int unusedEDX) {
    Zeal::EqGame::EqGameInternal::CXWndOnMinimizeBox(wnd, unusedEDX);
}

// Not currently getting called by the framework.  Base class does nothing anyways.
static int __fastcall OnActivate(Zeal::EqUI::EQWND* wnd, int unusedEDX) {
    return Zeal::EqGame::EqGameInternal::CXWndOnActivate(wnd, unusedEDX);
}

// Hooked to characterize.
static int __fastcall AboutToDeleteWnd(Zeal::EqUI::EQWND* wnd, int unusedEDX) {
    return Zeal::EqGame::EqGameInternal::CXWndAboutToDeleteWnd(wnd, unusedEDX);
}

// Hooked to characterize and to allow debug printing when desired.
static Zeal::EqUI::CXRect* __fastcall GetMinimizedRect(Zeal::EqUI::EQWND* wnd, int unusedEDX, Zeal::EqUI::CXRect* rect) {
    return Zeal::EqGame::EqGameInternal::CXWndGetMinimizedRect(wnd, unusedEDX, rect);
}

// Hooked to characterize and to allow debug printing when desired.
static void __fastcall StoreIniInfo(Zeal::EqUI::EQWND* wnd, int unusedEDX) {
    Zeal::EqGame::EqGameInternal::CSidlScreenWndStoreIniInfo(wnd, unusedEDX);
}

// This callback is required since the Map window isn't tied into the EQ window list for creation. The
// code goes through and allocates memory and then calls the constructor of each sidl window. The
// ui CreateSidlScreen function handles that by creating a base SidlScreenWnd.
void ZoneMap::callback_init_ui()
{
    static const char* xml_file = "./uifiles/zeal/EQUI_ZealMap.xml";
    if (!wnd && std::filesystem::exists(xml_file) && ZealService::get_instance()->ui)
        wnd = ZealService::get_instance()->ui->CreateSidlScreenWnd("ZealMap");

    if (!wnd) {
        Zeal::EqGame::print_chat("Error: Failed to load %s", xml_file);
        return;
    }

    auto* vtbl = static_cast<Zeal::EqUI::SidlScreenWndVTable*>(wnd->vtbl);
    vtbl->DrawBackground = DrawBackground;
    vtbl->HandleWheelMove = HandleWheelMove;
    vtbl->HandleMouseMove = DropMouseUpdate;
    vtbl->HandleLButtonDown = HandleLButtonDown;
    vtbl->HandleLButtonUp = DropMouseUpdate;
    vtbl->HandleLButtonHeld = DropMouseUpdate;
    vtbl->HandleLButtonUpAfterHeld = DropMouseUpdate;
    vtbl->HandleRButtonDown = HandleRButtonDown;
    vtbl->HandleRButtonUp = DropMouseUpdate;
    vtbl->HandleRButtonHeld = DropMouseUpdate;
    vtbl->HandleRButtonUpAfterHeld = DropMouseUpdate;
    vtbl->HandleWheelButtonDown = HandleWheelButtonDown;
    vtbl->Deactivate = Deactivate;
    vtbl->OnShow = OnShow;
    vtbl->OnMove = OnMove;
    vtbl->OnResize = OnResize;
    vtbl->OnMinimizeBox = OnMinimizeBox;
    vtbl->GetMinimizedRect = GetMinimizedRect;
    vtbl->OnActivate = OnActivate;
    vtbl->AboutToDeleteWnd = AboutToDeleteWnd;
    vtbl->StoreIniInfo = StoreIniInfo;

    // Add sanity check constraints on the window size.
    if ((wnd->Location.Right - wnd->Location.Left < kWinMinSize) ||
        (wnd->Location.Bottom - wnd->Location.Top) < kWinMinSize) {
        Zeal::EqGame::EqGameInternal::CXWndMoveAndInvalidate(wnd, 0, 0, 0, kWinMinSize, kWinMinSize);
    }

    // Re-initialize to defaults on every reload (after character select). Also lets the enable arm.
    if (ZealService::get_instance() && ZealService::get_instance()->ini
        && ZealService::get_instance()->ini.get())
        load_ini(ZealService::get_instance()->ini.get());
    else
        Zeal::EqGame::print_chat("Error: Failed to load map ini options");

    // The EnterZone hook gets called before the init_ui() callback, so call it here after the wnd has
    // been generated so we can support enabling windows by default.
    callback_zone();
}
 

bool ZoneMap::ui_is_visible() const {
    if (!wnd)
        return false;
    return wnd->IsVisible;
}

void ZoneMap::ui_hide()
{
    if (wnd)
        Deactivate(wnd, 0);
}

bool ZoneMap::ui_show()
{
    if (!wnd)
        return false;
    if (!wnd->IsVisible)
        wnd->show(1, false);
    return wnd->IsVisible;
}

// This callback is required since the Map window isn't tied into the EQ window list for destruction.
// ui_clean() goes through a list of UI elements calling their destructors and nulling the pointers.
// - Called at StartingCharacterSelection for the new UI and looks like when switching to gamestate == 2
void ZoneMap::callback_clean_ui()
{
    if (wnd) {
        // We assume that deactivate_ui() was called by the framework already (so not needed here).
        ZealService::get_instance()->ui->DestroySidlScreenWnd(wnd);
        wnd = nullptr;  // Disables map rendering until another ui_init().
    }

    hide_external_window();  // Also take advantage to shut down the external window.
}

// This callback is required since the Map window isn't tied into the EQ window list for deactivation.
// The client code just cycles through that list invoking the Deactivate handler (0x88).
// -ui_deactivate() is called at least once before ui_init() is first called. It is
// also called on exiting a zone.
void ZoneMap::callback_deactivate_ui()
{
    if (wnd) {
        if (enabled && map_interactive_enabled && ui_is_visible())
            reenable_on_zone = true;  // Arm it to re-enable if we enter a new zone.

        Deactivate(wnd, 0);  // Directly call the vtbl entry we populate in init.
    }
}

ZoneMap::ZoneMap(ZealService* zeal, IO_ini* ini)
{
    zeal->callbacks->AddGeneric([this]() { callback_render(); }, callback_type::RenderUI);
    zeal->callbacks->AddGeneric([this]() { callback_dx_reset(); }, callback_type::DXReset);
    zeal->callbacks->AddGeneric([this]() { callback_zone(); }, callback_type::Zone);
    zeal->commands_hook->Add("/map", {}, "Controls map overlay",
        [this](const std::vector<std::string>& args) {
            return parse_command(args);
        });

    zeal->callbacks->AddGeneric([this]() { callback_clean_ui(); }, callback_type::CleanUI);
    zeal->callbacks->AddGeneric([this]() { callback_init_ui(); }, callback_type::InitUI);
    zeal->callbacks->AddGeneric([this]() { callback_deactivate_ui(); }, callback_type::DeactivateUI);

    zeal->ui->AddXmlInclude("EQUI_ZealMap.xml");  // Required for the SIDL values to load properly.
}

ZoneMap::~ZoneMap()
{
    render_release_resources();
    release_d3d_external_window();
    destroy_external_window();
}


// Static callback passed to windows that acts as a wrapper for class member.
LRESULT CALLBACK ZoneMap::static_external_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    ZoneMap* thisptr = nullptr;

    if (uMsg == WM_NCCREATE) {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        thisptr = static_cast<ZoneMap*>(lpcs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(thisptr));
    }
    else {
        thisptr = reinterpret_cast<ZoneMap*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (thisptr) {
        return thisptr->external_window_proc(hwnd, uMsg, wParam, lParam);
    }
    else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Translate Windows Virtual VK key codes to direct input.
static uint32_t virtual_key_to_dik(uint32_t vk_key) {
    static constexpr UINT kVtoSExceptions[][2] = {
        {VK_UP,DIK_UP}, {VK_DOWN,DIK_DOWN}, {VK_LEFT,DIK_LEFT}, {VK_RIGHT,DIK_RIGHT},
        {VK_INSERT,DIK_INSERT}, {VK_HOME,DIK_HOME}, {VK_PRIOR,DIK_PRIOR}, {VK_DELETE,DIK_DELETE},
        {VK_END,DIK_END}, {VK_NEXT,DIK_NEXT}, {VK_LMENU,DIK_LMENU}, {VK_RMENU,DIK_RMENU},
        {VK_LCONTROL,DIK_LCONTROL}, {VK_RCONTROL,DIK_RCONTROL}, {VK_DIVIDE,DIK_DIVIDE}
    };

    const int num_entries = sizeof(kVtoSExceptions) / sizeof(kVtoSExceptions[0]);
    for (int i = 0; i < num_entries; i++) {
        if (kVtoSExceptions[i][0] == vk_key)
            return kVtoSExceptions[i][1];
    }
    return MapVirtualKey(vk_key, 0);
}

void process_key(uint32_t vk_key, bool key_down) {
    ZealService* zeal = ZealService::get_instance();
    if (!zeal)
        return;

    uint32_t key_code = virtual_key_to_dik(vk_key);
    switch (key_code) {
    case DIK_LCONTROL:
    case DIK_RCONTROL:
    case DIK_LSHIFT:
    case DIK_RSHIFT:
    case DIK_LMENU:
    case DIK_RMENU:
        return;  // Ignore
    default:
        break;
    }

    // The EqGame keeps track of all this internally, just poll similarly for external.
    if (GetKeyState(VK_SHIFT) & 0x8000)
        key_code = key_code | 0x10000000;
    if (GetKeyState(VK_CONTROL) & 0x8000)
        key_code = key_code | 0x20000000;
    if (GetKeyState(VK_MENU) & 0x8000)
        key_code = key_code | 0x40000000;

    // Note: Not supporting the proper eqgame call for DIK_ESCAPE functionality (not a command).

    // Translate the final key code into an index into the binds.
    // Suppress some commands since we don't support typing into the chat bar from the other window.
    const int kCommandSlashIndex = 130;
    const int kCommandTellIndex = 13;
    const int kCommandReplyIndex = 59;
    const int kCommandChatHistoryUp = 79;
    const int kCommandChatHistoryDown = 80;
    const int first_index = 0;  // To be safer, could start this at 211 (first user bind). 
    const int lut_size = 256;
    const uint32_t* key_bind_pri = reinterpret_cast<const uint32_t*>(Zeal::EqGame::ptr_PrimaryKeyMap);
    const uint32_t* key_bind_alt = reinterpret_cast<const uint32_t*>(Zeal::EqGame::ptr_AlternateKeyMap);
    for (int i = first_index; i <= lut_size; ++i) {
        if (i == kCommandSlashIndex || i == kCommandTellIndex || i == kCommandReplyIndex ||
            i == kCommandChatHistoryUp || i == kCommandChatHistoryDown)
            continue;  // Skip searching for these commands.
        if (key_bind_pri[i] == key_code || key_bind_alt[i] == key_code)
            Zeal::EqGame::execute_cmd(i, key_down, 0);
    }
}

// Window processing logic.  Note that the primary game window is using DirectInput for the keyboard and mouse,
// and trying to set the cooperative level of the keyboard failed to re-route input back to the game. Re-routing
// WM key messages to the game wndproc doesn't work since it isn't DirectInput.
LRESULT CALLBACK ZoneMap::external_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
        case WM_DPICHANGED:
            return 0;  // Ignore any resizing due to a monitor scale change.
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if ((lParam & 0x40000000) == 0)  // Only send if this is a transition.
                process_key(wParam, true);
            return 0;
        case WM_KEYUP:
        case WM_SYSKEYUP:
                process_key(wParam, false);
            return 0;
        case WM_LBUTTONDOWN:
            process_left_mouse_button_down(static_cast<int16_t>(lParam & 0xffff), static_cast<int16_t>(lParam >> 16));
            return 0;
        case WM_RBUTTONDOWN:
            process_right_mouse_button_down(static_cast<int16_t>(lParam & 0xffff), static_cast<int16_t>(lParam >> 16));
            return 0;
        case WM_MBUTTONDOWN:
            process_wheel_mouse_button_down(static_cast<int16_t>(lParam & 0xffff), static_cast<int16_t>(lParam >> 16));
            return 0;
        case WM_MOUSEWHEEL:
            process_mouse_wheel(static_cast<int16_t>(wParam >> 16), wParam & 0xffff,
                static_cast<int16_t>(lParam & 0xffff), static_cast<int16_t>(lParam >> 16));
            return 0;
        case WM_GETMINMAXINFO:  // Set minimum size to avoid corner case crashes.
            reinterpret_cast<LPMINMAXINFO>(lParam)->ptMinTrackSize.x = kWinMinSize;
            reinterpret_cast<LPMINMAXINFO>(lParam)->ptMinTrackSize.y = kWinMinSize;
            return 0;
        case WM_DESTROY:
            set_enabled(false, false);  // Disables, releases resources.
            release_d3d_external_window(); // And also purge D3D resources connected to the window.
            external_hwnd = nullptr;
            break;  // Let DefWindowProc() handle the rest.
        case WM_SIZING:
            process_on_resize(0, 0);  // Just used to trigger autocenter mode.
            break;
        case WM_SYSCOMMAND:
            if ((wParam & 0xFFF0) == SC_CLOSE && enabled) {
                set_enabled(false, false);  // Disables, releases resources.
                release_d3d_external_window();  // And also purge D3D resources connected to the window.
                // Even though we never SetCapture() in zone_map, something else is which can bug out the mouse.
                ReleaseCapture();  // Calling this prevents mouse glitching behavior.
            }
            break;
        default:
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ZoneMap::process_external_window_messages() {

    if (!external_hwnd)
        return;

    MSG msg = {};
    while (PeekMessage(&msg, external_hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void ZoneMap::hide_external_window() {
    if (external_hwnd)
        ShowWindow(external_hwnd, SW_HIDE);
}

bool ZoneMap::show_external_window() {
    if (!create_external_window()) {
        Zeal::EqGame::print_chat("Error creating external map window");
        return false;
    }

    ShowWindow(external_hwnd, SW_SHOWNOACTIVATE);
    return true;
}

// Creates the external window, shows it, and acquires / initializes D3D.
bool ZoneMap::create_external_window() {
    if (external_hwnd)
        return true;

    external_hinstance = GetModuleHandleA("zeal.asi");  // Assuming this is correct hInstance.

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    if (!GetClassInfoEx(external_hinstance, kExternalWindowClassName, &wcex)) {
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;  // Not sure if these are required.
        wcex.lpfnWndProc = static_external_window_proc;
        wcex.hInstance = external_hinstance;
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.lpszClassName = kExternalWindowClassName;
        if (RegisterClassEx(&wcex) == 0) {   // Returns 0 if fails.
            Zeal::EqGame::print_chat("Error registering external map window class: %i", GetLastError());
            return false;
        }
    }

    // Stored window size is the client area (viewport).
    RECT win_rect = { .left = external_monitor_left_offset, .top = external_monitor_top_offset,
                        .right = external_monitor_left_offset + external_monitor_width,
                        .bottom = external_monitor_top_offset + external_monitor_height };
    AdjustWindowRectEx(&win_rect, WS_OVERLAPPEDWINDOW, false, NULL);
    external_hwnd = CreateWindowEx(NULL,
        kExternalWindowClassName,
        kExernalWindowTitle,
        WS_OVERLAPPEDWINDOW,
        win_rect.left, win_rect.top, win_rect.right - win_rect.left, win_rect.bottom - win_rect.top,
        NULL,
        NULL,
        external_hinstance,
        this); // Pass 'this' as lpCreateParams

    return (external_hwnd != nullptr);
}


// Acquire and initialize the Direct 3D resources for the external window.
bool ZoneMap::initialize_d3d_external_window()
{
    if (!external_hwnd)
        return false;

    RECT win_rect = { 0 };
    GetClientRect(external_hwnd, &win_rect);
    int external_width = win_rect.right - win_rect.left;
    int external_height = win_rect.bottom - win_rect.top;

    if (!external_d3d)
        external_d3d = Direct3DCreate8(D3D_SDK_VERSION); // create the Direct3D interface

    if (!external_d3d) {
        Zeal::EqGame::print_chat("Error creating external map D3D");
        release_d3d_external_window();
        reset_zone_state();
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp = { 0 }; // create a struct to hold various device information
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = external_hwnd;    // set the window to be used by Direct3D
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = external_width;    // set the width of the buffer
    d3dpp.BackBufferHeight = external_height;    // set the height of the buffer

    // create a device class using this information and the info from the d3dpp stuct
    auto result = external_d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        external_hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &external_d3ddev);

    if (FAILED(result)) {
        Zeal::EqGame::print_chat("Error creating external map device: %d (%d x %d)",
            result, external_width, external_height);
        release_d3d_external_window();
        reset_zone_state();
        return false;
    }

    return true;
}

void ZoneMap::release_d3d_external_window() {
    if (external_d3ddev) {
        external_d3ddev->Release(); // close and release the 3D device
        external_d3ddev = nullptr;
    }
    if (external_d3d) {
        external_d3d->Release(); // close and release Direct3D
        external_d3d = nullptr;
    }
}

void ZoneMap::destroy_external_window() {
    if (external_hwnd != nullptr) {
        DestroyWindow(external_hwnd);
        external_hwnd = nullptr;
        UnregisterClass(kExternalWindowClassName, NULL);
    }
}

