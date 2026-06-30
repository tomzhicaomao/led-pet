#pragma once
#include <cstdint>

// 性格特质
enum Trait : uint8_t {
    TRAIT_PLAYFUL = 0,  // 爱玩
    TRAIT_SHY = 1,      // 害羞
    TRAIT_CURIOUS = 2,  // 好奇
    TRAIT_LAZY = 3,     // 懒惰
    TRAIT_BRAVE = 4,    // 勇敢
    TRAIT_GENTLE = 5,   // 温柔
    TRAIT_COUNT = 6
};

// 互动类型
enum InteractionType : uint8_t {
    INTERACT_FEED = 0,   // 喂食
    INTERACT_PET = 1,    // 抚摸
    INTERACT_PLAY = 2,   // 玩耍
    INTERACT_REST = 3,   // 休息
    INTERACT_COUNT = 4
};

// 性格配置
struct PersonalityConfig {
    uint8_t initialTraitValue; // 初始特质值
    uint8_t minTraitValue;     // 最小特质值
    uint8_t maxTraitValue;     // 最大特质值
    int8_t traitImpact[INTERACT_COUNT][TRAIT_COUNT]; // 互动对特质的影响
};

class Personality {
public:
    Personality();
    explicit Personality(const PersonalityConfig& config);

    // 获取特质强度
    uint8_t getTrait(Trait trait) const;

    // 更新特质
    void updateTrait(Trait trait, int8_t delta);

    // 根据互动类型发展性格
    void developFromInteraction(InteractionType type, uint8_t intensity = 1);

    // 获取主导特质
    Trait getDominantTrait() const;

    // 获取特质描述
    const char* getTraitName(Trait trait) const;

    // 序列化
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    uint8_t _traits[TRAIT_COUNT];
    uint8_t _initialValue;
    uint8_t _minValue;
    uint8_t _maxValue;

    void clampTrait(uint8_t& value);
};
