#include "zone_map.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include "zone_map_data.h"
#include <fstream>
#include <string>

namespace {

// Possible enhancements:
// - Current design was intended as a static background map with minimal use of modifying
//   the D3D matrices or use of D3D clip planes, which is suboptimal for the always_center_align
//   type of operation which requires frequent full map reloads
// - Support 'windowed mode' (drag and drop, resizing)
// - Support rendering outside of viewport
// - Look into intermittent z-depth clipping due to walls or heads/faces

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

static constexpr DWORD kMapVertexFvfCode = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

// Group member position support.
using GroupNameArrayType = char[0x40];
using GroupEntityPtrArrayType = Zeal::EqStructures::Entity*;

GroupEntityPtrArrayType* groupEntityPtrs = reinterpret_cast<GroupEntityPtrArrayType*>(0x7913F8);
GroupNameArrayType* groupNames = reinterpret_cast<GroupNameArrayType*>(0x7912B5);

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
            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

            float x = (outcodeOut == outcode0) ? line.x1 : line.x0;  // Satisfy compiler.
            float y = (outcodeOut == outcode0) ? line.y1 : line.x0;

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
}  // namespace

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
    if (label_font) {
        label_font->Release();
        label_font = nullptr;
    }
}

void ZoneMap::render_load_map() {
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    if (!device || !Zeal::EqGame::get_self())
        return;

    render_release_resources();  // Forces update of all graphics.

    const ZoneMapData* zone_map_data = get_zone_map(zone_id);
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
    device->GetViewport(&viewport);  // Update viewport (drawable screen size and offset).
    const float rect_left = std::floor(map_rect_left * viewport.Width) + viewport.X;
    const float rect_right = std::ceil(map_rect_right * viewport.Width) + viewport.X - 1;
    const float rect_top = std::floor(map_rect_top * viewport.Height) + viewport.Y;
    const float rect_bottom = std::ceil(map_rect_bottom * viewport.Height) + viewport.Y - 1;

    const float kPadding = 1.f;  // Small padding to prevent any clipping of edge lines.
    float rect_delta_x = rect_right - rect_left;
    float rect_delta_y = rect_bottom - rect_top;
    float scale_factor_x = rect_delta_x / (zone_map_data->max_x - zone_map_data->min_x + 2*kPadding);
    float scale_factor_y = rect_delta_y / (zone_map_data->max_y - zone_map_data->min_y + 2*kPadding);
    scale_factor = min(scale_factor_x, scale_factor_y);  // Preserve aspect ratio.
    scale_factor *= zoom_factor;  // Apply scale factor.

    // Calculate the initial offset based on alignment of the full map data.
    offset_y = rect_top - zone_map_data->min_y * scale_factor + kPadding;
    if (map_alignment_state == AlignmentType::kRight)
        offset_x = rect_right - zone_map_data->max_x * scale_factor - kPadding;
    else if (map_alignment_state == AlignmentType::kCenter)
        offset_x = (rect_right + rect_left) * 0.5 -
        (zone_map_data->max_x + zone_map_data->min_x) * 0.5f * scale_factor;
    else  // Default left alignment.
        offset_x = rect_left - zone_map_data->min_x * scale_factor + kPadding;

    bool align_to_center = always_align_to_center;
    if (!align_to_center && zoom_factor > 1.f) {
        // Initially set offset so the screen position doesn't change and then
        // perform the check to see if it needs to recenter.
        offset_x = screen_position_x - position_x * scale_factor;
        offset_y = screen_position_y - position_y * scale_factor;
        align_to_center = render_check_for_zoom_recenter();
    }

    if (align_to_center) {
        screen_position_x = (rect_left + rect_right) * 0.5;
        screen_position_y = (rect_bottom + rect_top) * 0.5;
        offset_x = screen_position_x - position_x * scale_factor;
        offset_y = screen_position_y - position_y * scale_factor;
    }

    // Clip the background (and lines) to the available map data. Add a padding of 1.
    clip_rect_left = min(rect_right,
        max(rect_left, zone_map_data->min_x * scale_factor + offset_x - kPadding));
    clip_rect_right = max(clip_rect_left,
        min(rect_right, zone_map_data->max_x * scale_factor + offset_x + kPadding));
    clip_rect_top = min(rect_bottom,
        max(rect_top, zone_map_data->min_y * scale_factor + offset_y - kPadding));
    clip_rect_bottom = max(clip_rect_top,
        min(rect_bottom, zone_map_data->max_y * scale_factor + offset_y + kPadding));

    const int kMaxLineCount = zone_map_data->num_lines;  // Allocate a buffer assuming all visible.
    std::unique_ptr<MapVertex[]> line_vertices = std::make_unique<MapVertex[]>(kMaxLineCount * 2);

    bool z_filtering = (map_level_zone_id == zone_id);
    int level_id = z_filtering ? zone_map_data->levels[map_level_index].level_id : kZoneMapInvalidLevelId;
    clip_max_z = z_filtering ? zone_map_data->levels[map_level_index].max_z : zone_map_data->max_z;
    clip_max_z += 10;  // Pad up for player height and other labels.
    clip_min_z = z_filtering ? zone_map_data->levels[map_level_index].min_z : zone_map_data->min_z;
    const TwoPoints clip_rect = { .x0 = clip_rect_left, .y0 = clip_rect_top, .x1 = clip_rect_right, .y1 = clip_rect_bottom };
    line_count = 0;  // Tracks number of visible (after clipping) lines.
    for (int i = 0; i < kMaxLineCount; ++i) {
        if (z_filtering) {
            if (zone_map_data->lines[i].level_id == kZoneMapInvalidLevelId) {
                // Filter all unknown lines based on z height range.
                int line_max_z = max(zone_map_data->lines[i].z0, zone_map_data->lines[i].z1);
                int line_min_z = min(zone_map_data->lines[i].z0, zone_map_data->lines[i].z1);
                if (line_max_z < clip_min_z || line_min_z > clip_max_z)
                    continue; // Skip if line does not cross level.
            } else if (zone_map_data->lines[i].level_id != level_id)
                    continue;  // Skip display of all other map levels.
        }
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
    if (FAILED(device->CreateVertexBuffer(kPositionBufferSize,
        D3DUSAGE_WRITEONLY,
        kMapVertexFvfCode,
        D3DPOOL_MANAGED,
        &position_vertex_buffer))) {
        return;
    }

    render_load_labels(*zone_map_data);
}

void ZoneMap::render_load_labels(const ZoneMapData& zone_map_data) {
    labels_list.clear();

    // Sneak in the current Level filter setting as another label.
    if (map_level_zone_id == zone_id) {
        snprintf(map_level_label_string, sizeof(map_level_label_string), "Level: %i", map_level_index);
        map_level_label_string[sizeof(map_level_label_string) - 1] = 0;
        map_level_label.x = static_cast<int16_t>((clip_rect_left - offset_x + 25) / scale_factor);
        map_level_label.y = static_cast<int16_t>((clip_rect_top - offset_y + 15) / scale_factor);
        map_level_label.z = clip_min_z;
        map_level_label.red = 255;
        map_level_label.green = 255;
        map_level_label.blue = 255;
        map_level_label.label = map_level_label_string;
        labels_list.push_back(&map_level_label);
    }

    int num_labels_to_scan = zone_map_data.num_labels;
    if (map_labels_mode == LabelsMode::kOff)
        num_labels_to_scan = 0;  // Disable the scan below.

    // Scan through POI's adding to the list if appropriate.
    for (int i = 0; i < num_labels_to_scan; ++i) {
        const ZoneMapLabel& label = zone_map_data.labels[i];

        // First do a quick vertical z-axis check.
        if (label.z < clip_min_z || label.z > clip_max_z)
            continue;  // Not on visible level, skip it.

        // Then check if the label is visible on the clipped map rect.
        float label_x = label.x * scale_factor + offset_x;
        float label_y = label.y * scale_factor + offset_y;
        if ((label_x < clip_rect_left) || (label_x > clip_rect_right) ||
            (label_y < clip_rect_top) || (label_y > clip_rect_bottom))
            continue;  // Off-screen, skip the label.

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

    if (!labels_list.empty() || !dynamic_labels_list.empty())
        render_load_font();
}

void ZoneMap::render_load_font() {
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    if (!device)
        return;

    if (label_font) {
        label_font->Release();
        label_font = nullptr;
    }
    LOGFONT log_font = {
            14, //height
            0,  //width
            0,  // lfEscapement
            0,  //lfOrientation
            FW_NORMAL, // lfWeight
            FALSE, // lfItalic
            FALSE, // lfUnderline
            FALSE, // lfStrikeOut
            DEFAULT_CHARSET, // lfCharSet
            OUT_DEFAULT_PRECIS, //lfOutPrecision
            CLIP_DEFAULT_PRECIS, // lfClipPrecision
            ANTIALIASED_QUALITY,// lfQuality
            DEFAULT_PITCH,// lfPitchAndFamily
            L"Arial"// lfFaceName[LF_FACESIZE]
    };
    HRESULT hr = D3DXCreateFontIndirect(device, &log_font, &label_font);
    if (FAILED(hr))
        label_font = nullptr;  // Just in case.
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

    if (marker_vertex_buffer) {
        device->SetStreamSource(0, marker_vertex_buffer, sizeof(MapVertex));
        device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, kMarkerCount);
    }

    render_labels();

    render_positions();
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

void ZoneMap::render_labels() {
    if (!label_font || (labels_list.empty() && dynamic_labels_list.empty()))
        return;

    for (const ZoneMapLabel* label : labels_list)
        render_label_text(label->label, label->y, label->x, D3DCOLOR_XRGB(label->red, label->green, label->blue));

    ULONGLONG timestamp = GetTickCount64();
    for (auto it = dynamic_labels_list.begin(); it != dynamic_labels_list.end();) {
        if (it->timeout == 0 || it->timeout >= timestamp) {
            render_label_text(it->label.c_str(), -it->loc_y, -it->loc_x, it->color);
            it++;
        }
        else {
            it = dynamic_labels_list.erase(it);  // Drop timed out labels.
        }
    }
}


void ZoneMap::render_label_text(const char * label, int y, int x, D3DCOLOR font_color) {
    const int view_left = viewport.X;
    const int view_top = viewport.Y;
    const int view_right = viewport.X + viewport.Width - 1;
    const int view_bottom = viewport.Y + viewport.Height - 1;

    // Use colorful labels but override the black ones on most map backgrounds.
    if (font_color == D3DCOLOR_XRGB(0, 0, 0))
        font_color = D3DCOLOR_XRGB(192, 192, 192);  // Flip to light grey.

    // Calculate and clip the on-screen coordinate position of the text.
    const int kHalfHeight = 25;  // Centered so just has to be sufficiently large.
    const int kHalfWidth = 60;
    int label_x = static_cast<int>(x * scale_factor + offset_x + 0.5f);
    int label_y = static_cast<int>(y * scale_factor + offset_y + 0.5f);
    RECT text_rect = { .left = label_x - kHalfWidth, .top = label_y - kHalfHeight,
                    .right = label_x + kHalfWidth, .bottom = label_y + kHalfHeight };
    text_rect.left = max(view_left, min(view_right, text_rect.left));
    text_rect.right = max(view_left, min(view_right, text_rect.right));
    text_rect.top = max(view_top, min(view_bottom, text_rect.top));
    text_rect.bottom = max(view_top, min(view_bottom, text_rect.bottom));
    int length = strlen(label);
    if (length > 20) {
        length = 20;  // Truncate it to 20.
        for (int i = 1; i < length - 1; ++i) {
            if (label[i] == '_' && label[i + 1] == '(')  // Extra info in () to drop.
                length = i;  // Truncates and breaks loop.
        }
    }

    label_font->DrawTextA(label, length, &text_rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE, font_color);
}


void ZoneMap::add_position_marker_vertices(float screen_y, float screen_x, float heading, float size,
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

    // Allow the position marker to exceed the rect limits by size but must stay on screen.
    const int view_left = viewport.X;
    const int view_top = viewport.Y;
    const int view_right = viewport.X + viewport.Width - 1;
    const int view_bottom = viewport.Y + viewport.Height - 1;
    const float clip_left = max(clip_rect_left - size, view_left);
    const float clip_top = max(clip_rect_top - size, view_top);
    const float clip_right = min(clip_rect_right + size, view_right);
    const float clip_bottom = min(clip_rect_bottom + size, view_bottom);
    for (int i = 0; i < kPositionVertices; ++i) {
        vertices.push_back(MapVertex{
                .x = max(clip_left, min(clip_right, symbol[i].x + screen_x)),
                .y = max(clip_top, min(clip_bottom, symbol[i].y + screen_y)),
                .z = 0.5f,
                .rhw = 1.f,
                .color = color
            });
    }
}

void ZoneMap::add_group_member_position_vertices(std::vector<MapVertex>& vertices) const {
    if (!map_show_group)
        return;

    const float kShrinkFactor = 0.8f;  // Make group members 20% smaller.
    float size = position_size * min(viewport.Width, viewport.Height) * kShrinkFactor;
    size = max(5.f, size);  // Constrain so it remains visible.

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

        float screen_y = -member->Position.x * scale_factor + offset_y;  // Position is y,x,z.
        float screen_x = -member->Position.y * scale_factor + offset_x;
        add_position_marker_vertices(screen_y, screen_x, member->Heading, size,
            kGroupColorLut[i], vertices);
    }
}

void ZoneMap::render_group_member_labels() {
    if (!map_show_group_labels)
        return;

    if (!label_font) {
        render_load_font();
        if (!label_font)
            return;
    }

    for (int i = 0; i < EQ_NUM_GROUP_MEMBERS; ++i) {
        Zeal::EqStructures::Entity* member = groupEntityPtrs[i];
        if ((strlen(groupNames[i]) == 0) || !member)
            continue;  // Not a valid group member (or member = nullptr when out of zone).

        // Writes the character group number (F2 - F6) centered at the character position.
        int loc_y = static_cast<int>(member->Position.x + 0.5f);  // Position is y,x,z.
        int loc_x = static_cast<int>(member->Position.y + 0.5f);  // Also need to negate it below.
        const char label[] = { i + '2', 0};
        render_label_text(label,-loc_y, -loc_x, D3DCOLOR_XRGB(255, 255, 255));
    }
}

void ZoneMap::add_raid_marker_vertices(float screen_y, float screen_x, float size,
    D3DCOLOR color, std::vector<MapVertex>& vertices) const {

    // Add a simple equilateral triangle centered at the position pointing up.
    const float kFactor1 = 0.577350f; // 1 / sqrt(3)
    const float kFactor2 = 0.288675f; // 1 / (2 * sqrt(3))
    Vec3 vertex0 = { 0, -size * kFactor1, 0 };
    Vec3 vertex1 = { +size * 0.5f, size * kFactor2, 0 };
    Vec3 vertex2 = { -size * 0.5f, size * kFactor2, 0 };
    Vec3 symbol[kRaidPositionVertices] = { vertex0, vertex1, vertex2};

    // Allow the position marker to exceed the rect limits by size but must stay on screen.
    const int view_left = viewport.X;
    const int view_top = viewport.Y;
    const int view_right = viewport.X + viewport.Width - 1;
    const int view_bottom = viewport.Y + viewport.Height - 1;
    const float clip_left = max(clip_rect_left - size, view_left);
    const float clip_top = max(clip_rect_top - size, view_top);
    const float clip_right = min(clip_rect_right + size, view_right);
    const float clip_bottom = min(clip_rect_bottom + size, view_bottom);
    for (int i = 0; i < kRaidPositionVertices; ++i) {
        vertices.push_back(MapVertex{
                .x = max(clip_left, min(clip_right, symbol[i].x + screen_x)),
                .y = max(clip_top, min(clip_bottom, symbol[i].y + screen_y)),
                .z = 0.5f,
                .rhw = 1.f,
                .color = color
            });
    }
}

void ZoneMap::add_raid_member_position_vertices(std::vector<MapVertex>& vertices) const {
    if (!map_show_raid)
        return;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    auto entity_manager = ZealService::get_instance()->entity_manager.get();  // Short-term ptr.
    if (!entity_manager || !self)
        return;

    float size = position_size * min(viewport.Width, viewport.Height);
    size = max(5.f, size);  // Constrain so it remains visible.

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
        float screen_y = -entity->Position.x * scale_factor + offset_y;  // Position is y,x,z.
        float screen_x = -entity->Position.y * scale_factor + offset_x;
        add_raid_marker_vertices(screen_y, screen_x, size, color, vertices);
    }
}


void ZoneMap::render_positions() {
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!position_vertex_buffer || !self || !device) {
        return;
    }

    // Populate a vector with D3DPT_TRIANGLELIST vertices.
    std::vector<MapVertex> position_vertices;
    add_raid_member_position_vertices(position_vertices);
    add_group_member_position_vertices(position_vertices);

    int other_triangle_vertices = position_vertices.size();

    // Translate loc (world) coordinates to screen coordinates. Note that the game
    // world coordinates are negated (polarity flipped) to match the map data.
    Vec3 position = self->Position;
    float screen_y = -position.x * scale_factor + offset_y;  // Note position is y,x,z.
    float screen_x = -position.y * scale_factor + offset_x;

    float size = position_size * min(viewport.Width, viewport.Height);
    size = max(5.f, size);  // Constrain so it remains visible.

    // Use default cursor color if levels not active or within the z clipping range.
    bool use_default_color = (map_level_zone_id != zone_id) ||
                (position.z >= clip_min_z && position.z <= clip_max_z);
    auto color = use_default_color ? D3DCOLOR_XRGB(250, 250, 51) : // Lemon yellow
        D3DCOLOR_XRGB(195, 176, 145); // Lemon khaki.

    add_position_marker_vertices(screen_y, screen_x, self->Heading, size,
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

    device->SetStreamSource(0, position_vertex_buffer, sizeof(MapVertex));

    // First draw the "other" (raid, group) markers.
    int other_triangle_count = other_triangle_vertices / 3; // D3DPT_TRIANGLELIST
    if (other_triangle_count)
        device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, other_triangle_count);

    // Then draw the group labels (on top of markers but below self marker).
    render_group_member_labels();

    // And finally draw the self marker. Three vertices per triangle in D3DPT_TRIANGLELIST.
    int self_triangle_count = position_vertices.size() / 3 - other_triangle_count;
    device->DrawPrimitive(D3DPT_TRIANGLELIST, other_triangle_vertices, self_triangle_count);
}

bool ZoneMap::render_check_for_zoom_recenter() {
    if (always_align_to_center)
        return true;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (zone_id == kInvalidZoneId || zoom_factor <= 1.f || !self)
        return false;

    // Trigger a re-centering in zoom if:
    // (1) the marker position is within 20% of an edge and that edge isn't already
    //    at the maximum of the map data and there is movement in that direction
    // (2) or recenter if the screen x,y is outside the clip_rect (fallback case)
    float delta_y = -self->MovementSpeedY;  // World direction is negated.
    float delta_x = -self->MovementSpeedX;

    // Translate loc (world) coordinates to screen coordinates. Note that the game
    // world coordinates are negated (polarity flipped) to match the map data.
    float screen_y = -self->Position.x * scale_factor + offset_y;  // Note position is y,x,z.
    float screen_x = -self->Position.y * scale_factor + offset_x;

    const ZoneMapData* zone_map_data = get_zone_map(zone_id);
    float vertical_limit = (clip_rect_bottom - clip_rect_top) * 0.20f;
    float map_top = zone_map_data->min_y * scale_factor + offset_y;
    bool trigger = ((delta_y < 0) && (screen_y - clip_rect_top < vertical_limit) &&
        (clip_rect_top > map_top)) || (screen_y < clip_rect_top);
    float map_bottom = zone_map_data->max_y * scale_factor + offset_y;
    trigger = trigger || ((delta_y > 0) && (clip_rect_bottom - screen_y < vertical_limit) &&
        (clip_rect_bottom < map_bottom)) || (screen_y > clip_rect_bottom);
    float horizontal_limit = (clip_rect_right - clip_rect_left) * 0.20f;
    float map_left = zone_map_data->min_x * scale_factor + offset_x;
    trigger = trigger || ((delta_x < 0) && (screen_x - clip_rect_left < horizontal_limit) &&
        (clip_rect_left > map_left)) || (screen_x < clip_rect_left);
    float map_right = zone_map_data->max_x * scale_factor + offset_x;
    trigger = trigger || ((delta_x > 0) && (clip_rect_right - screen_x < horizontal_limit) &&
        (clip_rect_right < map_right)) || (screen_x > clip_rect_right);

    return trigger;
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
    float size = max(5.f, marker_size * min(viewport.Width, viewport.Height) * 0.5f);
    float short_size = size * 0.25f;
    const int kNumVertices = kMarkerCount * 3;  // Separate triangles with 3 vertices (CW direction).
    Vec3 marker[kNumVertices] = {{0, 0, 0}, {short_size, size, 0}, {-short_size, size, 0},
                                    {0, 0, 0}, {-short_size, -size, 0}, {short_size, -size, 0},
                                    {0, 0, 0}, {size, -short_size, 0}, {size, short_size, 0},
                                    {0, 0, 0}, {-size, short_size, 0}, {-size, -short_size, 0}
    };

    // Skip drawing the marker if the center is off the map.
    if ((screen_x <= clip_rect_left) || (screen_x >= clip_rect_right) ||
        (screen_y <= clip_rect_top) || (screen_y >= clip_rect_bottom))
        return;

    // Add vertices constraining to fit within the valid screen.
    const int view_left = viewport.X;
    const int view_top = viewport.Y;
    const int view_right = viewport.X + viewport.Width - 1;
    const int view_bottom = viewport.Y + viewport.Height - 1;
    MapVertex marker_vertices[kNumVertices];
    for (int i = 0; i < kNumVertices; ++i) {
        marker_vertices[i].x = max(view_left, min(view_right, marker[i].x + screen_x));
        marker_vertices[i].y = max(view_top, min(view_bottom, marker[i].y + screen_y));
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
    if (zone_id != self->ZoneId || (always_align_to_center && self->MovementSpeed > 0) 
        || render_check_for_zoom_recenter()) {
        zone_id = self->ZoneId;
        render_load_map();
    }

    // Add the marker if it is empty and this is the right zone.
    if (zone_id != kInvalidZoneId && marker_zone_id == zone_id
        && marker_vertex_buffer == nullptr) {
        render_update_marker_buffer();
    }

    render_map();
}


void ZoneMap::add_dynamic_label(const std::string& label_text, int loc_y, int loc_x,
    unsigned int duration_ms, D3DCOLOR font_color) {
    if (label_text.empty() || zone_id == kInvalidZoneId)
        return;

    if (label_font == nullptr)
        render_load_font();

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

void ZoneMap::set_show_group(bool enable, bool update_default) {
    if (map_show_group != enable) {
        map_show_group = enable;
        zone_id = kInvalidZoneId;  // Triggers reload.
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapShowGroup", map_show_group);

    update_ui_options();
}

void ZoneMap::set_show_group_labels(bool enable, bool update_default) {
    if (map_show_group_labels != enable) {
        map_show_group_labels = enable;
        zone_id = kInvalidZoneId;  // Triggers reload.
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapShowGroupLabels", map_show_group_labels);

    update_ui_options();
}

void ZoneMap::set_show_raid(bool enable, bool update_default) {
    if (map_show_raid != enable) {
        map_show_raid = enable;
        zone_id = kInvalidZoneId;  // Triggers reload.
    }

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<bool>("Zeal", "MapShowRaid", map_show_raid);

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

void ZoneMap::toggle_labels() {
    map_labels_mode = LabelsMode::e(static_cast<int>(map_labels_mode) + 1);
    if (map_labels_mode > LabelsMode::kLast) {
        map_labels_mode = LabelsMode::kFirst;
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

bool ZoneMap::set_labels_mode(int mode_in, bool update_default) {
    LabelsMode::e mode{ mode_in };
    mode = max(LabelsMode::kFirst, min(LabelsMode::kLast, mode));

    map_labels_mode = mode;
    zone_id = kInvalidZoneId;  // Triggers reload.

    if (update_default && ZealService::get_instance() && ZealService::get_instance()->ini)
        ZealService::get_instance()->ini->setValue<int>("Zeal", "MapLabelsMode", mode);

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
    if (!ini->exists("Zeal", "MapShowGroup"))
        ini->setValue<bool>("Zeal", "MapShowGroup", true);
    if (!ini->exists("Zeal", "MapShowGroupLabels"))
        ini->setValue<bool>("Zeal", "MapShowGroupLabels", false);
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
    map_show_group = ini->getValue<bool>("Zeal", "MapShowGroup");
    map_show_group_labels = ini->getValue<bool>("Zeal", "MapShowGroupLabels");
    map_show_raid = ini->getValue<bool>("Zeal", "MapShowRaid");
    map_data_mode = MapDataMode::e(ini->getValue<int>("Zeal", "MapDataMode"));
    map_background_state = BackgroundType::e(ini->getValue<int>("Zeal", "MapBackgroundState"));
    map_background_alpha = ini->getValue<float>("Zeal", "MapBackgroundAlpha");
    map_alignment_state = AlignmentType::e(ini->getValue<int>("Zeal", "MapAlignment"));
    map_labels_mode = LabelsMode::e(ini->getValue<int>("Zeal", "MapLabelsMode"));
    map_rect_top = ini->getValue<float>("Zeal", "MapRectTop");
    map_rect_left = ini->getValue<float>("Zeal", "MapRectLeft");
    map_rect_bottom = ini->getValue<float>("Zeal", "MapRectBottom");
    map_rect_right = ini->getValue<float>("Zeal", "MapRectRight");
    position_size = ini->getValue<float>("Zeal", "MapPositionSize");
    marker_size = ini->getValue<float>("Zeal", "MapMarkerSize");

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
    ini->setValue<bool>("Zeal", "MapShowGroup", map_show_group);
    ini->setValue<bool>("Zeal", "MapShowGroupLabels", map_show_group_labels);
    ini->setValue<bool>("Zeal", "MapShowRaid", map_show_raid);
    ini->setValue<int>("Zeal", "MapDataMode", static_cast<int>(map_data_mode));
    ini->setValue<int>("Zeal", "MapBackgroundState", static_cast<int>(map_background_state));
    ini->setValue<float>("Zeal", "MapBackgroundAlpha", map_background_alpha);
    ini->setValue<int>("Zeal", "MapAlignment", static_cast<int>(map_alignment_state));
    ini->setValue<int>("Zeal", "MapLabelsMode", static_cast<int>(map_labels_mode));
    ini->setValue<float>("Zeal", "MapRectTop", map_rect_top);
    ini->setValue<float>("Zeal", "MapRectLeft", map_rect_left);
    ini->setValue<float>("Zeal", "MapRectBottom", map_rect_bottom);
    ini->setValue<float>("Zeal", "MapRectRight", map_rect_right);
    ini->setValue<float>("Zeal", "MapPositionSize", position_size);
    ini->setValue<float>("Zeal", "MapMarkerSize", marker_size);
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
        (tlhw[1] + tlhw[3]) / 100.f), false)   // And width to right (left + width).
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
    }

    if ((labels < LabelsMode::kFirst) || !set_labels_mode(labels, false)) {
        Zeal::EqGame::print_chat("Usage: /map labels off,summary,all");
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
    else if (args.size() == 3 && args[2] == "labels") {
        set_show_group_labels(!is_show_group_labels_enabled(), false);
        Zeal::EqGame::print_chat("Showing group member labels is %s",
            is_show_group_labels_enabled() ? "ON" : "OFF");
    }
    else {
        Zeal::EqGame::print_chat("Usage: /map show_group (toggles on/off), /map show_group labels (toggles)");
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

void ZoneMap::dump() {
    Zeal::EqGame::print_chat("enabled: %i, background: %i (%.2f), align: %i, labels:%i, zone: %i",
                            enabled, map_background_state, map_background_alpha, map_alignment_state, map_labels_mode, zone_id);
    Zeal::EqGame::print_chat("marker: zone: %i, y: %i, x: %i, num_labels: %i", marker_zone_id, marker_y, marker_x, labels_list.size());
    Zeal::EqGame::print_chat("view: t: %i, l: %i, h: %i, w: %i", viewport.Y, viewport.X, viewport.Height, viewport.Width);
    Zeal::EqGame::print_chat("rect: t: %f, l: %f, b: %f, r: %f", map_rect_top, map_rect_left, map_rect_bottom, map_rect_right);
    Zeal::EqGame::print_chat("clip: t: %f, l: %f, b: %f, r: %f", clip_rect_top, clip_rect_left, clip_rect_bottom, clip_rect_right);
    Zeal::EqGame::print_chat("level: zone: %i, index: %i, z_max: %i, z_min: %i", map_level_zone_id, map_level_index, clip_max_z, clip_min_z);
    Zeal::EqGame::print_chat("position_size: %f, marker_size: %f, show_group: %i, %i, show_raid: %i",
        position_size, marker_size, map_show_group, map_show_group_labels, map_show_raid);
    Zeal::EqGame::print_chat("scale_factor: %f, offset_y: %f, offset_x: %f, zoom: %f", scale_factor, offset_y, offset_x, zoom_factor);
    Zeal::EqGame::print_chat("dyn_labels_size: %i, dyn_labels_zone_id: %i, data_mode: %i, data_cache: %i", 
        dynamic_labels_list.size(), dynamic_labels_zone_id, map_data_mode, map_data_cache.size());
    Zeal::EqGame::print_chat("line_count: %i, line: %i, position: %i, marker: %i, font: %i", line_count,
        line_vertex_buffer != nullptr, position_vertex_buffer != nullptr, marker_vertex_buffer != nullptr, label_font != nullptr);
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

ZoneMap::ZoneMap(ZealService* zeal, IO_ini* ini)
{
    load_ini(ini);
    zeal->callbacks->AddGeneric([this]() { callback_render(); }, callback_type::RenderUI);
    zeal->callbacks->AddGeneric([this]() { callback_zone(); }, callback_type::Zone);
    zeal->commands_hook->Add("/map", {}, "Controls map overlay",
        [this](const std::vector<std::string>& args) {
            return parse_command(args);
        });
}

ZoneMap::~ZoneMap()
{
    render_release_resources();
}