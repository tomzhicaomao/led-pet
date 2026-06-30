#include "bubble.h"

Bubble::Bubble()
    : _type(BUBBLE_FOOD)
    , _visible(false)
    , _intensity(0)
    , _startTime(0)
    , _frameIndex(0)
{
}

void Bubble::show(BubbleType type, uint8_t intensity) {
    _type = type;
    _visible = true;
    _intensity = intensity;
    _startTime = 0;
    _frameIndex = 0;
}

void Bubble::hide() {
    _visible = false;
}

void Bubble::update(uint32_t nowMs) {
    if (!_visible) return;

    if (nowMs - _startTime >= 500) {
        _frameIndex = (_frameIndex + 1) % 4;
        _startTime = nowMs;
    }
}

void Bubble::render(int16_t x, int16_t y) {
    if (!_visible) return;
    // 渲染逻辑将在实际硬件上实现
}
