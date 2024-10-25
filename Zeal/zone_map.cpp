#include "zone_map.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include "zone_map_data.h"
#include "bitmap_font.h"
#include <fstream>
#include <string>


// Possible enhancements:
// - Look into intermittent z-depth clipping due to walls or heads/faces
// - External map window issues:
//   - window message queue is not working and so not supporting window
//     resizing or things like window panning or scroll wheel zooming

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


namespace {

// Constants used in DirectX resource allocation.
static constexpr int kBackgroundCount = 2;  // Two triangles in split mode.
static constexpr int kBackgroundVertices = 3 + (kBackgroundCount - 1);  // Two triangles using split.
static constexpr int kMarkerCount = 4;  // Four triangles.
static constexpr int kPositionCount = 2;  // Two triangles.
static constexpr int kPositionVertices = kPositionCount * 3; // Fixed triangle list.
static constexpr int kRaidMaxMembers = 72;
static constexpr int kRaidPositionVertices = 3; // Fixed triangle list with one triangle.
static constexpr int kMaxDynamicLabels = 10;
static constexpr int kPositionBufferSize = sizeof(ZoneMap::MapVertex) * (kPositionVertices
                                            * (EQ_NUM_GROUP_MEMBERS + 1) + kRaidPositionVertices * kRaidMaxMembers);

static constexpr DWORD kMapVertexFvfCode = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

// Group member position support.
using GroupNameArrayType = char[0x40];
using GroupEntityPtrArrayType = Zeal::EqStructures::Entity*;

GroupEntityPtrArrayType* groupEntityPtrs = reinterpret_cast<GroupEntityPtrArrayType*>(0x7913F8);
GroupNameArrayType* groupNames = reinterpret_cast<GroupNameArrayType*>(0x7912B5);


}  // namespace


// Releases the manually managed DirectX related resources.
void ZoneMap::render_release_resources() {
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
}

// Use the map_rect values and render target to configure the viewport rectangle.
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
        description.Width = 320;  // "safe" fallback values.
        description.Height = 320;
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

    const DWORD rect_left = static_cast<DWORD>(std::floor(map_rect_left * description.Width));
    const DWORD rect_right = static_cast<DWORD>(std::ceil(map_rect_right * description.Width));
    const DWORD rect_top = static_cast<DWORD>(std::floor(map_rect_top * description.Height));
    const DWORD rect_bottom = static_cast<DWORD>(std::ceil(map_rect_bottom * description.Height));

    viewport = D3DVIEWPORT8{ .X = rect_left, .Y = rect_top,
        .Width = rect_right - rect_left, .Height = rect_bottom - rect_top,
        .MinZ = 0.0f, .MaxZ = 1.0f };
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

    const Vec3 position = Zeal::EqGame::get_self()->Position;
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

    D3DXMATRIX mat_scale;
    D3DXMatrixScaling(&mat_scale, scale_factor, scale_factor, 1.f);
    D3DXMATRIX mat_offset;
    D3DXMatrixTranslation(&mat_offset, offset_x, offset_y, 0.0f);
    mat_model2world = mat_scale * mat_offset;

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

// Populates the "static" per zone line_vertex_buffer and labels list. Also allocates the
// position_vertex_buffer (worst-case sized).
void ZoneMap::render_load_map(IDirect3DDevice8& device, const ZoneMapData& zone_map_data) {
    render_release_resources();  // Forces update of all graphics.

    const int kMaxLineCount = zone_map_data.num_lines;  // Allocate a buffer assuming all visible.
    std::unique_ptr<MapVertex[]> line_vertices = std::make_unique<MapVertex[]>(kMaxLineCount * 2);

    bool z_filtering = (map_level_zone_id == zone_id);
    int level_id = z_filtering ? zone_map_data.levels[map_level_index].level_id : kZoneMapInvalidLevelId;
    clip_max_z = z_filtering ? zone_map_data.levels[map_level_index].max_z : zone_map_data.max_z;
    clip_max_z += 10;  // Pad up for player height and other labels.
    clip_min_z = z_filtering ? zone_map_data.levels[map_level_index].min_z : zone_map_data.min_z;
    line_count = 0;  // Tracks number of visible (after clipping) lines.
    for (int i = 0; i < kMaxLineCount; ++i) {
        if (z_filtering) {
            if (zone_map_data.lines[i].level_id == kZoneMapInvalidLevelId) {
                // Filter all unknown lines based on z height range.
                int line_max_z = max(zone_map_data.lines[i].z0, zone_map_data.lines[i].z1);
                int line_min_z = min(zone_map_data.lines[i].z0, zone_map_data.lines[i].z1);
                if (line_max_z < clip_min_z || line_min_z > clip_max_z)
                    continue; // Skip if line does not cross level.
            } else if (zone_map_data.lines[i].level_id != level_id)
                    continue;  // Skip display of all other map levels.
        }

        auto color = D3DCOLOR_XRGB(zone_map_data.lines[i].red, zone_map_data.lines[i].green, zone_map_data.lines[i].blue);
        if (color == D3DCOLOR_XRGB(0, 0, 0) &&
            (!external_enabled || map_background_state == BackgroundType::kDark)) {
            color = D3DCOLOR_XRGB(64, 64, 64);  // Increase visibility of black lines.
        }

        line_vertices[2 * line_count].x = zone_map_data.lines[i].x0;
        line_vertices[2 * line_count].y = zone_map_data.lines[i].y0;
        line_vertices[2 * line_count].z = 0.5f;
        line_vertices[2 * line_count].color = color;
        line_vertices[2 * line_count + 1].x = zone_map_data.lines[i].x1;
        line_vertices[2 * line_count + 1].y = zone_map_data.lines[i].y1;
        line_vertices[2 * line_count + 1].z = 0.5f;
        line_vertices[2 * line_count + 1].color = color;
        line_count++;
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

    const int line_buffer_size = sizeof(MapVertex) * line_count * 2;
    const int background_buffer_size = sizeof(MapVertex) * kBackgroundVertices;
    // Create a Vertex buffer and copy the map line segments over.
    if (FAILED(device.CreateVertexBuffer(line_buffer_size + background_buffer_size,
        D3DUSAGE_WRITEONLY,
        kMapVertexFvfCode,
        D3DPOOL_MANAGED,
        &line_vertex_buffer))) {
        return;
    }

    BYTE* data = nullptr;
    if (FAILED(line_vertex_buffer->Lock(0, 0, &data, D3DLOCK_DISCARD))) {
        enabled = false;
        render_release_resources();
        return;
    }
    memcpy(data, background_vertices, background_buffer_size);
    memcpy(data + background_buffer_size, (const void*)line_vertices.get(), line_buffer_size);
    line_vertex_buffer->Unlock();

    // Create a worst-case sized Vertex buffer for live position updates.
    if (FAILED(device.CreateVertexBuffer(kPositionBufferSize,
        D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
        kMapVertexFvfCode,
        D3DPOOL_DEFAULT,
        &position_vertex_buffer))) {
        return;
    }

    render_load_labels(device, zone_map_data);
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
        background_color = D3DCOLOR_ARGB(alpha, 240, 180, 140);  // Tan.
    }
    else if (external_enabled && map_background_state == BackgroundType::kClear) {
        background_color = D3DCOLOR_ARGB(alpha, 85, 90, 96);  // Default gray.
    }
    return background_color;
}

// Loads the POI labels from the ZoneMapData with some level-based filtering.
void ZoneMap::render_load_labels(IDirect3DDevice8& device, const ZoneMapData& zone_map_data) {
    labels_list.clear();

    int num_labels_to_scan = zone_map_data.num_labels;
    if (map_labels_mode == LabelsMode::kOff)
        num_labels_to_scan = 0;  // Disable the scan below.

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
                constexpr std::string_view succor_string{ "succor" };
                std::transform(label_string.begin(), label_string.end(), label_string.begin(), ::tolower);
                if (label_string.find(circle_string) == std::string::npos &&
                    label_string.find(ring_string) == std::string::npos &&
                    label_string.find(portal_string) == std::string::npos &&
                    label_string.find(spires_string) == std::string::npos &&
                    label_string.find(succor_string) == std::string::npos)
                    continue;  // Skip label.
            }
        }
        labels_list.push_back(&label);
    }

    if (dynamic_labels_zone_id != zone_id)
        dynamic_labels_list.clear();
}

// Loads the bitmap font for real-time text rendering to screen.
void ZoneMap::render_load_font(IDirect3DDevice8& device) {
    if (bitmap_font)
        return;

    if (font_filename.empty())  // Used to signal error state below.
        return;

    std::string full_filename = "uifiles/zeal/fonts/" + font_filename + ".spritefont";
    bitmap_font = std::make_unique<BitmapFont>(device, full_filename.c_str());
    if (bitmap_font->is_valid())
        return;  // Font is ready, all done.

    bitmap_font.reset();  // Null the bad font to disable attempts to use.
    font_filename = "";  // Clear to indicate invalid and do not retry.
    Zeal::EqGame::print_chat("Failed to load font file: %s", full_filename.c_str());
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

    // Configure for 2D drawing with alpha blending ready but disabled.
    D3DRenderStateStash render_state(device);
    render_state.store_and_modify({ D3DRS_CULLMODE, D3DCULL_NONE });
    render_state.store_and_modify({ D3DRS_ALPHABLENDENABLE, FALSE });
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

    if (line_count) {
        device.SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
        device.DrawPrimitive(D3DPT_LINELIST, kBackgroundVertices, line_count);
    }

    if (marker_vertex_buffer) {
        device.SetStreamSource(0, marker_vertex_buffer, sizeof(MapVertex));
        device.DrawPrimitive(D3DPT_TRIANGLELIST, 0, kMarkerCount);
    }

    render_labels(device);

    render_positions(device);

    device.SetTransform(D3DTS_PROJECTION, &projection_original);
    device.SetTransform(D3DTS_WORLD, &world_original);
    device.SetTransform(D3DTS_VIEW, &view_original);
    render_state.restore_state();
    device.SetViewport(&original_viewport);
}

// Handles the rendering of the map background tinting.
void ZoneMap::render_background(IDirect3DDevice8& device) {
    // Enable alpha blending for the background
    D3DRenderStateStash render_state(device);
    render_state.store_and_modify({ D3DRS_ALPHABLENDENABLE, TRUE });

    // Background vertices are stored at the start of the line_vertex_buffer.
    device.SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
    device.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, kBackgroundCount);

    render_state.restore_state();
}

// Handles the rendering of the labels_list and dynamic_labels_list.
void ZoneMap::render_labels(IDirect3DDevice8& device) {
    bool add_level_label = (map_level_zone_id == zone_id);
    if (!add_level_label && labels_list.empty() && dynamic_labels_list.empty())
        return;

    render_load_font(device);
    if (!bitmap_font)
        return;

    if (add_level_label) {
        char level_label_string[20];
        snprintf(level_label_string, sizeof(level_label_string), "Level: %i", map_level_index);
        level_label_string[sizeof(level_label_string) - 1] = 0;  // Ensure null-terminated.
        Vec2 level_size = bitmap_font->measure_string(level_label_string);
        const float indent_x = scale_pixels_to_model(5 + 0.5f * level_size.x);
        const float indent_y = scale_pixels_to_model(5 + 0.5f * level_size.y);
        render_label_text(level_label_string, static_cast<int16_t>(clip_min_y + indent_y),
            static_cast<int16_t>(clip_min_x + indent_x), D3DCOLOR_XRGB(255, 255, 255));
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
}

// Handles writing a text label at map coordinates y and x to the screen.
void ZoneMap::render_label_text(const char * label, int map_y, int map_x, D3DCOLOR font_color,
                                LabelType label_type, Vec2 offset_pixels) {
    if (!bitmap_font)  // Programming error if this happens but paranoid checking.
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
    const int kHalfHeight = 10;  // Centered so just has to be sufficiently large.
    int length = strlen(label);
    if (length > 20) {
        length = 20;  // Truncate it to 20.
        for (int i = 1; i < length - 1; ++i) {
            if (label[i] == '_' && label[i + 1] == '(')  // Extra info in () to drop.
                length = i;  // Truncates and breaks loop.
        }
    }
    char short_label[21];
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
    if (label_type != LabelType::Normal) {
        Vec2 size = bitmap_font->measure_string(short_label);
        const float half_width = size.x * 0.5f;
        const float half_height = size.y * 0.5f;
        label_x = max(view_left + half_width, min(view_right - half_width, label_x));
        label_y = max(view_top + half_height, min(view_bottom - half_height, label_y));
    }
    bitmap_font->queue_string(short_label, Vec2(label_x, label_y), true, font_color);
    if (label_type == LabelType::AddMarker)
        bitmap_font->queue_string("+", Vec2(label_screen[0], label_screen[1]), true, font_color);
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
    if (!map_show_group)
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
        Zeal::EqStructures::Entity* member = groupEntityPtrs[i];
        if ((strlen(groupNames[i]) == 0) || !member)
            continue;  // Not a valid group member (or member = nullptr when out of zone).

        // Position is y,x,z.
        add_position_marker_vertices(-member->Position.x, -member->Position.y, member->Heading, size,
            kGroupColorLut[i], vertices);
    }
}

// Adds a label for each group member if enabled.
void ZoneMap::render_group_member_labels(IDirect3DDevice8& device) {
    if (!map_show_group ||
        (!map_show_all_names_override && map_group_labels_mode == GroupLabelsMode::kOff))
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

    for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; ++i) {
        Zeal::EqStructures::Entity* member = groupEntityPtrs[i];
        if ((strlen(groupNames[i]) == 0) || !member)
            continue;  // Not a valid group member (or member = nullptr when out of zone).

        // Writes the character name or group number (F2 - F6) centered at the character position.
        int loc_y = static_cast<int>(member->Position.x + 0.5f);  // Position is y,x,z.
        int loc_x = static_cast<int>(member->Position.y + 0.5f);  // Also need to negate it below.
        constexpr int kShortNameLength = 5;  // Just show the first 3 letters of the name.
        char label[kShortNameLength + 1] = { 0 };  // Null-terminate.
        if (!map_show_all_names_override && map_group_labels_mode == GroupLabelsMode::kNumbers)
            label[0] = static_cast<uint8_t>(i) + '2';
        else
            for (int j = 0; j < kShortNameLength; ++j)
                label[j] = groupNames[i][j];
        render_label_text(label,-loc_y, -loc_x, color, LabelType::PositionLabel, offset_pixels);
    }
    bitmap_font->flush_queue_to_screen();
}

// Adds a label for each raid member if enabled.
void ZoneMap::render_raid_member_labels(IDirect3DDevice8 & device) {
    if (!map_show_raid || !map_show_all_names_override)
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

    const Zeal::EqStructures::RaidMember* raidMembers =
        reinterpret_cast<const Zeal::EqStructures::RaidMember*>(Zeal::EqGame::RaidMemberList);

    // Just add a label for members, including self, since this is intended to be for transient use.
    for (int i = 0; i < kRaidMaxMembers; ++i) {
        const auto& member = raidMembers[i];
        if (strlen(member.Name) == 0)
            continue;
        auto entity = entity_manager->Get(member.Name);
        if (!entity)
            continue;  // Could be out of zone.

        int loc_y = static_cast<int>(entity->Position.x + 0.5f);  // Position is y,x,z.
        int loc_x = static_cast<int>(entity->Position.y + 0.5f);  // Also need to negate it below.
        constexpr int kShortNameLength = 5;  // Just show the first 3 letters of the name.
        char label[kShortNameLength + 1] = { 0 };  // Null-terminate.
        for (int j = 0; j < kShortNameLength; ++j)
            label[j] = entity->Name[j];
        render_label_text(label, -loc_y, -loc_x, color, LabelType::PositionLabel, offset_pixels);
    }
    bitmap_font->flush_queue_to_screen();
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

// Goes through raid member list adding their position vertices if enabled.
void ZoneMap::add_raid_member_position_vertices(std::vector<MapVertex>& vertices) const {
    if (!map_show_raid)
        return;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    auto entity_manager = ZealService::get_instance()->entity_manager.get();  // Short-term ptr.
    if (!entity_manager || !self)
        return;

    const float size = convert_size_fraction_to_model(position_size);

    // TODO: Review color coding to be more distinct. Possibly by class.
    const DWORD kUngrouped = 0xffffffff;
    const D3DCOLOR kColorLeader = D3DCOLOR_XRGB(255, 153, 153);
    const D3DCOLOR kColorUngrouped = D3DCOLOR_XRGB(240, 240, 240);

    const Zeal::EqStructures::RaidMember* raidMembers =
        reinterpret_cast<const Zeal::EqStructures::RaidMember*>(Zeal::EqGame::RaidMemberList);

    // Disable the markers for the same group if show_group is active.
    DWORD self_group_number = kUngrouped - 1;  // Unique default number.
    if (map_show_group) {
        // Consider optimizing this with a cache if there's a way to detect raid member changes.
        for (int i = 0; i < kRaidMaxMembers; ++i) {
            if (strcmp(raidMembers[i].Name, self->Name) == 0) {
                if (raidMembers[i].GroupNumber != kUngrouped)
                    self_group_number = raidMembers[i].GroupNumber;
                break;
            }
        }
    }
    for (int i = 0; i < kRaidMaxMembers; ++i) {
        const auto& member = raidMembers[i];
        if ((member.GroupNumber == self_group_number) || (strlen(member.Name) == 0)
            || (strcmp(member.Name, self->Name) == 0))
            continue;  // In same group or no raid member or it is self.
        auto entity = entity_manager->Get(member.Name);
        if (!entity)
            continue;  // Could be out of zone.

        auto color = member.IsGroupLeader ? kColorLeader :
            (member.GroupNumber == kUngrouped ? kColorUngrouped :
                D3DCOLOR_XRGB(224, 224, 128 + member.GroupNumber * 8));
        // Position is y,x,z.
        add_raid_marker_vertices(entity->Position, size, color, vertices);
    }
}

// Handles updating and rendering of self, group, and raid positions.
void ZoneMap::render_positions(IDirect3DDevice8& device) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!position_vertex_buffer || !self) {
        return;
    }

    // Populate a vector with D3DPT_TRIANGLELIST vertices.
    std::vector<MapVertex> position_vertices;
    add_raid_member_position_vertices(position_vertices);
    add_group_member_position_vertices(position_vertices);

    int other_triangle_vertices = position_vertices.size();

    const float size = convert_size_fraction_to_model(position_size);

    // Use default cursor color if levels not active or within the z clipping range.
    Vec3 position = self->Position;
    bool use_default_color = (map_level_zone_id != zone_id) ||
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

    // First draw the "other" (raid, group) markers.
    int other_triangle_count = other_triangle_vertices / 3; // D3DPT_TRIANGLELIST
    if (other_triangle_count)
        device.DrawPrimitive(D3DPT_TRIANGLELIST, 0, other_triangle_count);

    // Then draw the raid and group labels (on top of markers but below self marker).
    render_raid_member_labels(device);
    render_group_member_labels(device);

    // And finally draw the self marker. Three vertices per triangle in D3DPT_TRIANGLELIST.
    int self_triangle_count = position_vertices.size() / 3 - other_triangle_count;
    device.DrawPrimitive(D3DPT_TRIANGLELIST, other_triangle_vertices, self_triangle_count);
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

// Perform a software (manual) vertex transformation from model (map data) to world.
Vec3 ZoneMap::transform_model_to_world(const Vec3& model) const {
    const float w = 1.0f;  // Needed for translation.
    return Vec3(
        mat_model2world(0, 0) * model[0] + mat_model2world(1, 0) * model[1]
            + mat_model2world(2, 0) * model[2] + mat_model2world(3, 0) * w,
        mat_model2world(0, 1) * model[0] + mat_model2world(1, 1) * model[1]
            + mat_model2world(2, 1) * model[2] + mat_model2world(3, 1) * w,
        mat_model2world(0, 2) * model[0] + mat_model2world(1, 2) * model[1]
            + mat_model2world(2, 2) * model[2] + mat_model2world(3, 2) * w
        );
}

// Perform a software (manual) vertex transformation from model (map data) to absolute screen.
Vec3 ZoneMap::transform_model_to_screen(const Vec3& model) const {
    Vec3 screen = transform_model_to_world(model);

    screen[0] += static_cast<float>(viewport.X + viewport.Width * 0.5f);
    screen[1] += static_cast<float>(viewport.Y + viewport.Height * 0.5f);
    return screen;
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
    Vec3 marker[kNumVertices] = {{0, 0, 0}, {short_size, size, 0}, {-short_size, size, 0},
                                    {0, 0, 0}, {-short_size, -size, 0}, {short_size, -size, 0},
                                    {0, 0, 0}, {size, -short_size, 0}, {size, short_size, 0},
                                    {0, 0, 0}, {-size, short_size, 0}, {-size, -short_size, 0}
    };

    MapVertex marker_vertices[kNumVertices];
    for (int i = 0; i < kNumVertices; ++i) {
        marker_vertices[i].x = marker[i].x + marker_x,
        marker_vertices[i].y = marker[i].y + marker_y,
        marker_vertices[i].z = 0.5f;
        marker_vertices[i].color = D3DCOLOR_XRGB(255, 0, 0);
    }

    // Create a Vertex buffer and copy the triangle vertices.
    int marker_buffer_size = sizeof(MapVertex) * kNumVertices;
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
    memcpy(data, marker_vertices, marker_buffer_size);
    marker_vertex_buffer->Unlock();
}

RECT ZoneMap::calc_external_window_client_rect() {
    max_viewport_width = 320;  // Safe defaults.
    max_viewport_height = 320;
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
    else {
        // Verify that the saved external_monitor offsets are valid and use to pick monitor.
        POINT point{ .x = external_monitor_left_offset, .y = external_monitor_top_offset };
        HMONITOR hmonitor = MonitorFromPoint(point, MONITOR_DEFAULTTONULL);
        MONITORINFO monitor_info = { 0 };
        monitor_info.cbSize = sizeof(monitor_info);
        if (hmonitor != NULL && GetMonitorInfoA(hmonitor, &monitor_info)) {
            max_viewport_width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
            max_viewport_height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
        }
        else {
            // Fallback to primary.
            external_monitor_left_offset = 16;  // Client area, so leave a little margin.
            external_monitor_top_offset = 16;
            max_viewport_width = GetSystemMetrics(SM_CXSCREEN);
            max_viewport_height = GetSystemMetrics(SM_CYSCREEN);
        }
    }

    const LONG rect_left = external_monitor_left_offset;
    const LONG rect_right = external_monitor_left_offset +
        static_cast<LONG>(std::ceil((map_rect_right - map_rect_left) * max_viewport_width));
    const LONG rect_top = external_monitor_top_offset;
    const LONG rect_bottom = external_monitor_top_offset + 
        static_cast<LONG>(std::ceil((map_rect_bottom - map_rect_top)* max_viewport_height));

    RECT win_rect = {.left = rect_left, .top = rect_top, .right = rect_right, .bottom = rect_bottom };
    return win_rect;
}

// The external window message processing queue is not entirely functional, so
// this function manually keeps the external d3d state (render target size) in
// sync with the external window.
void ZoneMap::synchronize_external_window() {

    if (!enabled || !external_enabled || !external_hwnd)
        return;

    // Moving the window between displays with different DPI triggers an
    // auto-rescaling that we don't want, so monitor for it. Preferably 
    // trigger this (or suppress the change) using windows message queue,
    // or alternatively trigger on a monitor ID change.
    RECT target_rect = calc_external_window_client_rect();  // Monitor resolution dependent.
    int target_width = target_rect.right - target_rect.left;
    int target_height = target_rect.bottom - target_rect.top;

    // Keep offsets synchronized.
    POINT corner = { 0 };
    ClientToScreen(external_hwnd, &corner);
    external_monitor_left_offset = corner.x;
    external_monitor_top_offset = corner.y;

    // First check if the Window itself needs to be resized.
    RECT win_rect = { 0 };
    GetClientRect(external_hwnd, &win_rect);
    bool force_resize = ((win_rect.right - win_rect.left) != target_width) ||
        ((win_rect.bottom - win_rect.top) != target_height);

    if (force_resize) {
        win_rect.left = external_monitor_left_offset;
        win_rect.top = external_monitor_top_offset;
        win_rect.right = win_rect.left + target_width;
        win_rect.bottom = win_rect.top + target_height;
        if (!AdjustWindowRectEx(&win_rect, WS_CAPTION, false, NULL))
            Zeal::EqGame::print_chat("error adjusting window rect");
        else
            SetWindowPos(external_hwnd, NULL, win_rect.left, win_rect.top,
                win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, NULL);
    }

    // And then check if the existing d3d framebuffer needs to be resized.
    if (external_d3ddev && (viewport.Height != target_height || viewport.Width != target_width)) {
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


// System callback to execute the map rendering.
void ZoneMap::callback_render() {
    process_external_window_messages();  // Handle even when map disabled.

    if (!enabled || !Zeal::EqGame::is_in_game())
        return;

    // The external window message queue does not appear to be working.
    // The call below manages the external window (if active) and keeps the
    // D3D render objects in sync with the window size.
    synchronize_external_window();

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    IDirect3DDevice8* device = external_enabled ?
        external_d3ddev : ZealService::get_instance()->dx->GetDevice();
    if (!self || !device)
        return;

    const ZoneMapData* zone_map_data = get_zone_map(self->ZoneId);
    if (!zone_map_data) {
        return;
    }

    // ZealMap is responsible for the updates when using the external d3d device.
    if (external_enabled) {
        auto background_color = get_background_color();
        device->Clear(0, NULL, D3DCLEAR_TARGET, background_color, 1.0f, 0);
        if (FAILED(device->BeginScene()))   // begins the 3D scene
            return;
    }

    if (zone_id != self->ZoneId) {
        zone_id = self->ZoneId;
        render_load_map(*device, *zone_map_data);
    }

    render_update_viewport(*device);  // Updates size and position of output viewport.
    render_update_transforms(*zone_map_data);  // Updates scaling, offsets, and clipping.

    // Add the marker if it is empty and this is the right zone.
    if (zone_id != kInvalidZoneId && marker_zone_id == zone_id
        && marker_vertex_buffer == nullptr) {
        render_update_marker_buffer(*device);
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

     if (dynamic_labels_zone_id != zone_id) {
        dynamic_labels_list.clear();
    }
    dynamic_labels_zone_id = zone_id;
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
    if (!internal_map || (map_data_mode == MapDataMode::kInternal))
        return internal_map;

    auto it = map_data_cache.find(zone_id);
    if (it != map_data_cache.end())
        if (!it->second || !it->second->zone_map_data || !it->second->zone_map_data->num_lines)
            return internal_map;  // Failed previous search so return default.
        else
            return it->second->zone_map_data.get();  // Note: Sharing a naked pointer here.

    // Not in cache, so try to load it. Note the internal_map->name field is required.
    auto new_map = std::make_unique<CustomMapData>();

    // Primary file must exist.
    std::string filename = "map_files/" + std::string(internal_map->name) + ".txt";
    if (!add_map_data_from_file(filename, *new_map)) {
        map_data_cache[zone_id] = nullptr;  // Flag it as a failed load.
        return internal_map;
    }

    // Optional data from a second file (typically poi's).
    std::string filename_1 = "map_files/" + std::string(internal_map->name) + "_1.txt";
    add_map_data_from_file(filename_1, *new_map);

    if (map_data_mode == MapDataMode::kBoth)
        add_map_data_from_internal(*internal_map, *new_map);
    else if (new_map->lines.size() == 0) {
        map_data_cache[zone_id] = nullptr;  // Flag it as a failed load.
        return internal_map;
    }

    // Analyzes all added data to populate the final ZoneMapData structure.
    assemble_zone_map(internal_map->name, *new_map);

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
    if (!_enabled) {
        render_release_resources();
        zone_id = kInvalidZoneId;  // Triggers update when re-enabled.
        hide_external_window();
    }
    else if (!enabled && external_enabled) {
        render_release_resources();  // Just in case cleanup.
        zone_id = kInvalidZoneId;  // Triggers reload of map.
        if (!show_external_window()) {
            Zeal::EqGame::print_chat("Falling back to internal map overlay");
            external_enabled = false;
        }
    }
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
        if (was_enabled)
            set_enabled(true, false);
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapExternalEnabled", external_enabled);

    update_ui_options();
}

void ZoneMap::set_show_group(bool enable, bool update_default) {
    if (map_show_group != enable) {
        map_show_group = enable;
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapShowGroup", map_show_group);

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

bool ZoneMap::set_group_labels_mode(int mode_in, bool update_default) {
    GroupLabelsMode::e mode{ mode_in };
    mode = max(GroupLabelsMode::kFirst, min(GroupLabelsMode::kLast, mode));

    map_group_labels_mode = mode;  // Dynamic labels so reload not needed.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapGroupLabelsMode", mode);

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
    zone_id = kInvalidZoneId;  // Triggers reload.
    map_level_zone_id = kInvalidZoneId;  // Reset (may be out of date).
    marker_zone_id = kInvalidZoneId;
    dynamic_labels_zone_id = kInvalidZoneId;

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapDataMode", mode);

    update_ui_options();
    return true;
}

bool ZoneMap::set_position_size(int new_size_percent, bool update_default) {
    position_size = max(0.01f, min(kMaxPositionSize, new_size_percent / 100.f * kMaxPositionSize));

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapPositionSize", position_size);

    update_ui_options();
    return true;  // Just clamp and report success.
}

bool ZoneMap::set_marker_size(int new_size_percent, bool update_default) {
    marker_size = max(0.01f, min(kMaxMarkerSize, new_size_percent / 100.f * kMaxMarkerSize));
    clear_marker(false);  // Release resources but keep validity state to trigger an update.
    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapMarkerSize", marker_size);

    update_ui_options();
    return true;  // Just clamp and report success.
}

void ZoneMap::clear_marker(bool invalidate_zone_id) {
    if (invalidate_zone_id)
        marker_zone_id = kInvalidZoneId;
    if (marker_vertex_buffer) {
        marker_vertex_buffer->Release();
        marker_vertex_buffer = nullptr;
    }
}

void ZoneMap::set_marker(int y, int x) {
    clear_marker();  // Release any resources (also triggers update).

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return;
    marker_zone_id = self->ZoneId;
    marker_x = -x;  // Negate game coordinates to model.
    marker_y = -y;
}

void ZoneMap::toggle_zoom() {
    const int kNumDefaultZoomFactors = 4;
    const float kDefaultZoomFactors[kNumDefaultZoomFactors] = { 2.f, 4.f, 8.f };

    // Assumes zoom factors are monotonic and look for the first zoom higher
    // than the current state.  Wraps back to 100% if beyond max zoom.
    float new_zoom_factor = 1.f;
    for (int i = 0; i < kNumDefaultZoomFactors; ++i) {
        if (zoom_factor < kDefaultZoomFactors[i]) {
            new_zoom_factor = kDefaultZoomFactors[i];
            break;
        }
    }
    clear_marker(false);  // Marker size is dependent on scale_factor.
    zoom_factor = new_zoom_factor;
    update_ui_options();
}

bool ZoneMap::set_zoom(int zoom_percent) {
    clear_marker(false);  // Marker size is dependent on scale_factor.
    zoom_factor = zoom_percent * 0.01f;
    zoom_factor = min(100.f, max(1.f, zoom_factor));

    update_ui_options();
    return true;  // Just clamp and return success.
}

void ZoneMap::load_ini(IO_ini* ini)
{
    if (!ini->exists("Zeal", "MapEnabled"))
        ini->setValue<bool>("Zeal", "MapEnabled", false);
    if (!ini->exists("Zeal", "MapExternalEnabled"))
        ini->setValue<bool>("Zeal", "MapExternalEnabled", false);
    if (!ini->exists("Zeal", "MapExternalLeftOffset"))
        ini->setValue<int>("Zeal", "MapExternalLeftOffset", 16);
    if (!ini->exists("Zeal", "MapExternalTopOffset"))
        ini->setValue<int>("Zeal", "MapExternalTopOffset", 16);
    if (!ini->exists("Zeal", "MapShowGroup"))
        ini->setValue<bool>("Zeal", "MapShowGroup", true);
    if (!ini->exists("Zeal", "MapShowRaid"))
        ini->setValue<bool>("Zeal", "MapShowRaid", false);
    if (!ini->exists("Zeal", "MapDataMode"))
        ini->setValue<int>("Zeal", "MapDataMode", static_cast<int>(MapDataMode::kInternal));
    if (!ini->exists("Zeal", "MapBackgroundState"))
        ini->setValue<int>("Zeal", "MapBackgroundState", static_cast<int>(BackgroundType::kClear));
    if (!ini->exists("Zeal", "MapBackgroundAlpha"))
        ini->setValue<float>("Zeal", "MapBackgroundAlpha", kDefaultBackgroundAlpha);
    if (!ini->exists("Zeal", "MapAlignment"))
        ini->setValue<int>("Zeal", "MapAlignment", static_cast<int>(AlignmentType::kFirst));
    if (!ini->exists("Zeal", "MapLabelsMode"))
        ini->setValue<int>("Zeal", "MapLabelsMode", static_cast<int>(LabelsMode::kOff));
    if (!ini->exists("Zeal", "MapGroupLabelsMode"))
        ini->setValue<int>("Zeal", "MapGroupLabelsMode", static_cast<int>(GroupLabelsMode::kOff));
    if (!ini->exists("Zeal", "MapRectTop"))
        ini->setValue<float>("Zeal", "MapRectTop", kDefaultRectTop);
    if (!ini->exists("Zeal", "MapRectLeft"))
        ini->setValue<float>("Zeal", "MapRectLeft", kDefaultRectLeft);
    if (!ini->exists("Zeal", "MapRectBottom"))
        ini->setValue<float>("Zeal", "MapRectBottom", kDefaultRectBottom);
    if (!ini->exists("Zeal", "MapRectRight"))
        ini->setValue<float>("Zeal", "MapRectRight", kDefaultRectRight);
    if (!ini->exists("Zeal", "MapPositionSize"))
        ini->setValue<float>("Zeal", "MapPositionSize", kDefaultPositionSize);
    if (!ini->exists("Zeal", "MapMarkerSize"))
        ini->setValue<float>("Zeal", "MapMarkerSize", kDefaultMarkerSize);
    if (!ini->exists("Zeal", "MapFontFilename"))
        ini->setValue<std::string>("Zeal", "MapFontFilename", std::string("default"));

    // TODO: Protect against corrupted ini file (like a boolean instead of float).
    enabled = ini->getValue<bool>("Zeal", "MapEnabled");
    external_enabled = ini->getValue<bool>("Zeal", "MapExternalEnabled");
    // Force map to start up disabled in external window mode, which will require a manual
    // enable that will happen after the primary EQ D3D is setup and running.
    enabled = enabled && !external_enabled;
    external_monitor_left_offset = ini->getValue<int>("Zeal", "MapExternalLeftOffset");
    external_monitor_top_offset = ini->getValue<int>("Zeal", "MapExternalTopOffset");
    map_show_group = ini->getValue<bool>("Zeal", "MapShowGroup");
    map_show_raid = ini->getValue<bool>("Zeal", "MapShowRaid");
    map_data_mode = MapDataMode::e(ini->getValue<int>("Zeal", "MapDataMode"));
    map_background_state = BackgroundType::e(ini->getValue<int>("Zeal", "MapBackgroundState"));
    map_background_alpha = ini->getValue<float>("Zeal", "MapBackgroundAlpha");
    map_alignment_state = AlignmentType::e(ini->getValue<int>("Zeal", "MapAlignment"));
    map_labels_mode = LabelsMode::e(ini->getValue<int>("Zeal", "MapLabelsMode"));
    map_group_labels_mode = GroupLabelsMode::e(ini->getValue<int>("Zeal", "MapGroupLabelsMode"));
    map_rect_top = ini->getValue<float>("Zeal", "MapRectTop");
    map_rect_left = ini->getValue<float>("Zeal", "MapRectLeft");
    map_rect_bottom = ini->getValue<float>("Zeal", "MapRectBottom");
    map_rect_right = ini->getValue<float>("Zeal", "MapRectRight");
    position_size = ini->getValue<float>("Zeal", "MapPositionSize");
    marker_size = ini->getValue<float>("Zeal", "MapMarkerSize");
    font_filename = ini->getValue<std::string>("Zeal", "MapFontFilename");

    // Sanity clamp ini values.
    map_data_mode = max(MapDataMode::kFirst, min(MapDataMode::kLast, map_data_mode));
    map_background_state = max(BackgroundType::kFirst, min(BackgroundType::kLast, map_background_state));
    map_background_alpha = max(0, min(1.f, map_background_alpha));
    map_alignment_state = max(AlignmentType::kFirst, min(AlignmentType::kLast, map_alignment_state));
    map_labels_mode = max(LabelsMode::kFirst, min(LabelsMode::kLast, map_labels_mode));
    if (map_rect_top < 0 || map_rect_top > map_rect_bottom || map_rect_bottom > 1) {
        map_rect_top = kDefaultRectTop;
        map_rect_bottom = kDefaultRectBottom;
    }
    if (map_rect_left < 0 || map_rect_left > map_rect_right || map_rect_right > 1) {
        map_rect_left = kDefaultRectLeft;
        map_rect_right = kDefaultRectRight;
    }
    position_size = max(0.01f, min(kMaxPositionSize, position_size));
    marker_size = max(0.01f, min(kMaxMarkerSize, marker_size));
}

void ZoneMap::save_ini()
{
    if (!ZealService::get_instance())
        return;
    std::shared_ptr<IO_ini> ini = ZealService::get_instance()->ini;
    if (!ini)
        return;

    ini->setValue<bool>("Zeal", "MapEnabled", enabled);
    ini->setValue<bool>("Zeal", "MapExternalEnabled", external_enabled);
    ini->setValue<int>("Zeal", "MapExternalLeftOffset", external_monitor_left_offset);
    ini->setValue<int>("Zeal", "MapExternalTopOffset", external_monitor_top_offset);
    ini->setValue<bool>("Zeal", "MapShowGroup", map_show_group);
    ini->setValue<bool>("Zeal", "MapShowRaid", map_show_raid);
    ini->setValue<int>("Zeal", "MapDataMode", static_cast<int>(map_data_mode));
    ini->setValue<int>("Zeal", "MapBackgroundState", static_cast<int>(map_background_state));
    ini->setValue<float>("Zeal", "MapBackgroundAlpha", map_background_alpha);
    ini->setValue<int>("Zeal", "MapAlignment", static_cast<int>(map_alignment_state));
    ini->setValue<int>("Zeal", "MapLabelsMode", static_cast<int>(map_labels_mode));
    ini->setValue<int>("Zeal", "MapGroupLabelsMode", static_cast<int>(map_group_labels_mode));
    ini->setValue<float>("Zeal", "MapRectTop", map_rect_top);
    ini->setValue<float>("Zeal", "MapRectLeft", map_rect_left);
    ini->setValue<float>("Zeal", "MapRectBottom", map_rect_bottom);
    ini->setValue<float>("Zeal", "MapRectRight", map_rect_right);
    ini->setValue<float>("Zeal", "MapPositionSize", position_size);
    ini->setValue<float>("Zeal", "MapMarkerSize", marker_size); 
    ini->setValue<std::string>("Zeal", "MapFontFilename",
                        font_filename.empty() ? std::string("default") : font_filename);
}

void ZoneMap::update_ui_options() {
    if (ZealService::get_instance() && ZealService::get_instance()->ui
                    && ZealService::get_instance()->ui->options)
       ZealService::get_instance()->ui->options->UpdateOptionsMap();
}

bool ZoneMap::set_map_top(int top_percent, bool update_default, bool preserve_height) {
    float new_top = top_percent / 100.f;
    float bottom = preserve_height ? (new_top + (map_rect_bottom - map_rect_top)) : map_rect_bottom;
    return set_map_rect(new_top, map_rect_left, bottom, map_rect_right, update_default);
}

bool ZoneMap::set_map_left(int left_percent, bool update_default, bool preserve_width) {
    float new_left = left_percent / 100.f;
    float right = preserve_width ? (new_left + (map_rect_right - map_rect_left)) : map_rect_right;
    return set_map_rect(map_rect_top, new_left, map_rect_bottom, right, update_default);
}

bool ZoneMap::set_map_bottom(int bottom_percent, bool update_default) {
    return set_map_rect(map_rect_top, map_rect_left, bottom_percent / 100.f, map_rect_right, update_default);
}

bool ZoneMap::set_map_height(int height_percent, bool update_default) {
    return set_map_rect(map_rect_top, map_rect_left, map_rect_top + height_percent / 100.f, map_rect_right, update_default);
}

bool ZoneMap::set_map_right(int right_percent, bool update_default) {
    return set_map_rect(map_rect_top, map_rect_left, map_rect_bottom, right_percent / 100.f, update_default);
}

bool ZoneMap::set_map_width(int width_percent, bool update_default) {
    return set_map_rect(map_rect_top, map_rect_left, map_rect_bottom, map_rect_left + width_percent / 100.f, update_default);
}

bool ZoneMap::set_map_rect(float top, float left, float bottom, float right, bool update_default) {
    const float kMinimumDimension = 0.1f;
    top = max(0.f, min(1.f, top));
    left = max(0.f, min(1.f, left));
    bottom = max(0.f, min(1.f, bottom));
    right = max(0.f, min(1.f, right));
    if ((top > bottom - kMinimumDimension) || (left > right - kMinimumDimension)) {
        update_ui_options();
        return false;  // Reject if not at least minimum size.
    }

    map_rect_top = top;
    map_rect_left = left;
    map_rect_bottom = bottom;
    map_rect_right = right;

    render_release_resources();  // Invalidate buffers.
    zone_id = kInvalidZoneId;  // Triggers reload.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini) {
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectTop", top);
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectLeft", left);
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectBottom", bottom);
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectRight", right);
    }
    update_ui_options();

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

    if ((tlbr.size() != 4) || !set_map_rect(tlbr[0] / 100.f, tlbr[1] / 100.f, tlbr[2] / 100.f, tlbr[3] / 100.f, false))
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
        (tlhw[1] + tlhw[3]) / 100.f, false))   // And width to right (left + width).
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
    if (args.size() == 2) {
        set_show_group(!is_show_group_enabled(), false);
        Zeal::EqGame::print_chat("Showing group members is %s", is_show_group_enabled() ? "ON" : "OFF");
    }
    else if (args.size() == 3 && args[2] == "labels_off") {
        set_group_labels_mode(GroupLabelsMode::kOff, false);
        Zeal::EqGame::print_chat("Showing group member labels is off");
    }
    else if (args.size() == 3 && args[2] == "numbers") {
        set_group_labels_mode(GroupLabelsMode::kNumbers, false);
        Zeal::EqGame::print_chat("Showing group member labels as numbers");
    }
    else if (args.size() == 3 && args[2] == "names") {
        set_group_labels_mode(GroupLabelsMode::kNames, false);
        Zeal::EqGame::print_chat("Showing group member labels as names");
    }
    else {
        Zeal::EqGame::print_chat("Usage: /map show_group (toggles on/off), /map show_group [labels_off, numbers, names]");
    }
}

void ZoneMap::parse_show_raid(const std::vector<std::string>& args) {
    set_show_raid(!is_show_raid_enabled(), false);
    Zeal::EqGame::print_chat("Showing raid members is %s", is_show_raid_enabled() ? "ON" : "OFF");
}

void ZoneMap::parse_marker(const std::vector<std::string>& args) {
    if (args.size() <= 2) {
        clear_marker();
        return;
    }

    int y = 0;
    int x = 0;
    if (args.size() >= 4 && args.size() < 6 && Zeal::String::tryParse(args[2], &y) && Zeal::String::tryParse(args[3], &x)) {
        if (args.size() == 5) {
            int new_size = 0;
            if (!Zeal::String::tryParse(args[4], &new_size) || new_size <= 0) {
                clear_marker();
                return;
            }
            set_marker_size(new_size, false);
        }
        set_marker(y, x);
        set_enabled(true);
        return;
    }

    Zeal::EqGame::print_chat("Usage: /map marker <y> <x> [size] (percent size: optional, 0 disables)");
    Zeal::EqGame::print_chat("Example: /map marker 500 -1000 (drops a marker at loc 500, -1000)");
}

bool ZoneMap::search_poi(const std::string& search_term) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return false;  // Not handled here.

    const ZoneMapData* zone_map_data = get_zone_map(self->ZoneId);
    if (!zone_map_data || zone_map_data->num_labels == 0) {
        Zeal::EqGame::print_chat("No map POI data available for this zone");
        return false;
    }

    // Go through the labels looking for a match. Use lowercase for both.
    std::string search_lower = search_term;
    std::transform(search_lower.begin(), search_lower.end(), search_lower.begin(), ::tolower);
    int match_count = 0;
    for (int i = 0; i < zone_map_data->num_labels; ++i) {
        const ZoneMapLabel& label = zone_map_data->labels[i];
        std::string label_lower = std::string(label.label);
        std::transform(label_lower.begin(), label_lower.end(), label_lower.begin(), ::tolower);
        if (label_lower.find(search_lower) != std::string::npos) {
            match_count++;
            const char* flag = "-";
            if (match_count == 1) {
                Zeal::EqGame::print_chat("Map poi search results for: %s:", search_term.c_str());
                flag = "+";  // Flag the POI that was used for the marker.
                // Note: Need to negate y and x to go from map data to world coordinates.
                set_marker(-zone_map_data->labels[i].y, -zone_map_data->labels[i].x);
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
            clear_marker();
            return true;
        }
        if (args[1] != "help") {
            return search_poi(args[1]);
        }
    }
    else if (args.size() == 3 && Zeal::String::tryParse(args[1], &y) && Zeal::String::tryParse(args[2], &x)) {
        set_marker(y, x);
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

void ZoneMap::parse_font(const std::vector<std::string>& args) {
    if (args.size() == 3) {
        release_font();
        font_filename = std::string(args[2]);
        Zeal::EqGame::print_chat("Setting font to: %s", font_filename.c_str());
        return;
    }
    Zeal::EqGame::print_chat("Usage: /map font <name> (uifiles/zeal/fonts/<name>.spritefont)");
}

void ZoneMap::toggle_level_up() {
    set_level(map_level_index + 1);
}

void ZoneMap::toggle_level_down() {
    set_level(map_level_index - 1);
}

bool ZoneMap::set_level(int level_index) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return false;

    const ZoneMapData* zone_map_data = get_zone_map(self->ZoneId);
    if (zone_id == kInvalidZoneId || !zone_map_data || zone_map_data->num_levels < 2)
        return false;

    // Commit to making an update. Disable level filtering by default.
    int current_zone_id = zone_id;
    zone_id = kInvalidZoneId;  // Triggers refresh.
    map_level_zone_id = kInvalidZoneId;
    map_level_index = 0;

    // Support wraparound of toggles up and down.
    if (level_index == zone_map_data->num_levels) {
        level_index = 0;
    }
    else if (level_index == -1) {
        level_index = zone_map_data->num_levels - 1;
    }

    // Index [0] is expected to be the leftover kZoneMapInvalidLevelId lines. Return
    // success if that is correct.
    if (level_index == 0)
        return (zone_map_data->levels[level_index].level_id == kZoneMapInvalidLevelId);

    if (level_index < 0 || level_index >= zone_map_data->num_levels ||
        zone_map_data->levels[level_index].level_id == kZoneMapInvalidLevelId)
        return false;  // Ineligible index.

    map_level_zone_id = current_zone_id;
    map_level_index = level_index;
    return true;
}

void ZoneMap::parse_level(const std::vector<std::string>& args) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return;

    const ZoneMapData* zone_map_data = get_zone_map(self->ZoneId);
    if (!zone_map_data || zone_map_data->num_levels < 2) {
        Zeal::EqGame::print_chat("No map level data available for this zone");
        return;
    }

    int level_index = 0;
    if (args.size() == 2) {
        Zeal::EqGame::print_chat("Levels for zone: %s", zone_map_data->name);
        for (int i = 0; i < zone_map_data->num_levels; ++i) {
            const ZoneMapLevel& level = zone_map_data->levels[i];
            Zeal::EqGame::print_chat("[%i]: min_z: %i, max_z: %i, id: %i",
                i, level.min_z, level.max_z, level.level_id);
        }
    }
    else if (args.size() != 3 || !Zeal::String::tryParse(args[2], &level_index) || !set_level(level_index))
        Zeal::EqGame::print_chat("Usage: /map level <index> (set to 0 to show all levels)");
}


void ZoneMap::parse_poi(const std::vector<std::string>& args) {

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return;

    const ZoneMapData* zone_map_data = get_zone_map(self->ZoneId);
    if (!zone_map_data || zone_map_data->num_labels == 0) {
        Zeal::EqGame::print_chat("No map POI data available for this zone");
        return;
    }

    // Note: Need to negate y and x to go from map data to world coordinates.
    int poi = 0;
    if (args.size() == 2) {
        Zeal::EqGame::print_chat("Map POI list for zone: %s", zone_map_data->name);
        for (int i = 0; i < zone_map_data->num_labels; ++i) {
            const ZoneMapLabel& label = zone_map_data->labels[i];
            Zeal::EqGame::print_chat("[%i]: (%i, %i): %s", i, -label.y, -label.x, label.label);
        }
    }
    else if (args.size() == 3 && Zeal::String::tryParse(args[2], &poi, true)) {
        if (poi < 0 || poi >= zone_map_data->num_labels) {
            Zeal::EqGame::print_chat("Invalid selection index");
            return;
        }
        set_marker(-zone_map_data->labels[poi].y, -zone_map_data->labels[poi].x);
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
    Zeal::EqGame::print_chat("enabled: %i, background: %i (%.2f), align: %i, labels:%i, zone: %i",
                            enabled, map_background_state, map_background_alpha, map_alignment_state, map_labels_mode, zone_id);
    Zeal::EqGame::print_chat("marker: zone: %i, y: %i, x: %i, num_labels: %i",
                            marker_zone_id, marker_y, marker_x, labels_list.size());
    Zeal::EqGame::print_chat("view: t: %i, l: %i, h: %i, w: %i, Max H: %i, W: %i", 
        viewport.Y, viewport.X, viewport.Height, viewport.Width, max_viewport_height, max_viewport_width);
    Zeal::EqGame::print_chat("rect: t: %f, l: %f, b: %f, r: %f", map_rect_top, map_rect_left, map_rect_bottom, map_rect_right);
    Zeal::EqGame::print_chat("clip: t: %f, l: %f, b: %f, r: %f", clip_min_y, clip_min_x, clip_max_y, clip_max_x);
    Zeal::EqGame::print_chat("level: zone: %i, index: %i, z_max: %i, z_min: %i", map_level_zone_id, map_level_index, clip_max_z, clip_min_z);
    Zeal::EqGame::print_chat("position_size: %f, marker_size: %f, show_group: %i, %i, show_raid: %i",
        position_size, marker_size, map_show_group, static_cast<int>(map_group_labels_mode), map_show_raid);
    Zeal::EqGame::print_chat("scale_factor: %f, offset_y: %f, offset_x: %f, zoom: %f",
        mat_model2world(0,0), mat_model2world(3,1), mat_model2world(3, 0), zoom_factor);
    Zeal::EqGame::print_chat("dyn_labels_size: %i, dyn_labels_zone_id: %i, data_mode: %i, data_cache: %i", 
        dynamic_labels_list.size(), dynamic_labels_zone_id, map_data_mode, map_data_cache.size());
    Zeal::EqGame::print_chat("line_count: %i, line: %i, position: %i, marker: %i, font: %i (%s)", line_count,
        line_vertex_buffer != nullptr, position_vertex_buffer != nullptr, marker_vertex_buffer != nullptr,
        bitmap_font != nullptr, font_filename.c_str());
    Zeal::EqGame::print_chat("external_monitor_left_offset: %i, monitor_left_offset: %i, monitor_top_offset: %i",
        external_enabled, external_monitor_left_offset, external_monitor_top_offset);
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
    else if (args[1] == "save_ini") {
        Zeal::EqGame::print_chat("Saving current map settings");
        save_ini();
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
        Zeal::EqGame::print_chat("Usage: /map [on|off|size|alignment|marker|background|zoom|poi|labels|level|show_group|save_ini]");
        Zeal::EqGame::print_chat("Beta: /map [data_mode|always_center]");
        Zeal::EqGame::print_chat("Shortcuts: /map <y> <x>, /map 0, /map <poi_search_term>");
        Zeal::EqGame::print_chat("Examples: /map 100 -200 (drops a marker at loc 100, -200), /map 0 (clears marker)");
    }
    return true;
}

void ZoneMap::callback_zone() {
    zone_id = kInvalidZoneId;
    marker_zone_id = kInvalidZoneId;
    dynamic_labels_zone_id = kInvalidZoneId;
    map_level_zone_id = kInvalidZoneId;
    map_level_index = 0;
    zoom_factor = 1.f;  // Reset for more consistent behavior.
}

void ZoneMap::release_font() {
    if (bitmap_font) {
        bitmap_font->release();  // Paranoid redundancy for destructor below.
        bitmap_font.reset();
    }
}

void ZoneMap::callback_dx_reset() {
    render_release_resources();
    release_font();
    release_d3d_external_window();
    zone_id = kInvalidZoneId;  // Triggers reload of map.
}

ZoneMap::ZoneMap(ZealService* zeal, IO_ini* ini)
{
    load_ini(ini);
    zeal->callbacks->AddGeneric([this]() { callback_render(); }, callback_type::RenderUI);
    zeal->callbacks->AddGeneric([this]() { callback_dx_reset(); }, callback_type::DXReset);
    zeal->callbacks->AddGeneric([this]() { callback_zone(); }, callback_type::Zone);
    zeal->commands_hook->Add("/map", {}, "Controls map overlay",
        [this](const std::vector<std::string>& args) {
            return parse_command(args);
        });
}

ZoneMap::~ZoneMap()
{
    render_release_resources();
    release_font();
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

// Window processing logic.
LRESULT CALLBACK ZoneMap::external_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_DPICHANGED:
            return 0;  // Ignore any resizing due to a monitor scale change.
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


void ZoneMap::process_external_window_messages() {

    if (!external_hwnd)
        return;

    MSG msg = {};
    if (PeekMessage(&msg, external_hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT) {
        set_enabled(false, false);  // Disables, releases resources.
        external_hwnd = nullptr;
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
    wcex.style = CS_HREDRAW | CS_VREDRAW;  // Not sure if these are required.
    wcex.lpfnWndProc = static_external_window_proc;
    wcex.hInstance = external_hinstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = kExternalWindowClassName;
    if (RegisterClassEx(&wcex) == 0) {   // Returns 0 if fails.
        Zeal::EqGame::print_chat("Error registering external map window class");
        return false;
    }

    // Set the created window size so that the client size matches our render target.
    // Note: WS_OVERLAPPED does not work with adjust window, so use WS_CAPTION.
    RECT win_rect = calc_external_window_client_rect();
    if (!AdjustWindowRectEx(&win_rect, WS_CAPTION, false, NULL))
        Zeal::EqGame::print_chat("Error adjusting window rect");

    external_hwnd = CreateWindowEx(NULL,
        kExternalWindowClassName,
        kExernalWindowTitle,
        WS_OVERLAPPED,
        win_rect.left, win_rect.top,  // x, y offset.
        win_rect.right - win_rect.left,  // adjusted for client width
        win_rect.bottom - win_rect.top,  // adjust for client height
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
    if (FAILED(external_d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        external_hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &external_d3ddev))) {
        Zeal::EqGame::print_chat("Error creating external map device");
        release_d3d_external_window();
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

