#include "personality.h"
#include <cstring>

// 默认性格配置
static const PersonalityConfig DEFAULT_CONFIG = {
    50,  // 初始特质值
    0,   // 最小特质值
    100, // 最大特质值
    {
        // PLAYFUL, SHY, CURIOUS, LAZY, BRAVE, GENTLE
        {  0,      0,    0,       0,     0,     5   },  // 喂食 → 温柔+
        {  0,     -5,    0,       0,    -5,    10   },  // 抚摸 → 温柔++, 害羞-, 勇敢-
        { 10,     -5,   10,      -5,     5,     0   },  // 玩耍 → 爱玩++, 好奇++, 懒惰-
        { -5,      5,   -5,      10,    -5,     0   },  // 休息 → 懒惰++, 害羞+, 爱玩-
    }
};

// 特质名称
static const char* TRAIT_NAMES[TRAIT_COUNT] = {
    "Playful", "Shy", "Curious", "Lazy", "Brave", "Gentle"
};

Personality::Personality()
    : Personality(DEFAULT_CONFIG)
{
}

Personality::Personality(const PersonalityConfig& config)
    : _initialValue(config.initialTraitValue)
    , _minValue(config.minTraitValue)
    , _maxValue(config.maxTraitValue)
{
    for (uint8_t i = 0; i < TRAIT_COUNT; i++) {
        _traits[i] = config.initialTraitValue;
    }
}

uint8_t Personality::getTrait(Trait trait) const {
    if (trait >= TRAIT_COUNT) return 0;
    return _traits[static_cast<uint8_t>(trait)];
}

void Personality::updateTrait(Trait trait, int8_t delta) {
    if (trait >= TRAIT_COUNT) return;

    uint8_t idx = static_cast<uint8_t>(trait);
    int16_t newValue = _traits[idx] + delta;

    if (newValue < _minValue) {
        _traits[idx] = _minValue;
    } else if (newValue > _maxValue) {
        _traits[idx] = _maxValue;
    } else {
        _traits[idx] = static_cast<uint8_t>(newValue);
    }
}

void Personality::developFromInteraction(InteractionType type, uint8_t intensity) {
    if (type >= INTERACT_COUNT) return;

    // 默认性格影响表
    static const int8_t IMPACT[INTERACT_COUNT][TRAIT_COUNT] = {
        // PLAYFUL, SHY, CURIOUS, LAZY, BRAVE, GENTLE
        {  0,      0,    0,       0,     0,     5   },  // 喂食
        {  0,     -5,    0,       0,    -5,    10   },  // 抚摸
        { 10,     -5,   10,      -5,     5,     0   },  // 玩耍
        { -5,      5,   -5,      10,    -5,     0   },  // 休息
    };

    for (uint8_t i = 0; i < TRAIT_COUNT; i++) {
        int8_t impact = IMPACT[type][i] * intensity;
        updateTrait(static_cast<Trait>(i), impact);
    }
}

Trait Personality::getDominantTrait() const {
    uint8_t maxValue = 0;
    Trait dominant = TRAIT_PLAYFUL;

    for (uint8_t i = 0; i < TRAIT_COUNT; i++) {
        if (_traits[i] > maxValue) {
            maxValue = _traits[i];
            dominant = static_cast<Trait>(i);
        }
    }

    return dominant;
}

const char* Personality::getTraitName(Trait trait) const {
    if (trait >= TRAIT_COUNT) return "Unknown";
    return TRAIT_NAMES[static_cast<uint8_t>(trait)];
}

void Personality::serialize(uint8_t* buffer) const {
    for (uint8_t i = 0; i < TRAIT_COUNT; i++) {
        buffer[i] = _traits[i];
    }
}

void Personality::deserialize(const uint8_t* buffer) {
    for (uint8_t i = 0; i < TRAIT_COUNT; i++) {
        _traits[i] = buffer[i];
    }
}
