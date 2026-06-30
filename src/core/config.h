#pragma once
#include <cstdint>

// 按钮引脚配置
#define BTN_A_PIN 32
#define BTN_B_PIN 33
#define BTN_C_PIN 34
#define BTN_DEBOUNCE_MS 50
#define BTN_LONG_PRESS_MS 1000

// 系统配置
struct SystemConfig {
    // 显示配置
    uint16_t screenWidth;
    uint16_t screenHeight;

    // 交互配置
    uint16_t debounceMs;
    uint16_t longPressMs;

    // 经济配置
    uint16_t maxPoints;
    uint8_t maxFoodStack;

    // 疲劳配置
    uint8_t fatigueThreshold;
    uint32_t fatigueCooldownMs;

    // 需求配置
    uint8_t hungerDecayIntervalMin;   // 饥饿衰减间隔（分钟）
    uint8_t happinessDecayIntervalMin; // 快乐衰减间隔（分钟）
    uint8_t energyDecayIntervalMin;    // 能量衰减间隔（分钟）
    uint8_t bondDecayIntervalMin;      // 羁绊衰减间隔（分钟）
};

// 默认配置
static const SystemConfig DEFAULT_CONFIG = {
    320,    // screenWidth
    240,    // screenHeight
    50,     // debounceMs
    1000,   // longPressMs
    9999,   // maxPoints
    99,     // maxFoodStack
    5,      // fatigueThreshold
    600000, // fatigueCooldownMs (10分钟)
    10,     // hungerDecayIntervalMin
    30,     // happinessDecayIntervalMin
    20,     // energyDecayIntervalMin
    120,    // bondDecayIntervalMin (2小时)
};

// 食物配置
struct FoodConfig {
    uint8_t type;
    const char* name;
    uint16_t price;
    uint8_t feedValue;
};

// 默认食物配置
static const FoodConfig FOOD_CONFIGS[5] = {
    {0, "Apple",     3,  10},  // 苹果
    {1, "Drumstick", 5,  20},  // 鸡腿
    {2, "Cake",      8,  30},  // 蛋糕
    {3, "Star",      12, 50},  // 星星
    {4, "Ev.Stone",  20, 0},   // 进化石
};

// 进化阈值
struct EvolutionThreshold {
    uint8_t stage;
    uint16_t feedCount;
    bool requiresStone;
};

// 默认进化阈值
static const EvolutionThreshold EVOLUTION_THRESHOLDS[4] = {
    {1, 100, true},  // Egg → Baby
    {2, 200, true},  // Baby → Teen
    {3, 350, true},  // Teen → Adult
    {4, 500, true},  // Adult → Final
};

// 成就配置
struct AchievementConfig {
    uint8_t type;
    const char* name;
    const char* title;
    uint16_t rewardPoints;
};

// 默认成就配置
static const AchievementConfig ACHIEVEMENT_CONFIGS[] = {
    {0,  "Consecutive Feed 3",   "Little Feeder",       20},
    {1,  "Consecutive Feed 7",   "Loyal Guardian",      50},
    {2,  "Consecutive Feed 14",  "Feeding Master",      100},
    {3,  "Consecutive Feed 30",  "Eternal Guardian",    200},
    {4,  "Evolution Baby",       "New Life",            0},
    {5,  "Evolution Teen",       "Grower",              0},
    {6,  "Evolution Adult",      "Mature",              0},
    {7,  "Evolution Final",      "Complete",            0},
    {8,  "Bond 50",              "Friend",              0},
    {9,  "Bond 80",              "Best Friend",         0},
    {10, "Bond 100",             "Soul Mate",           0},
    {11, "Care Master",          "Care Master",         100},
};

// 成就数量
static const uint8_t ACHIEVEMENT_COUNT = sizeof(ACHIEVEMENT_CONFIGS) / sizeof(AchievementConfig);
