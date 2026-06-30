#pragma once
#include <cstdint>
#include "../core/config.h"

// 成就类型
enum AchievementType : uint8_t {
    ACH_CONSECUTIVE_FEED_3 = 0,
    ACH_CONSECUTIVE_FEED_7 = 1,
    ACH_CONSECUTIVE_FEED_14 = 2,
    ACH_CONSECUTIVE_FEED_30 = 3,
    ACH_EVOLUTION_BABY = 4,
    ACH_EVOLUTION_TEEN = 5,
    ACH_EVOLUTION_ADULT = 6,
    ACH_EVOLUTION_FINAL = 7,
    ACH_BOND_50 = 8,
    ACH_BOND_80 = 9,
    ACH_BOND_100 = 10,
    ACH_CARE_MASTER = 11,
    ACH_COUNT = 12
};

class AchievementSystem {
public:
    AchievementSystem();

    void begin();

    // 检查成就
    void checkConsecutiveFeed(uint8_t consecutiveDays);
    void checkEvolution(uint8_t newStage);
    void checkBondLevel(uint8_t bondValue);
    void checkCareMaster(uint8_t hunger, uint8_t happiness, uint8_t energy, uint8_t bond);

    // 查询
    bool isUnlocked(AchievementType type) const;
    uint8_t getUnlockedCount() const;
    const char* getTitle(AchievementType type) const;
    uint16_t getReward(AchievementType type) const;

    // 序列化
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    uint32_t _unlocked;
    uint8_t _consecutiveDays;
};
