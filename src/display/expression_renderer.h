#pragma once
#include "../pet/pet_expression.h"
#include "bubble.h"

class ExpressionRenderer {
public:
    ExpressionRenderer();

    // 更新表情
    void update(ExpressionType type, uint8_t intensity);

    // 渲染
    void render(int16_t x, int16_t y);

    // 清除
    void clear();

    // 获取当前气泡
    const Bubble& bubble() const { return _bubble; }

private:
    Bubble _bubble;
    ExpressionType _currentType;

    // 表情精灵
    void renderHungry(int16_t x, int16_t y);
    void renderHappy(int16_t x, int16_t y);
    void renderTired(int16_t x, int16_t y);
    void renderLonely(int16_t x, int16_t y);
    void renderExcited(int16_t x, int16_t y);
    void renderSad(int16_t x, int16_t y);
    void renderFatigued(int16_t x, int16_t y);
};
