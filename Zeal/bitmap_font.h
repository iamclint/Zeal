//--------------------------------------------------------------------------------------
// This file was inspired by DirectX 11 SpriteFont.h
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include "directx.h"
#include "vectors.h"

// Directx 8 compatible class for fast text rendering using batch processing of bitmap glyphs.
class BitmapFont
{
public:
    BitmapFont(IDirect3DDevice8& device, std::span<const uint8_t> data_span);
    BitmapFont(IDirect3DDevice8& device, const char* filename);
    virtual ~BitmapFont();

    // Disable copy and move.
    BitmapFont(BitmapFont const&) = delete;
    BitmapFont& operator= (BitmapFont const&) = delete;
    BitmapFont(BitmapFont&&) = delete;
    BitmapFont& operator= (BitmapFont&&) = delete;

    bool is_valid() const { return texture != nullptr; }

    // Primary interface for drawing text. Centers the string at position (pixel coordinates).
    void queue_string(const char* text, const Vec2& position,
        const D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

    // Utilities for adjusting the string position (sizes in screen pixels).
    Vec2 measure_string(const char* text) const;
    RECT measure_draw_rect(const char* text, const Vec2& position) const;
    float get_line_spacing() const { return line_spacing; }

    // Renders any queued string content to the screen and clears the queue.
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
    static constexpr DWORD kGlyphVertexFvfCode = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    struct GlyphVertex {
        float x, y, z, rhw;  // Transformed position coordinates and rhw.
        D3DCOLOR color;      // Optional color (0 for white).
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
