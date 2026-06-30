#pragma once
#include <cstdint>

// Minimal 5x7 pixel font for status bar.
// We use TFT_eSPI's built-in GLCD font (5x7) via drawString().
// This header provides the font constants and helper for pixel-perfect text.

#define FONT_WIDTH   6
#define FONT_HEIGHT  8

// Pixel font data: ASCII 32-90 (space through Z), 5 columns each, 7 rows
// Each byte is one column (LSB = top pixel, MSB unused)
#define PIXEL_FONT_CHARS 59

// Pre-generated 5x7 font bitmap for digits 0-9 (chars 48-57)
// Each digit: 5 bytes (one per column), LSB top
const uint8_t pixel_font_digits[10][5] PROGMEM = {
    {0x7C,0x82,0x82,0x82,0x7C}, // 0
    {0x00,0x42,0xFE,0x02,0x00}, // 1
    {0x46,0x8A,0x92,0xA2,0x42}, // 2
    {0x44,0x82,0x92,0x92,0x6C}, // 3
    {0x30,0x28,0x24,0xFE,0x20}, // 4
    {0xF2,0x92,0x92,0x92,0x8C}, // 5
    {0x7C,0x92,0x92,0x92,0x4C}, // 6
    {0x80,0x8E,0x90,0xA0,0xC0}, // 7
    {0x6C,0x92,0x92,0x92,0x6C}, // 8
    {0x64,0x92,0x92,0x92,0x7C}, // 9
};

// Draw a single 5x7 character at (x, y) using a custom color
// Returns the x offset for the next character
inline void drawPixelChar(TFT_eSPI& tft, char c, int16_t x, int16_t y, uint16_t color) {
    if (c >= '0' && c <= '9') {
        const uint8_t* glyph = pixel_font_digits[c - '0'];
        for (int col = 0; col < 5; col++) {
            uint8_t bits = glyph[col];
            for (int row = 0; row < 7; row++) {
                if (bits & (1 << row)) {
                    tft.drawPixel(x + col, y + row, color);
                }
            }
        }
    }
}
