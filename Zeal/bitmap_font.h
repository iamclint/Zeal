//--------------------------------------------------------------------------------------
// This file was inspired by DirectX 11 SpriteFont.h (MIT License)
// http://go.microsoft.com/fwlink/?LinkId=248929
//
// This is a simplified and rewritten implementation targeting DirectX8.
//--------------------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include "directx.h"
#include "vectors.h"

// Directx 8 compatible classes for fast text rendering using batch processing of bitmap glyphs.
// BitmapFont: Use for 2-D text that does not need z-buffer processing (occlusion, scaling).
// SpriteFont: Use for 2-D text that is in the 3D world space and needs z-buffer processing.

// Virtual base class with utility functions for handling fonts and some common processing.
class BitmapFontBase
{
public:
    static constexpr char kFontDirectoryPath[] = "./uifiles/zeal/fonts";
    static constexpr char kFontFileExtension[] = ".spritefont";
    static constexpr char kDefaultFontName[] = "default";

    // Utility method to report the available fonts.
    static std::vector<std::string> get_available_fonts();

    BitmapFontBase(IDirect3DDevice8& device, std::span<const uint8_t> data_span);
    BitmapFontBase(IDirect3DDevice8& device, const char* filename);
    virtual ~BitmapFontBase();

    // Disable copy.
    BitmapFontBase(BitmapFontBase const&) = delete;
    BitmapFontBase& operator= (BitmapFontBase const&) = delete;

    // Returns true if a valid bitmap texture is ready for use by the GPU.
    bool is_valid() const { return texture != nullptr; }

    // Utilities for adjusting the string position (sizes in screen pixels).
    Vec2 measure_string(const char* text) const;
    RECT measure_draw_rect(const char* text, const Vec2& position) const;
    float get_line_spacing() const { return line_spacing; }

    // Utilities for drop shadow effects.
    float calculate_shadow_offset() const;
    void set_drop_shadow(bool enable) { drop_shadow = enable; }
    void set_outlined(bool enable) { outlined = enable; }
    void set_align_bottom(bool enable) { align_bottom = enable; }
    
    // Primary interface for drawing text. The position is in screen pixel coordinates
    // and specifies the center (true) or the upper left (false).
    virtual void queue_string(const char* text, const Vec3& position, bool center = true,
        const D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255), bool grid_align = true);

    // Renders any queued string content to the screen and clears the queue.
    // Note that the D3D stream source, indices, vertex shader, and texture states
    // are not preserved across this call.
    virtual void flush_queue_to_screen();

    // Releases resources including DirectX. Must call on a DirectX reset / lost device.
    virtual void release();  // Note: No longer usable after this call (delete).

    virtual void dump() const;  // Print debug information like texture size.

protected:
    // Strings are split into multiple lines.
    struct Lines {
        std::string text;
        Vec2 upper_left;
    };

    // Describes a single character bitmap glyph.
    struct Glyph
    {
        uint32_t character = '\0';
        RECT sub_rect = { 0,0,0,0 };
        float x_offset = 0;
        float y_offset = 0;
        float x_advance = 0;
    };

    // Strings are stored in the queue as character glyphs.
    struct GlyphQueueEntry {
        const Glyph* glyph;
        const Vec2 position;
        const D3DCOLOR color;
    };

    static constexpr int kNumGlyphs = 128;  // Support ASCII 0 - 127.

    void queue_lines(const std::vector<Lines>& lines, D3DCOLOR color, Vec2 offset = Vec2(0,0));
    const Glyph* get_glyph(char character) const;
    void create_texture(uint32_t width, uint32_t height, D3DFORMAT format,
        uint32_t stride, uint32_t rows, const uint8_t* data);
    bool create_index_buffer();

    virtual DWORD get_fvf_code() const = 0;
    virtual DWORD get_vertex_size() const = 0;
    virtual void render_queue() = 0;

    template<typename TAction>
    void for_each_glyph(const char* text, TAction action) const;

    bool drop_shadow = false;
    bool outlined = false;
    bool align_bottom = false;  // Applies only when text is queued with center flag.

    IDirect3DDevice8& device;
    Glyph glyph_table[kNumGlyphs] = {};
    float line_spacing = 0;
    char default_character = '\0';
    std::vector<GlyphQueueEntry> glyph_queue;

    IDirect3DTexture8* texture = nullptr;
    Vec2 inverse_texture_size = {};
    IDirect3DVertexBuffer8* vertex_buffer = nullptr;
    IDirect3DIndexBuffer8* index_buffer = nullptr;
    int vertex_buffer_wr_index = 0;
};


// Directx 8 compatible class for fast text rendering using batch processing of bitmap glyphs.
// BitmapFont handles 2-D pre-transformed vertex rendering without the z-buffer.
class BitmapFont : public BitmapFontBase
{
public:
    // Use the create_bitmap_font factory to create a valid font object.
    // A font_filename of kDefaultFontName or empty will load the embedded font.
    // Releases resources and returns nullptr if unsuccessful.
    static std::unique_ptr<BitmapFont> create_bitmap_font(IDirect3DDevice8& device,
        const std::string& font_filename);

    // Use the class factory to create an object.
    BitmapFont(IDirect3DDevice8& device_in, std::span<const uint8_t> data_span) :
        BitmapFontBase(device_in, data_span) {}

    BitmapFont(IDirect3DDevice8& device_in, const char* filename) :
        BitmapFontBase(device_in, filename) {}

    virtual ~BitmapFont() { release(); }

    // Releases resources including DirectX. Must call on a DirectX reset / lost device.
    void release() override;  // Release resources.  No longer usable if this is used.

protected:
    // Vertices allow texturing and color modulation.
    struct GlyphVertex {
        static constexpr DWORD kGlyphVertexFvfCode = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

        float x, y, z, rhw;  // Transformed position coordinates and rhw.
        D3DCOLOR color;      // Color (modulates font color which is typically white).
        float u, v;          // Texture coordinates from D3DFVF_TEX1.
    };

    DWORD get_fvf_code() const override { return GlyphVertex::kGlyphVertexFvfCode; }
    DWORD get_vertex_size() const override { return sizeof(GlyphVertex); }
    void render_queue() override;
    void calculate_glyph_vertices(const GlyphQueueEntry& entry,
        GlyphVertex glyph_vertices[4]) const;

    std::unique_ptr<GlyphVertex[]> vertices;  // Local CPU scratch.
};

// Directx 8 compatible class for fast text rendering using batch processing of bitmap glyphs.
// BitmapFont handles 3-D text rendering where matrix ops and a z-buffer is needed. The text
// is rotated so it always faces the camera.
class SpriteFont : public BitmapFontBase
{
public:
    // Use the create_sprite_font factory to create a valid font object.
    // A font_filename of kDefaultFontName or empty will load the embedded font.
    // Releases resources and returns nullptr if unsuccessful.
    static std::unique_ptr<SpriteFont> create_sprite_font(IDirect3DDevice8& device,
        const std::string& font_filename);

    // Use the class factory to create an object.
    SpriteFont(IDirect3DDevice8& device_in, std::span<const uint8_t> data_span) :
        BitmapFontBase(device_in, data_span) {}

    SpriteFont(IDirect3DDevice8& device_in, const char* filename) :
        BitmapFontBase(device_in, filename) {}

    virtual ~SpriteFont() { release(); }

    void queue_string(const char* text, const Vec3& position, bool center = true,
        const D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255), bool grid_align = false) override;
    void flush_queue_to_screen() override;
    void release() override;

protected:
    struct GlyphString {
        Vec3 position;  // Origin anchor point for a string of glyphs.
        int start_index;
        int stop_index;  // Exclusive.
    };

    // Vertices allow texturing and color modulation.
    struct Glyph3DVertex {
        static constexpr DWORD kFvfCode = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

        float x, y, z;  // Transformed position coordinates and rhw.
        D3DCOLOR color;      // Color (modulates font color which is typically white).
        float u, v;          // Texture coordinates from D3DFVF_TEX1.
    };

    DWORD get_fvf_code() const override { return Glyph3DVertex::kFvfCode; }
    DWORD get_vertex_size() const override { return sizeof(Glyph3DVertex); }
    void render_queue() override;
    void calculate_glyph_vertices(const GlyphQueueEntry& entry,
        Glyph3DVertex glyph_vertices[4]) const;

    std::vector<GlyphString> glyph_string_queue;  // List of strings at different Vec3 positions.
    std::unique_ptr<Glyph3DVertex[]> vertices;  // Local CPU scratch.

};