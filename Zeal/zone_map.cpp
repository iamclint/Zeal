#include "zone_map.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include "zone_map_data.h"
#include <string>

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

// Convenience class for stashing the active D3D render state before modifying
// a list of parameters.
class RenderStateStash {
public:
    struct Pair {
        D3DRENDERSTATETYPE state_type;
        DWORD value;
    };

    RenderStateStash(IDirect3DDevice8* device_) {
        device = device_;
    }

    // Processes list of pairs stashing current state and modifying to new state.
    void store_and_modify(const std::vector<Pair>& pairs) {
        for (const auto& pair : pairs) {
            DWORD value;
            device->GetRenderState(pair.state_type, &value);
            stored_pairs.push_back({ pair.state_type, value });
            device->SetRenderState(pair.state_type, pair.value);
        }
    }

    // Copies the stashed state back.
    void restore_state() {
        for (const auto& pair : stored_pairs)
            device->SetRenderState(pair.state_type, pair.value);
    }

private:
    IDirect3DDevice8* device;
    std::vector<Pair> stored_pairs;
};

class TextureStateStash {
public:
    struct Pair {
        D3DTEXTURESTAGESTATETYPE state_type;
        DWORD value;
    };

    TextureStateStash(IDirect3DDevice8* device_) {
        device = device_;
    }

    // Processes list of pairs stashing current state and modifying to new state.
    void store_and_modify(const std::vector<Pair>& pairs) {
        for (const auto& pair : pairs) {
            DWORD value;
            device->GetTextureStageState(0, pair.state_type, &value);
            stored_pairs.push_back({ pair.state_type, value });
            device->SetTextureStageState(0, pair.state_type, pair.value);
        }
    }

    // Copies the stashed state back.
    void restore_state() {
        for (const auto& pair : stored_pairs)
            device->SetTextureStageState(0, pair.state_type, pair.value);
    }

private:
    IDirect3DDevice8* device;
    std::vector<Pair> stored_pairs;
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
    if (!device || !Zeal::EqGame::get_self())
        return;

    render_release_resources();  // Forces update of all graphics.

    const ZoneMapData* zone_map_data = get_zone_map_data(zone_id);
    if (!zone_map_data) {
        return;
    }

    // In some Zoom transitions, we lock the position on the screen, so calculate the current
    // position in screen coordinate using the existing scales and offsets.
    // Translate loc (world) coordinates to screen coordinates. Note that the game
    // world coordinates are negated (polarity flipped) to match the map data.
    const Vec3 position = Zeal::EqGame::get_self()->Position;
    const float position_y = -position.x; // Note position is y,x,z.
    const float position_x = -position.y;
    float screen_position_y = position_y * scale_factor + offset_y;
    float screen_position_x = position_x * scale_factor + offset_x;

    // Notes on scaling the map_data world coordinates to map screen window coordinates.
    //   The map data coordinate system is in the game world scale but negated y and x values.
    //     map_data: +y = south, +x = east.  game location: +y = north, +x = east.
    //   The map_data polarity is consistent with the screen coordinate system for North = up.
    //   window_x[i] = (map_x[i] - min(map_x)) * window_size_x/map_size_x + window_x_offset
    //   window_x[i] = map_x[i] * scale_factor + offset where
    //     scale_factor = window_size_x/map_size_x and offset = window_x_offset - min(map_x)*scale_factor.
    const Vec2 screen_size = ZealService::get_instance()->dx->GetScreenRect();
    const float rect_left = map_rect_left * screen_size.x;
    const float rect_right = map_rect_right * screen_size.x;
    const float rect_top = map_rect_top * screen_size.y;
    const float rect_bottom = map_rect_bottom * screen_size.y;

    float window_size_x = rect_right - rect_left;
    float window_size_y = rect_bottom - rect_top;
    float scale_factor_x = window_size_x / (zone_map_data->max_x - zone_map_data->min_x);
    float scale_factor_y = window_size_y / (zone_map_data->max_y - zone_map_data->min_y);
    scale_factor = min(scale_factor_x, scale_factor_y);  // Preserve aspect ratio.

    // Calculate the initial offset based on alignment of the full map data.
    offset_y = rect_top - zone_map_data->min_y * scale_factor;
    if (map_alignment_state == AlignmentType::kRight)
        offset_x = rect_right - zone_map_data->max_x * scale_factor;
    else if (map_alignment_state == AlignmentType::kCenter)
        offset_x = (rect_right + rect_left) * 0.5 -
          (zone_map_data->max_x + zone_map_data->min_x) * 0.5f * scale_factor;
    else  // Default left alignment.
        offset_x = rect_left - zone_map_data->min_x * scale_factor;

    // Update the scale and offsets in zoom if enabled.
    if (zoom_factor > 1.f && Zeal::EqGame::get_self()) {
        scale_factor *= zoom_factor;  // Scale is easy.

        // We set offsets based on where we want the position marker to be on the screen.
        if (zoom_recenter_zone_id == zone_id) {
            screen_position_x = (rect_left + rect_right) * 0.5;
            screen_position_y = (rect_bottom + rect_top) * 0.5;
        }
        offset_x = screen_position_x - position_x * scale_factor;
        offset_y = screen_position_y - position_y * scale_factor;
    }
    zoom_recenter_zone_id = kInvalidZoneId;

    // Clip the background (and lines) to the available map data.
    clip_rect_left = max(rect_left, zone_map_data->min_x * scale_factor + offset_x);
    clip_rect_right = min(rect_right, zone_map_data->max_x * scale_factor + offset_x);
    clip_rect_top = max(rect_top, zone_map_data->min_y * scale_factor + offset_y);
    clip_rect_bottom = min(rect_bottom, zone_map_data->max_y * scale_factor + offset_y);

    const int kMaxLineCount = zone_map_data->num_lines;  // Allocate a buffer assuming all visible.
    std::unique_ptr<MapVertex[]> line_vertices = std::make_unique<MapVertex[]>(kMaxLineCount * 2);

    const TwoPoints clip_rect = { .x0 = clip_rect_left, .y0 = clip_rect_top, .x1 = clip_rect_right, .y1 = clip_rect_bottom };
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

    // Create the background as two triangles using 4 vertices.
    unsigned int alpha = static_cast<int>(map_background_alpha * 255 + 0.5f);
    auto background_color = D3DCOLOR_ARGB(alpha, 0, 0, 0);  // Black or clear.
    if (map_background_state == BackgroundType::kLight) {
        background_color = D3DCOLOR_ARGB(alpha, 160, 160, 160);  // Light grey.
    }
    else if (map_background_state == BackgroundType::kTan) {
        background_color = D3DCOLOR_ARGB(alpha, 240, 180, 140);  // Tan.
    }
    MapVertex background_vertices[kBackgroundVertices] = {
        { clip_rect_left, clip_rect_top, 0.5f, 1.f, background_color },  // x, y, z, rhw, color
        { clip_rect_right, clip_rect_top, 0.5f, 1.f, background_color },
        { clip_rect_left, clip_rect_bottom, 0.5f, 1.f, background_color },
        { clip_rect_right, clip_rect_bottom, 0.5f, 1.f, background_color },
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

    if (map_background_state != BackgroundType::kClear)
        render_background();  // Background supports alpha blending which needs pipeline.

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

void ZoneMap::render_background() {
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();

    // Enable alpha blending for the background
    std::vector<RenderStateStash::Pair> render_pairs;
    render_pairs.push_back({ D3DRS_CULLMODE, D3DCULL_NONE });
    render_pairs.push_back({ D3DRS_ALPHABLENDENABLE, TRUE });
    render_pairs.push_back({ D3DRS_SRCBLEND, D3DBLEND_SRCALPHA });
    render_pairs.push_back({ D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA });
    render_pairs.push_back({ D3DRS_ZENABLE, TRUE });
    render_pairs.push_back({ D3DRS_ZWRITEENABLE, TRUE });  // Enable depth writing
    render_pairs.push_back({ D3DRS_LIGHTING, FALSE });  // Disable lighting
    RenderStateStash render_state(device);
    render_state.store_and_modify(render_pairs);

    // Set texture stage states to avoid any unexpected texturing
    std::vector<TextureStateStash::Pair> texture_pairs;
    texture_pairs.push_back({ D3DTSS_COLOROP, D3DTOP_SELECTARG1 });
    texture_pairs.push_back({ D3DTSS_COLORARG1, D3DTA_DIFFUSE });
    texture_pairs.push_back({ D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 });
    texture_pairs.push_back({ D3DTSS_ALPHAARG1, D3DTA_DIFFUSE });
    TextureStateStash texture_state(device);
    texture_state.store_and_modify(texture_pairs);

    // Background vertices are stored at the start of the line_vertex_buffer.
    device->SetStreamSource(0, line_vertex_buffer, sizeof(MapVertex));
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, kBackgroundCount);

    render_state.restore_state();
    texture_state.restore_state();
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

    // Allow the position marker to exceed the rect limits by size but must stay on screen.
    const float clip_left = max(clip_rect_left - size, 0);
    const float clip_top = max(clip_rect_top - size, 0);
    const float clip_right = min(clip_rect_right + size, screen_size.x - 1);
    const float clip_bottom = min(clip_rect_bottom + size, screen_size.y - 1);
    MapVertex position_vertices[kPositionVertices];
    for (int i = 0; i < kPositionVertices; ++i) {
        position_vertices[i].x = max(clip_left, min(clip_right, symbol[i].x + screen_x));
        position_vertices[i].y = max(clip_top, min(clip_bottom, symbol[i].y + screen_y));
        position_vertices[i].z = 0.5f;
        position_vertices[i].rhw = 1.f;
        position_vertices[i].color = D3DCOLOR_XRGB(250, 250, 51);  // Lemon yellow.
    }

    // Note: The limit is set low to avoid issues on maps that allow movement to the edge (zone
    // boundaries) that would trigger excessive retries.
    float limit = 1.f;
    if (zoom_factor > 1.f && (clip_rect_bottom - screen_y < limit || screen_y - clip_rect_top < limit ||
        clip_rect_right - screen_x < limit || screen_x - clip_rect_left < limit)) {
        zoom_recenter_zone_id = zone_id;  // Trigger it to re-center the zoom region.
        zone_id = kInvalidZoneId; // Triggers the update.
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

    // Skip drawing the marker if the center is off the map.
    const float rect_left = map_rect_left * screen_size.x;
    const float rect_right = map_rect_right * screen_size.x;
    const float rect_top = map_rect_top * screen_size.y;
    const float rect_bottom = map_rect_bottom * screen_size.y;
    if ((screen_x <= rect_left) || (screen_x >= rect_right) ||
        (screen_y <= rect_top) || (screen_y >= rect_bottom))
        return;

    // Add vertices constraining to fit within the valid screen.
    MapVertex marker_vertices[kNumVertices];
    for (int i = 0; i < kNumVertices; ++i) {
        marker_vertices[i].x = max(0, min(screen_size.x - 1, marker[i].x + screen_x));
        marker_vertices[i].y = max(0, min(screen_size.y - 1, marker[i].y + screen_y));
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
        if (zone_id != kInvalidZoneId)  // Not internally triggered so probably a zone event.
            zoom_factor = 1.f;  // Reset zoom factor on a zone to reduce corner cases.
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
    enabled = _enabled;
    if (!enabled) {
        render_release_resources();
        zone_id = kInvalidZoneId;  // Triggers update when re-enabled.
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapEnabled", enabled);

    update_ui_options();
}

void ZoneMap::toggle_background() {
    map_background_state = BackgroundType::e(static_cast<int>(map_background_state) + 1);
    if (map_background_state > BackgroundType::kLast) {
        map_background_state = BackgroundType::kFirst;
    }
    zone_id = kInvalidZoneId;  // Triggers reload.
    update_ui_options();
}

bool ZoneMap::set_background(int new_state_in, bool update_default) {
    BackgroundType::e new_state = BackgroundType::e(new_state_in);
    if ((new_state < BackgroundType::kFirst) || (new_state > BackgroundType::kLast)) {
        update_ui_options();  // Keep UI in sync when fails.
        return false;
    }

    map_background_state = new_state;
    zone_id = kInvalidZoneId;  // Triggers reload.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapBackgroundState", new_state);

    update_ui_options();
    return true;
}

bool ZoneMap::set_background_alpha(int percent, bool update_default) {
    map_background_alpha = max(0, min(1.f, percent / 100.f));
    zone_id = kInvalidZoneId;  // Triggers reload.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapBackgroundAlpha", map_background_alpha);

    update_ui_options();
    return true;
}



bool ZoneMap::set_alignment(int alignment_in, bool update_default) {
    AlignmentType::e alignment{ alignment_in };
    alignment = max(AlignmentType::kFirst, min(AlignmentType::kLast, alignment));

    map_alignment_state = alignment;
    zone_id = kInvalidZoneId;  // Triggers reload.

    set_zoom(100);  // Reset zoom on an alignment change to avoid confusion and corner cases.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
            ZealService::get_instance()->ini->setValue<int>("Zeal", "MapAlignment", alignment);
 
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
    int zone_id = marker_zone_id;  // Save across clear.
    clear_marker();  // Release any resources (also triggers update).
    marker_zone_id = zone_id;
    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<float>("Zeal", "MapMarkerSize", marker_size);

    update_ui_options();
    return true;  // Just clamp and report success.
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
    zone_id = kInvalidZoneId;  // Triggers reload (including marker).
    zoom_factor = new_zoom_factor;
    update_ui_options();
}

bool ZoneMap::set_zoom(int zoom_percent) {
    zone_id = kInvalidZoneId;  // Triggers reload (including marker).
    zoom_factor = zoom_percent * 0.01f;
    zoom_factor = min(100.f, max(1.f, zoom_factor));

    update_ui_options();
    return true;  // Just clamp and return success.
}

void ZoneMap::load_ini(IO_ini* ini)
{
    if (!ini->exists("Zeal", "MapEnabled"))
        ini->setValue<bool>("Zeal", "MapEnabled", false);
    if (!ini->exists("Zeal", "MapBackgroundState"))
        ini->setValue<int>("Zeal", "MapBackgroundState", static_cast<int>(BackgroundType::kClear));
    if (!ini->exists("Zeal", "MapBackgroundAlpha"))
        ini->setValue<float>("Zeal", "MapBackgroundAlpha", kDefaultBackgroundAlpha);
    if (!ini->exists("Zeal", "MapAlignment"))
        ini->setValue<int>("Zeal", "MapAlignment", static_cast<int>(AlignmentType::kFirst));
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
    map_background_state = BackgroundType::e(ini->getValue<int>("Zeal", "MapBackgroundState"));
    map_background_alpha = ini->getValue<float>("Zeal", "MapBackgroundAlpha");
    map_alignment_state = AlignmentType::e(ini->getValue<int>("Zeal", "MapAlignment"));
    map_rect_top = ini->getValue<float>("Zeal", "MapRectTop");
    map_rect_left = ini->getValue<float>("Zeal", "MapRectLeft");
    map_rect_bottom = ini->getValue<float>("Zeal", "MapRectBottom");
    map_rect_right = ini->getValue<float>("Zeal", "MapRectRight");
    position_size = ini->getValue<float>("Zeal", "MapPositionSize");
    marker_size = ini->getValue<float>("Zeal", "MapMarkerSize");

    // Sanity clamp ini values.
    map_background_state = max(BackgroundType::kFirst, min(BackgroundType::kLast, map_background_state));
    map_background_alpha = max(0, min(1.f, map_background_alpha));
    map_alignment_state = max(AlignmentType::kFirst, min(AlignmentType::kLast, map_alignment_state));
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

void ZoneMap::update_ui_options() {
    if (ZealService::get_instance() && ZealService::get_instance()->ui
                    && ZealService::get_instance()->ui->options)
       ZealService::get_instance()->ui->options->UpdateOptionsMapOnly();
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

    if ((alignment < AlignmentType::kFirst) || !set_alignment(alignment)) {
        Zeal::EqGame::print_chat("Usage: /map alignment left,center,right");
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

    const ZoneMapData* zone_map_data = get_zone_map_data(self->ZoneId);
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

void ZoneMap::dump() {
    Zeal::EqGame::print_chat("enabled: %i, background: %i (%.2f), align: %i, zone: %i",
                            enabled, map_background_state, map_background_alpha, map_alignment_state, zone_id);
    Zeal::EqGame::print_chat("marker: zone: %i, y: %i, x: %i", marker_zone_id, marker_y, marker_x);
    Zeal::EqGame::print_chat("rect: t: %f, l: %f, b: %f, r: %f", map_rect_top, map_rect_left, map_rect_bottom, map_rect_right);
    Zeal::EqGame::print_chat("clip: t: %f, l: %f, b: %f, r: %f", clip_rect_top, clip_rect_left, clip_rect_bottom, clip_rect_right);
    Zeal::EqGame::print_chat("position_size: %f, marker_size: %f, zoom_id: %i", position_size, marker_size, zoom_recenter_zone_id);
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
    else if (args[1] == "dump") {
        dump();
    }
    else if (!parse_shortcuts(args)) {
        Zeal::EqGame::print_chat("Usage: /map [on|off|size|alignment|marker|background|zoom|poi]");
        Zeal::EqGame::print_chat("Shortcuts: /map <y> <x>, /map 0, /map <poi_search_term>");
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