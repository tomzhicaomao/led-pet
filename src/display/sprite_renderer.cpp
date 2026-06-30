#include "sprite_renderer.h"

SpriteRenderer::SpriteRenderer() : _tft(TFTDriver::instance()), _bgColor(0x4D2A) {}

void SpriteRenderer::begin() {
    _tft.fillScreen(_bgColor);
}

void SpriteRenderer::drawFrame() {
    // Layer 0: background
    _tft.fillScreen(_bgColor);

    // Layer 1: scene objects (trees, house, ground)
    for (uint8_t i = 0; i < _sceneObjCount; i++) {
        blitSprite(_sceneObjs[i].sprite, _sceneObjs[i].x, _sceneObjs[i].y);
    }

    // Layer 2: pet sprite
    if (_petSprite && _petSprite->data) {
        blitSprite(*_petSprite, _petX, _petY);
    }

    // Layer 3: celebration particles
    if (_celebration) {
        // Sparkle effect: scatter white pixels
        for (uint8_t i = 0; i < 20; i++) {
            int16_t rx = random(320);
            int16_t ry = random(240);
            _tft.tft().drawPixel(rx, ry, TFT_WHITE);
        }
    }

    // Layer 4: status bar
    drawStatusBar();
}

void SpriteRenderer::blitSprite(const Sprite& sprite, int16_t x, int16_t y) {
    if (!sprite.data) return;
    int16_t x0 = x;
    int16_t y0 = y;
    int16_t x1 = x + sprite.w - 1;
    int16_t y1 = y + sprite.h - 1;

    // Clip to screen bounds
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= TFT_WIDTH)  x1 = TFT_WIDTH - 1;
    if (y1 >= TFT_HEIGHT) y1 = TFT_HEIGHT - 1;
    if (x0 > x1 || y0 > y1) return;

    _tft.tft().setAddrWindow(x0, y0, x1 - x0 + 1, y1 - y0 + 1);
    _tft.tft().startWrite();

    for (int16_t row = y0; row <= y1; row++) {
        for (int16_t col = x0; col <= x1; col++) {
            uint16_t srcCol = col - x;
            uint16_t srcRow = row - y;
            uint16_t color = sprite.data[srcRow * sprite.w + srcCol];
            _tft.tft().pushColor(color);
        }
    }
    _tft.tft().endWrite();
}

void SpriteRenderer::drawStatusBar() {
    _tft.tft().fillRect(0, 0, TFT_WIDTH, 18, 0x2104);
    if (_statusText) {
        _tft.tft().setTextColor(TFT_WHITE, 0x2104);
        _tft.tft().drawString(_statusText, 4, 2, 2);
    }
}

void SpriteRenderer::setPetSprite(const Sprite* sprite, int16_t x, int16_t y) {
    _petSprite = sprite;
    _petX = x;
    _petY = y;
}

void SpriteRenderer::setStatusText(const char* text) {
    _statusText = text;
}

void SpriteRenderer::addSceneObject(const Sprite& sprite, int16_t x, int16_t y) {
    if (_sceneObjCount < MAX_SCENE_OBJS) {
        _sceneObjs[_sceneObjCount++] = {sprite, x, y};
    }
}

void SpriteRenderer::clearSceneObjects() {
    _sceneObjCount = 0;
}

void SpriteRenderer::showCelebration(bool active) {
    _celebration = active;
}
