#include "achievement.h"
#include <cstring>

AchievementSystem::AchievementSystem()
    : _unlocked(0)
    , _consecutiveDays(0)
{
}

void AchievementSystem::begin() {
    _unlocked = 0;
    _consecutiveDays = 0;
}

void AchievementSystem::checkConsecutiveFeed(uint8_t consecutiveDays) {
    _consecutiveDays = consecutiveDays;
    if (consecutiveDays >= 3) _unlocked |= (1UL << ACH_CONSECUTIVE_FEED_3);
    if (consecutiveDays >= 7) _unlocked |= (1UL << ACH_CONSECUTIVE_FEED_7);
    if (consecutiveDays >= 14) _unlocked |= (1UL << ACH_CONSECUTIVE_FEED_14);
    if (consecutiveDays >= 30) _unlocked |= (1UL << ACH_CONSECUTIVE_FEED_30);
}

void AchievementSystem::checkEvolution(uint8_t newStage) {
    if (newStage >= 1) _unlocked |= (1UL << ACH_EVOLUTION_BABY);
    if (newStage >= 2) _unlocked |= (1UL << ACH_EVOLUTION_TEEN);
    if (newStage >= 3) _unlocked |= (1UL << ACH_EVOLUTION_ADULT);
    if (newStage >= 4) _unlocked |= (1UL << ACH_EVOLUTION_FINAL);
}

void AchievementSystem::checkBondLevel(uint8_t bondValue) {
    if (bondValue >= 50) _unlocked |= (1 << ACH_BOND_50);
    if (bondValue >= 80) _unlocked |= (1 << ACH_BOND_80);
    if (bondValue >= 100) _unlocked |= (1 << ACH_BOND_100);
}

void AchievementSystem::checkCareMaster(uint8_t hunger, uint8_t happiness, uint8_t energy, uint8_t bond) {
    if (hunger >= 80 && happiness >= 80 && energy >= 80 && bond >= 80) {
        _unlocked |= (1 << ACH_CARE_MASTER);
    }
}

bool AchievementSystem::isUnlocked(AchievementType type) const {
    if (type >= ACH_COUNT) return false;
    return (_unlocked & (1 << type)) != 0;
}

uint8_t AchievementSystem::getUnlockedCount() const {
    uint8_t count = 0;
    for (uint8_t i = 0; i < ACH_COUNT; i++) {
        if (_unlocked & (1 << i)) count++;
    }
    return count;
}

const char* AchievementSystem::getTitle(AchievementType type) const {
    if (type >= ACH_COUNT) return "Unknown";
    return ACHIEVEMENT_CONFIGS[type].title;
}

uint16_t AchievementSystem::getReward(AchievementType type) const {
    if (type >= ACH_COUNT) return 0;
    return ACHIEVEMENT_CONFIGS[type].rewardPoints;
}

void AchievementSystem::serialize(uint8_t* buffer) const {
    memcpy(buffer, &_unlocked, 4);
    buffer[4] = _consecutiveDays;
}

void AchievementSystem::deserialize(const uint8_t* buffer) {
    memcpy(&_unlocked, buffer, 4);
    _consecutiveDays = buffer[4];
}
