#pragma once
#include "tft_driver.h"
#include <cstdint>
#include <vector>

struct Sprite {
    const uint16_t* data;
    uint16_t w;
    uint16_t h;
};

struct SceneObject {
    Sprite sprite;
    int16_t x;
    int16_t y;
};

class SpriteRenderer {
public:
    SpriteRenderer();

    void begin();
    void drawFrame();

    void setBackground(uint16_t color) { _bgColor = color; }
    void setPetSprite(const Sprite* sprite, int16_t x, int16_t y);
    void setStatusText(const char* text);
    void addSceneObject(const Sprite& sprite, int16_t x, int16_t y);
    void clearSceneObjects();
    void showCelebration(bool active);

private:
    TFTDriver& _tft;
    uint16_t _bgColor;

    const Sprite* _petSprite = nullptr;
    int16_t _petX = 0;
    int16_t _petY = 0;

    const char* _statusText = nullptr;

    static const uint8_t MAX_SCENE_OBJS = 16;
    SceneObject _sceneObjs[MAX_SCENE_OBJS];
    uint8_t _sceneObjCount = 0;

    bool _celebration = false;

    void blitSprite(const Sprite& sprite, int16_t x, int16_t y);
    void drawStatusBar();
};
