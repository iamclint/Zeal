#include "zone_map.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include "zone_map_data.h"

// Possible enhancements:
// - Support a command for default small / med / large map sizes
// - Support 'windowed mode' (drag and drop, resizing)

static constexpr int kBackgroundCount = 2;  // Two triangles in split mode.
static constexpr int kBackgroundVertices = 3 + (kBackgroundCount - 1);  // Two triangles using split.
static constexpr int kMarkerCount = 4;  // Four triangles.
static constexpr int kPositionCount = 2;  // Two triangles.
static constexpr int kPositionVertices = kPositionCount * 3; // Fixed triangle list.

static constexpr DWORD kMapVertexFvfCode = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

struct MapVertex {
    float x, y, z, rhw;  // Position coordinates and rhw (D3DFVF_XYZRHW).
    D3DCOLOR color; // Color from the D3DFVF_DIFFUSE flag.
};

struct TwoPoints {
    float x0, y0, x1, y1;  // Either line segment end or rect TL BR.
};



// Implements Cohen-Sutherland clipping (with +y towards bottom).
// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

typedef int OutCode;

static constexpr int INSIDE = 0b0000;
static constexpr int LEFT = 0b0001;
static constexpr int RIGHT = 0b0010;
static constexpr int TOP = 0b0100;
static constexpr int BOTTOM = 0b1000;

// Returns the bit code for the point (x, y) relative to the clip rectangle.
OutCode compute_outcode(const TwoPoints& clip_rect, double x, double y)
{
    OutCode code = INSIDE;  // initialised as being inside of clip window

    if (x < clip_rect.x0)           // to the left of clip window
        code |= LEFT;
    else if (x > clip_rect.x1)      // to the right of clip window
        code |= RIGHT;
    if (y < clip_rect.y0)           // above the clip window
        code |= TOP;
    else if (y > clip_rect.y1)      // below the clip window
        code |= BOTTOM;

    return code;
}

// Returns true if the line parameter is visible (after clipping).
bool clip_line_segment(const TwoPoints& clip_rect, TwoPoints& line) {
    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    OutCode outcode0 = compute_outcode(clip_rect, line.x0, line.y0);
    OutCode outcode1 = compute_outcode(clip_rect, line.x1, line.y1);
    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) {
            return true;  // Bitwise OR is 0: Both points inside window. Done.
        }
        else if (outcode0 & outcode1) {
            return false;  // Points share an outside zone, so both outside. Done.
        }
        else {
            // Calculate the line segment to clip outside point to edge intersection.
            float x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

            // Now find the intersection point;
            // use formulas:
            //   slope = (y1 - y0) / (x1 - x0)
            //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
            // No need to worry about divide-by-zero because, in each case, the
            // outcode bit being tested guarantees the denominator is non-zero
            if (outcodeOut & BOTTOM) {           // point is below the clip window
                x = line.x0 + (line.x1 - line.x0) * (clip_rect.y1 - line.y0) / (line.y1 - line.y0);
                y = clip_rect.y1;
            }
            else if (outcodeOut & TOP) { // point is above the clip window
                x = line.x0 + (line.x1 - line.x0) * (clip_rect.y0 - line.y0) / (line.y1 - line.y0);
                y = clip_rect.y0;
            }
            else if (outcodeOut & RIGHT) {  // point is to the right of clip window
                y = line.y0 + (line.y1 - line.y0) * (clip_rect.x1 - line.x0) / (line.x1 - line.x0);
                x = clip_rect.x1;
            }
            else if (outcodeOut & LEFT) {   // point is to the left of clip window
                y = line.y0 + (line.y1 - line.y0) * (clip_rect.x0 - line.x0) / (line.x1 - line.x0);
                x = clip_rect.x0;
            }

            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0) {
                line.x0 = x;
                line.y0 = y;
                outcode0 = compute_outcode(clip_rect, line.x0, line.y0);
            }
            else {
                line.x1 = x;
                line.y1 = y;
                outcode1 = compute_outcode(clip_rect, line.x1, line.y1);
            }
        }
    }
}


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

void ZoneMap::render_load_map() {
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    if (!device)
        return;

    render_release_resources();  // Forces update of all graphics.

    const ZoneMapData* zone_map_data = get_zone_map_data(zone_id);
    if (!zone_map_data) {
        return;
    }

    // Notes on scaling the map_data world coordinates to map screen window coordinates.
    //   The map data coordinate system is in the game world scale but negated y and x values.
    //     map_data: +y = south, +x = east.  game location: +y = north, +x = east.
    //   The map_data polarity is consistent with the screen coordinate system for North = up.
    //   window_x[i] = (map_x[i] - min(map_x)) * window_size_x/map_size_x + window_x_offset
    //   window_x[i] = map_x[i] * scale_factor + offset where
    //     scale_factor = window_size_x/map_size_x and offset = window_x_offset - min(map_x)*scale_factor.
    const Vec2 screen_size = ZealService::get_instance()->dx->GetScreenRect();
    const float rect_left = map_rect_left * screen_size.x;
    float rect_right = map_rect_right * screen_size.x;
    const float rect_top = map_rect_top * screen_size.y;
    float rect_bottom = map_rect_bottom * screen_size.y;

    float window_size_x = rect_right - rect_left;
    float window_size_y = rect_bottom - rect_top;
    float scale_factor_x = window_size_x / (zone_map_data->max_x - zone_map_data->min_x);
    float scale_factor_y = window_size_y / (zone_map_data->max_y - zone_map_data->min_y);
    scale_factor = min(scale_factor_x, scale_factor_y);  // Preserve aspect ratio.
    offset_x = rect_left - zone_map_data->min_x * scale_factor;
    offset_y = rect_top - zone_map_data->min_y * scale_factor;

    // In zoom, the background is the full window and just adjust scale and offsets.
    if (zoom_factor > 1.f && Zeal::EqGame::get_self()) {
        scale_factor *= zoom_factor;  // Scale is easy.
        // For the offsets, map position in world coordinates to center of screen rect.
        Vec3 position = Zeal::EqGame::get_self()->Position;
        float position_y = -position.x;  // Note x and y are polarity flipped.
        float position_x = -position.y;  // And position is y, x, z.
        float rect_center_x = (rect_right + rect_left) * 0.5;
        float rect_center_y = (rect_bottom + rect_top) * 0.5;
        offset_x = rect_center_x - position_x * scale_factor;
        offset_y = rect_center_y - position_y * scale_factor;
    }
    else {
        // Clip right and bottom clipping edges using common scale factor (for background).
        rect_right = rect_left + (zone_map_data->max_x - zone_map_data->min_x) * scale_factor;
        rect_bottom = rect_top + (zone_map_data->max_y - zone_map_data->min_y) * scale_factor;
    }

    const int kMaxLineCount = zone_map_data->num_lines;  // Allocate a buffer assuming all visible.
    std::unique_ptr<MapVertex[]> line_vertices = std::make_unique<MapVertex[]>(kMaxLineCount * 2);

    const TwoPoints clip_rect = { .x0 = rect_left, .y0 = rect_top, .x1 = rect_right, .y1 = rect_bottom };
    line_count = 0;  // Tracks number of visible (after clipping) lines.
    for (int i = 0; i < kMaxLineCount; ++i) {
        TwoPoints line = { zone_map_data->lines[i].x0 * scale_factor + offset_x,
                            zone_map_data->lines[i].y0 * scale_factor + offset_y,
                            zone_map_data->lines[i].x1 * scale_factor + offset_x,
                            zone_map_data->lines[i].y1 * scale_factor + offset_y
        };
        if (!clip_line_segment(clip_rect, line))
            continue;

        auto color = D3DCOLOR_XRGB(zone_map_data->lines[i].red, zone_map_data->lines[i].green, zone_map_data->lines[i].blue);
        if (color == D3DCOLOR_XRGB(0, 0, 0)) {
            color = D3DCOLOR_XRGB(64, 64, 64);  // Increase visibility of black lines.
        }

        line_vertices[2 * line_count].x = line.x0;
        line_vertices[2 * line_count].y = line.y0;
        line_vertices[2 * line_count].z = 0.5f;
        line_vertices[2 * line_count].rhw = 1.f;
        line_vertices[2 * line_count].color = color;
        line_vertices[2 * line_count + 1].x = line.x1;
        line_vertices[2 * line_count + 1].y = line.y1;
        line_vertices[2 * line_count + 1].z = 0.5f;
        line_vertices[2 * line_count + 1].rhw = 1.f;
        line_vertices[2 * line_count + 1].color = color;
        line_count++;
    }

    // Add the background triangle vertices at the end of the buffer. 

    auto background_color = D3DCOLOR_XRGB(0, 0, 0);  // Black or clear.
    if (map_background_state == 2) {
        background_color = D3DCOLOR_XRGB(160, 160, 160);  // Light grey.
    }
    else if (map_background_state == 3) {
        background_color = D3DCOLOR_XRGB(240, 180, 140);  // Tan.
    }
    MapVertex background_vertices[kBackgroundVertices] = {
        { rect_left, rect_top, 0.5f, 1.f, background_color },  // x, y, z, rhw, color
        { rect_right, rect_top, 0.5f, 1.f, background_color },
        { rect_left, rect_bottom, 0.5f, 1.f, background_color },
        { rect_right, rect_bottom, 0.5f, 1.f, background_color },
    };

    const int line_buffer_size = sizeof(MapVertex) * line_count * 2;
    const int background_buffer_size = sizeof(MapVertex) * kBackgroundVertices;
    // Create a Vertex buffer and copy the map line segments over.
    if (FAILED(device->CreateVertexBuffer(line_buffer_size + background_buffer_size,
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
    int position_buffer_size = sizeof(MapVertex) * kPositionVertices;
    if (FAILED(device->CreateVertexBuffer(position_buffer_size,
        D3DUSAGE_WRITEONLY,
        kMapVertexFvfCode,
        D3DPOOL_MANAGED,
        &position_vertex_buffer))) {
        return;
    }

}

void ZoneMap::render_map()
{
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    if (!device || !line_vertex_buffer)
        return;
     
    device->SetTexture(0, NULL);  // Ensure no texture is bound
    device->SetVertexShader(kMapVertexFvfCode);

    if (map_background_state) {
        device->SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
        device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, kBackgroundCount);
    }

    if (line_count) {
        device->SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
        device->DrawPrimitive(D3DPT_LINELIST, kBackgroundVertices, line_count);
    }

    // Note: Markers are currently getting drawn under the map lines (good or bad?).
    if (position_vertex_buffer) {
        int triangle_count = render_update_position_buffer();
        device->SetStreamSource(0, position_vertex_buffer, sizeof(MapVertex));
        device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangle_count);
    }

    if (marker_vertex_buffer) {
        device->SetStreamSource(0, marker_vertex_buffer, sizeof(MapVertex));
        device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, kMarkerCount);
    }

}

int ZoneMap::render_update_position_buffer() {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!position_vertex_buffer || !self) {
        return 0;
    }

    // Translate loc (world) coordinates to screen coordinates. Note that the game
    // world coordinates are negated (polarity flipped) to match the map data.
    Vec3 position = self->Position;
    float screen_y = -position.x * scale_factor + offset_y;  // Note position is y,x,z.
    float screen_x = -position.y * scale_factor + offset_x;

    // Generate the vertices for two triangles that makeup the target.
    const Vec2 screen_size = ZealService::get_instance()->dx->GetScreenRect();
    float size = position_size * min(screen_size.x, screen_size.y);
    size = max(5.f, size);  // Constrain so it remains visible.

    // Heading: 0 = N = -y, 128 = W = -x, 256 = S = +y, 384 = E = +x.
    // So: Screen x tracks -sin(heading) and y tracks -cos(heading).
    float heading = self->Heading;
    float direction = static_cast<float>(heading * M_PI / 256); // In radians.
    const float rotation = static_cast<float>(120 * M_PI / 180);
    float cos_theta0 = cosf(direction);
    float sin_theta0 = sinf(direction);
    float cos_theta1 = cosf(direction - rotation);  // Rotated clockwise.
    float sin_theta1 = sinf(direction - rotation);
    float cos_theta2 = cosf(direction + rotation);  // Rotated CCW.
    float sin_theta2 = sinf(direction + rotation);

    Vec3 vertex0 = { -sin_theta0 * size, -cos_theta0 * size, 0 };
    Vec3 vertex1 = { -sin_theta1 * size, -cos_theta1 * size, 0 };
    Vec3 vertex2 = { -sin_theta2 * size, -cos_theta2 * size, 0 };
    Vec3 vertex3 = { -vertex0.x/8, -vertex0.y/8, 0 };

    Vec3 symbol[kPositionVertices] = { vertex0, vertex1, vertex3,
        vertex0, vertex3, vertex2};

    MapVertex position_vertices[kPositionVertices];
    const float rect_left = map_rect_left * screen_size.x;
    const float rect_right = map_rect_right * screen_size.x;
    const float rect_top = map_rect_top * screen_size.y;
    const float rect_bottom = map_rect_bottom * screen_size.y;
    for (int i = 0; i < kPositionVertices; ++i) {
        position_vertices[i].x = max(rect_left, min(rect_right, symbol[i].x + screen_x));
        position_vertices[i].y = max(rect_top, min(rect_bottom, symbol[i].y + screen_y));
        position_vertices[i].z = 0.5f;
        position_vertices[i].rhw = 1.f;
        position_vertices[i].color = D3DCOLOR_XRGB(250, 250, 51);  // Lemon yellow.
    }

    if (zoom_factor > 1.f && (rect_bottom - screen_y < size || screen_y - rect_top < size ||
        rect_right - screen_x < size || screen_x - rect_left < size)) {
        zone_id = kInvalidZoneId;  // Trigger an update (next render) to re-center the zoom region.
    }

    BYTE* data = nullptr;
    if (FAILED(position_vertex_buffer->Lock(0, 0, &data, D3DLOCK_DISCARD))) {
        position_vertex_buffer->Release();
        position_vertex_buffer = nullptr;
        return 0;
    }
    int position_buffer_size = sizeof(MapVertex) * kPositionVertices;
    memcpy(data, position_vertices, position_buffer_size);
    position_vertex_buffer->Unlock();

    return kPositionCount;
}

void ZoneMap::render_update_marker_buffer() {
    if (marker_vertex_buffer) {  // Release any resources (should be redundant).
        marker_vertex_buffer->Release();
        marker_vertex_buffer = nullptr;
    }

    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    if (!device || !line_vertex_buffer)
        return;

    // Translate loc (world) coordinates to screen coordinates. Note that the game
    // world coordinates are negated (polarity flipped) to match the map data.
    float screen_y = -marker_y * scale_factor + offset_y;
    float screen_x = -marker_x * scale_factor + offset_x;

    // Generate the vertices for four triangles that makeup the target.
    // Calculate the individual triangle 'size' in screen resolution.
    const Vec2 screen_size = ZealService::get_instance()->dx->GetScreenRect();
    float size = max(5.f, marker_size * min(screen_size.x, screen_size.y) * 0.5f);
    float short_size = size * 0.25f;
    const int kNumVertices = kMarkerCount * 3;  // Separate triangles with 3 vertices (CW direction).
    Vec3 marker[kNumVertices] = {{0, 0, 0}, {short_size, size, 0}, {-short_size, size, 0},
                                    {0, 0, 0}, {-short_size, -size, 0}, {short_size, -size, 0},
                                    {0, 0, 0}, {size, -short_size, 0}, {size, short_size, 0},
                                    {0, 0, 0}, {-size, short_size, 0}, {-size, -short_size, 0}
    };
    MapVertex marker_vertices[kNumVertices];
    const float rect_left = map_rect_left * screen_size.x;
    const float rect_right = map_rect_right * screen_size.x;
    const float rect_top = map_rect_top * screen_size.y;
    const float rect_bottom = map_rect_bottom * screen_size.y;
    for (int i = 0; i < kNumVertices; ++i) {
        marker_vertices[i].x = max(rect_left, min(rect_right, marker[i].x + screen_x));
        marker_vertices[i].y = max(rect_top, min(rect_bottom, marker[i].y + screen_y));
        marker_vertices[i].z = 0.5f;
        marker_vertices[i].rhw = 1.f;
        marker_vertices[i].color = D3DCOLOR_XRGB(255, 0, 0);
    }

    // Create a Vertex buffer and copy the triangle vertices.
    int marker_buffer_size = sizeof(MapVertex) * kNumVertices;
    if (FAILED(device->CreateVertexBuffer(marker_buffer_size,
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

void ZoneMap::callback_render()
{
    if (!enabled || !Zeal::EqGame::is_in_game() || !Zeal::EqGame::get_self())
        return;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (zone_id != self->ZoneId) {
        zone_id = self->ZoneId;
        render_load_map();
    }

    // Add the marker if it is empty and this is the right zone.
    if (marker_zone_id == zone_id && marker_vertex_buffer == nullptr) {
        render_update_marker_buffer();
    }

    render_map();
}

void ZoneMap::set_enabled(bool _enabled, bool update_default)
{
    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapEnabled", _enabled);

    enabled = _enabled;
    if (!enabled) {
        render_release_resources();
        zone_id = kInvalidZoneId;  // Triggers update when re-enabled.
    }
    ZealService::get_instance()->ui->options->UpdateOptions();
}

void ZoneMap::toggle_background() {
    if (++map_background_state >= kBackgroundStates) {
        map_background_state = 0;
    }
    zone_id = kInvalidZoneId;  // Triggers reload.
}

int ZoneMap::get_background()
{
    return map_background_state;
}

bool ZoneMap::set_background(int new_state, bool update_default) {
    if ((new_state < 0) || (new_state >= kBackgroundStates)) {
        return false;
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapBackgroundState", new_state);

    map_background_state = new_state;
    zone_id = kInvalidZoneId;  // Triggers reload.
    ZealService::get_instance()->ui->options->UpdateOptions();
    return true;
}

void ZoneMap::set_position_default_size(float new_size) {
    position_size = max(0.01f, min(0.1f, new_size));
    if (ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapPositionSize", marker_size);
}

void ZoneMap::set_marker_default_size(float new_size) {
    marker_size = max(0.01f, min(0.1f, new_size));
    if (ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapMarkerSize", marker_size);
}

void ZoneMap::clear_marker() {
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
    marker_x = x;
    marker_y = y;
}

void ZoneMap::toggle_zoom() {
    const int kNumDefaultZoomFactors = 4;
    const float kDefaultZoomFactors[kNumDefaultZoomFactors] = {2.f, 4.f, 8.f };

    // Assumes zoom factors are monotonic and look for the first zoom higher
    // than the current state.  Wraps back to 100% if beyond max zoom.
    float new_zoom_factor = 1.f;
    for (int i = 0; i < kNumDefaultZoomFactors; ++i) {
        if (zoom_factor < kDefaultZoomFactors[i]) {
            new_zoom_factor = kDefaultZoomFactors[i];
            break;
        }
    }
    zone_id = kInvalidZoneId;  // Triggers reload (including marker).
    zoom_factor = new_zoom_factor;
}
int ZoneMap::get_zoom() {
    int val = zoom_factor * 100;
    return val;
}

bool ZoneMap::set_zoom(int zoom_percent) {
    zone_id = kInvalidZoneId;  // Triggers reload (including marker).
    zoom_factor = zoom_percent * 0.01f;
    zoom_factor = min(100.f, max(1.f, zoom_factor));
    ZealService::get_instance()->ui->options->UpdateOptions();
    return true;
}

void ZoneMap::load_ini(IO_ini* ini)
{
    if (!ini->exists("Zeal", "MapEnabled"))
        ini->setValue<bool>("Zeal", "MapEnabled", false);
    if (!ini->exists("Zeal", "MapBackgroundState"))
        ini->setValue<int>("Zeal", "MapBackgroundState", 0);
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

    // TODO: Protect against corrupted ini file (like a boolean instead of float).
    enabled = ini->getValue<bool>("Zeal", "MapEnabled");
    map_background_state = ini->getValue<int>("Zeal", "MapBackgroundState");
    map_rect_top = ini->getValue<float>("Zeal", "MapRectTop");
    map_rect_left = ini->getValue<float>("Zeal", "MapRectLeft");
    map_rect_bottom = ini->getValue<float>("Zeal", "MapRectBottom");
    map_rect_right = ini->getValue<float>("Zeal", "MapRectRight");
    position_size = ini->getValue<float>("Zeal", "MapPositionSize");
    marker_size = ini->getValue<float>("Zeal", "MapMarkerSize");

    // Sanity clamp ini values.
    map_background_state = max(0, min(kBackgroundStates - 1, map_background_state));
    if (map_rect_top < 0 || map_rect_top > map_rect_bottom || map_rect_bottom > 1) {
        map_rect_top = kDefaultRectTop;
        map_rect_bottom = kDefaultRectBottom;
    }
    if (map_rect_left < 0 || map_rect_left > map_rect_right || map_rect_right > 1) {
        map_rect_left = kDefaultRectLeft;
        map_rect_right = kDefaultRectRight;
    }
    position_size = max(0.01f, min(0.10f, position_size));
    marker_size = max(0.01f, min(0.10f, marker_size));
}

void ZoneMap::set_map_width(float width) {
    map_rect_right = width;
    if (ZealService::get_instance() && ZealService::get_instance()->ini) {
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectRight", map_rect_right);
    }
    render_release_resources();  // Invalidate buffers.
    zone_id = kInvalidZoneId;  // Triggers reload.
}
void ZoneMap::set_map_height(float height) {
    map_rect_bottom = height;
    if (ZealService::get_instance() && ZealService::get_instance()->ini) {
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectBottom", map_rect_bottom);
    }
    render_release_resources();  // Invalidate buffers.
    zone_id = kInvalidZoneId;  // Triggers reload.
}
int ZoneMap::get_map_width()
{
    return (int)map_rect_right*100;
}
int ZoneMap::get_map_height()
{
    return (int)map_rect_bottom*100;
}

bool ZoneMap::set_map_rect(float top, float left, float bottom, float right, bool update_default) {
    const float kMinimumDimension = 0.1f;
    top = max(0.f, min(1.f, top));
    left = max(0.f, min(1.f, left));
    bottom = max(0.f, min(1.f, bottom));
    right = max(0.f, min(1.f, right));
    if ((top > bottom - kMinimumDimension) || (left > right - kMinimumDimension)) {
        return false;  // Reject if not at least minimum size.
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini) {
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectTop", top);
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectLeft", left);
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectBottom", bottom);
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapRectRight", right);
    }

    map_rect_top = top;
    map_rect_left = left;
    map_rect_bottom = bottom;
    map_rect_right = right;

    render_release_resources();  // Invalidate buffers.
    zone_id = kInvalidZoneId;  // Triggers reload.
    return true;
}

void ZoneMap::parse_rect(const std::vector<std::string>& args) {
    std::vector<float> tlbr;
    for (int i = 2; i < args.size(); i++)
    {
        float value = 0;
        if (Zeal::String::tryParse(args[i], &value))
            tlbr.push_back(value);
        else
            break;
    }

    if ((tlbr.size() != 4) || !set_map_rect(tlbr[0], tlbr[1], tlbr[2], tlbr[3]))
    {
        Zeal::EqGame::print_chat("Usage: /map rect <top> <left> <bottom> <right> (fraction of screen dimensions)");
        Zeal::EqGame::print_chat("Example: /map rect 0.1 0.2 0.5 0.6");
    }
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
            float new_size = 0;
            if (!Zeal::String::tryParse(args[4], &new_size) || new_size <= 0) {
                clear_marker();
                return;
            }
            set_marker_default_size(new_size);
        }
        set_marker(y, x);
        set_enabled(true);
        return;
    }

    Zeal::EqGame::print_chat("Usage: /map marker <y> <x> [size] (fractional size: optional, 0 disables)");
    Zeal::EqGame::print_chat("Example: /map marker 500 -1000 (drops a marker at loc 500, -1000)");
}

bool ZoneMap::parse_shortcuts(const std::vector<std::string>& args) {
    // Support implicit shortcuts (/map 0 = clear marker, /map y x = set marker).
    int y = 0;
    int x = 0;
    if (args.size() == 2 && args[1] == "0") {
        clear_marker();
        return true;
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
    if (args.size() != 3 || !Zeal::String::tryParse(args[2], &state) || !set_background(state))
        Zeal::EqGame::print_chat( "Usage: /map background <select> (0 to %i)", kBackgroundStates);
}

void ZoneMap::parse_zoom(const std::vector<std::string>& args) {
    int zoom_percent = 0;
    if (args.size() != 3 || !Zeal::String::tryParse(args[2], &zoom_percent) || !set_zoom(zoom_percent))
        Zeal::EqGame::print_chat( "Usage: /map zoom <percent> (<= 100 disables)");
}

void ZoneMap::parse_poi(const std::vector<std::string>& args) {

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return;

    const ZoneMapData* zone_map_data = get_zone_map_data(self->ZoneId);
    if (!zone_map_data || zone_map_data->num_labels == 0) {
        Zeal::EqGame::print_chat("No map POI data available for this zone");
        return;
    }

    // Note: Need to negate y and x to go from map data to world coordinates.
    int poi = 0;
    if (args.size() == 2) {
        for (int i = 0; i < zone_map_data->num_labels; ++i) {
            const ZoneMapLabel& label = zone_map_data->labels[i];
            Zeal::EqGame::print_chat("[%i]: (%i, %i): %s", i, -label.y, -label.x, label.label);
        }
    }
    else if (args.size() == 3 && Zeal::String::tryParse(args[2], &poi)) {
        if (poi < 0 || poi >= zone_map_data->num_labels) {
            Zeal::EqGame::print_chat("Invalid selection index");
            return;
        }
        set_marker(-zone_map_data->labels[poi].y, -zone_map_data->labels[poi].x);
        set_enabled(true);
    }
    else {
        Zeal::EqGame::print_chat("Usage: /map poi <index> (displays list if <index> is blank)");
        Zeal::EqGame::print_chat("Example: /map poi 2 (drops a marker at list item [2])");
    }
}


void ZoneMap::dump() {
    Zeal::EqGame::print_chat("enabled: %i, background: %i, zone: %i", enabled, map_background_state, zone_id);
    Zeal::EqGame::print_chat("marker: zone: %i, y: %i, x: %i", marker_zone_id, marker_y, marker_x);
    Zeal::EqGame::print_chat("rect: t: %f, l: %f, b: %f, r: %f", map_rect_top, map_rect_left, map_rect_bottom, map_rect_right);
    Zeal::EqGame::print_chat("position_size: %f, marker_size: %f", position_size, marker_size);
    Zeal::EqGame::print_chat("scale_factor: %f, offset_y: %f, offset_x: %f, zoom: %f", scale_factor, offset_y, offset_x, zoom_factor);
    Zeal::EqGame::print_chat("line_count: %i, line: %i, position: %i, marker: %i", line_count,
        line_vertex_buffer != nullptr, position_vertex_buffer != nullptr, marker_vertex_buffer != nullptr);
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
    else if (args[1] == "rect") {
        parse_rect(args);  // Set the Top, Left, Bottom, Right coordinates of window.
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
    else if (args[1] == "dump") {
        dump();
    }
    else if (!parse_shortcuts(args)) {
        Zeal::EqGame::print_chat("Usage: /map [on|off|rect|marker|background|zoom|poi], /map <y> <x>, /map 0");
        Zeal::EqGame::print_chat("Examples: /map 100 -200 (drops a marker at loc 100, -200), /map 0 (clears marker)");
    }
    return true;
}

ZoneMap::ZoneMap(ZealService* zeal, IO_ini* ini)
{
    load_ini(ini);
    zeal->callbacks->AddGeneric([this]() { callback_render(); }, callback_type::RenderUI);
    zeal->commands_hook->Add("/map", {}, "Controls map overlay",
        [this](const std::vector<std::string>& args) {
            return parse_command(args);
        });
}

ZoneMap::~ZoneMap()
{
    render_release_resources();
}