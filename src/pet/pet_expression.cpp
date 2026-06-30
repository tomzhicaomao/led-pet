#include "pet_expression.h"

// 表情名称
static const char* EXPRESSION_NAMES[EXPR_COUNT] = {
    "None", "Hungry", "Happy", "Tired", "Lonely",
    "Excited", "Sad", "Fatigued"
};

ExpressionSystem::ExpressionSystem()
    : _current(EXPR_NONE)
    , _intensity(0)
    , _startTime(0)
{
}

void ExpressionSystem::update(ExpressionType type, uint8_t intensity) {
    if (type != _current || intensity != _intensity) {
        _current = type;
        _intensity = intensity;
        _startTime = 0; // 重置开始时间
    }
}

uint32_t ExpressionSystem::duration(uint32_t nowMs) const {
    if (_startTime == 0) return 0;
    return nowMs - _startTime;
}

const char* ExpressionSystem::name() const {
    return name(_current);
}

const char* ExpressionSystem::name(ExpressionType type) const {
    if (type >= EXPR_COUNT) return "Unknown";
    return EXPRESSION_NAMES[static_cast<uint8_t>(type)];
}

void ExpressionSystem::serialize(uint8_t* buffer) const {
    buffer[0] = static_cast<uint8_t>(_current);
    buffer[1] = _intensity;
}

void ExpressionSystem::deserialize(const uint8_t* buffer) {
    _current = static_cast<ExpressionType>(buffer[0]);
    _intensity = buffer[1];
}
