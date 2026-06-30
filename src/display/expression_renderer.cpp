#include "expression_renderer.h"

ExpressionRenderer::ExpressionRenderer()
    : _currentType(EXPR_NONE)
{
}

void ExpressionRenderer::update(ExpressionType type, uint8_t intensity) {
    if (type != _currentType) {
        _currentType = type;
        _bubble.hide();
    }

    switch (type) {
        case EXPR_HUNGRY:
            _bubble.show(BUBBLE_FOOD, intensity);
            break;
        case EXPR_HAPPY:
            _bubble.show(BUBBLE_HEART, intensity);
            break;
        case EXPR_TIRED:
        case EXPR_FATIGUED:
            _bubble.show(BUBBLE_ZZZ, intensity);
            break;
        case EXPR_LONELY:
            _bubble.show(BUBBLE_THINK, intensity);
            break;
        default:
            _bubble.hide();
            break;
    }
}

void ExpressionRenderer::render(int16_t x, int16_t y) {
    _bubble.render(x - 20, y - 30);

    switch (_currentType) {
        case EXPR_HUNGRY:    renderHungry(x, y); break;
        case EXPR_HAPPY:     renderHappy(x, y); break;
        case EXPR_TIRED:     renderTired(x, y); break;
        case EXPR_LONELY:    renderLonely(x, y); break;
        case EXPR_EXCITED:   renderExcited(x, y); break;
        case EXPR_SAD:       renderSad(x, y); break;
        case EXPR_FATIGUED:  renderFatigued(x, y); break;
        default: break;
    }
}

void ExpressionRenderer::clear() {
    _currentType = EXPR_NONE;
    _bubble.hide();
}

void ExpressionRenderer::renderHungry(int16_t x, int16_t y) {}
void ExpressionRenderer::renderHappy(int16_t x, int16_t y) {}
void ExpressionRenderer::renderTired(int16_t x, int16_t y) {}
void ExpressionRenderer::renderLonely(int16_t x, int16_t y) {}
void ExpressionRenderer::renderExcited(int16_t x, int16_t y) {}
void ExpressionRenderer::renderSad(int16_t x, int16_t y) {}
void ExpressionRenderer::renderFatigued(int16_t x, int16_t y) {}
