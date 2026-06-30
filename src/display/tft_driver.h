#pragma once
#include <TFT_eSPI.h>
#include <cstdint>

#define TFT_WIDTH  320
#define TFT_HEIGHT 240

class TFTDriver {
public:
    bool begin();
    void fillScreen(uint16_t color);
    void setBrightness(uint8_t level);
    TFT_eSPI& tft() { return _tft; }

    static TFTDriver& instance();

private:
    TFTDriver() = default;
    TFT_eSPI _tft;
    uint8_t _brightness = 255;
};
