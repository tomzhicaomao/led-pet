#include "tft_driver.h"
#include <Arduino.h>

TFTDriver& TFTDriver::instance() {
    static TFTDriver inst;
    return inst;
}

bool TFTDriver::begin() {
    _tft.init();
    _tft.setRotation(1);
    _tft.fillScreen(TFT_BLACK);
    _tft.setTextColor(TFT_WHITE, TFT_BLACK);
    Serial.println("TFT initialized: 320x240 ILI9341");
    return true;
}

void TFTDriver::fillScreen(uint16_t color) {
    _tft.fillScreen(color);
}

void TFTDriver::setBrightness(uint8_t level) {
    _brightness = level;
    // ILI9341 brightness is controlled via PWM on backlight pin if connected
    // Software brightness: adjust colors via alpha
}
