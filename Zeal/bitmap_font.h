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

// Directx 8 compatible class for fast text rendering using batch processing of bitmap glyphs.
class BitmapFont
{
public:
    static constexpr char kFontDirectoryPath[] = "./uifiles/zeal/fonts";
    static constexpr char kFontFileExtension[] = ".spritefont";
    static constexpr char kDefaultFontName[] = "default";

    // Recommended to use the create_bitmap_font factory to create a valid font object.
    // A font_filename of kDefaultFontName or empty will load the embedded font.
    // Releases resources and returns nullptr if unsuccessful.
    static std::unique_ptr<BitmapFont> create_bitmap_font(IDirect3DDevice8& device,
                                                const std::string& font_filename);

    // Utility class to report the available fonts.
    static std::vector<std::string> get_available_fonts();

    BitmapFont(IDirect3DDevice8& device, std::span<const uint8_t> data_span);
    BitmapFont(IDirect3DDevice8& device, const char* filename);
    virtual ~BitmapFont();

    // Disable copy.
    BitmapFont(BitmapFont const&) = delete;
    BitmapFont& operator= (BitmapFont const&) = delete;

    // Returns true if a valid bitmap texture is ready for use by the GPU.
    bool is_valid() const { return texture != nullptr; }

    // Primary interface for drawing text. The position is in screen pixel coordinates
    // and specifies the center (true) or the upper left (false).
    void queue_string(const char* text, const Vec2& position, bool center = true,
        const D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

    // Utilities for adjusting the string position (sizes in screen pixels).
    Vec2 measure_string(const char* text) const;
    RECT measure_draw_rect(const char* text, const Vec2& position) const;
    float get_line_spacing() const { return line_spacing; }

    // Renders any queued string content to the screen and clears the queue.
    // Note that the D3D stream source, indices, vertex shader, and texture states
    // are not preserved across this call.
    void flush_queue_to_screen();

    // Releases resources including DirectX. Must call on a DirectX reset / lost device.
    void release();  // Release resources.

private:
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

    // Vertices allow texturing and color modulation.
    struct GlyphVertex {
        static constexpr DWORD kGlyphVertexFvfCode = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

        float x, y, z, rhw;  // Transformed position coordinates and rhw.
        D3DCOLOR color;      // Color (modulates font color which is typically white).
        float u, v;          // Texture coordinates from D3DFVF_TEX1.
    };

    static constexpr int kNumGlyphs = 128;  // Support ASCII 0 - 127.

    const Glyph* get_glyph(char character) const;
    void create_texture(uint32_t width, uint32_t height, D3DFORMAT format,
        uint32_t stride, uint32_t rows, const uint8_t* data);
    bool create_index_buffer();
    void render_queue();
    void calculate_glyph_vertices(const GlyphQueueEntry& entry,
                                    GlyphVertex glyph_vertices[4]) const;

    template<typename TAction>
    void for_each_glyph(const char* text, TAction action) const;


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
    std::unique_ptr<GlyphVertex[]> vertices;  // Local CPU scratch.
};
