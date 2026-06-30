#pragma once
#include <cstdint>

// 气泡类型
enum BubbleType : uint8_t {
    BUBBLE_FOOD = 0,     // 食物气泡（饥饿）
    BUBBLE_HEART = 1,    // 爱心气泡（快乐）
    BUBBLE_ZZZ = 2,      // 困倦气泡（能量）
    BUBBLE_THINK = 3,    // 思念气泡（羁绊）
    BUBBLE_COUNT = 4
};

// 气泡类
class Bubble {
public:
    Bubble();

    // 显示气泡
    void show(BubbleType type, uint8_t intensity);

    // 隐藏气泡
    void hide();

    // 更新动画
    void update(uint32_t nowMs);

    // 渲染
    void render(int16_t x, int16_t y);

    // 状态查询
    bool isVisible() const { return _visible; }
    BubbleType type() const { return _type; }
    uint8_t intensity() const { return _intensity; }

private:
    BubbleType _type;
    bool _visible;
    uint8_t _intensity;
    uint32_t _startTime;
    uint8_t _frameIndex;
};
