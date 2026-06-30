#pragma once
#include <cstdint>

#define FOOD_COUNT      5
#define MAX_FOOD_STACK  99

enum FoodType : uint8_t {
    FOOD_APPLE     = 0,
    FOOD_DRUMSTICK = 1,
    FOOD_CAKE      = 2,
    FOOD_STAR      = 3,
    FOOD_STONE     = 4,
};

struct FoodConfig {
    FoodType type;
    const char* name;
    uint16_t price;
    uint8_t feedValue;
    bool isEvolutionStone;
};

struct GameState {
    uint32_t points = 0;
    uint8_t currentStage = 0;
    uint32_t feedCount = 0;
    uint8_t foodBag[FOOD_COUNT] = {};
    uint32_t lastFeedTime = UINT32_MAX;
    char parentPIN[5] = "0000";
    uint8_t currentPet = 0;
    bool initialized = false;
};

class GameEngine {
public:
    GameEngine();

    void init();
    GameState& getState();
    const GameState& getState() const;

    bool addPoints(uint32_t points, const char* pin);
    bool buyFood(uint8_t foodType);
    bool feed(uint8_t foodType, bool& evolved, uint8_t& newStage);
    bool canEvolve() const;
    bool isHungry() const;
    void save();
    void reset();

    const FoodConfig* getFoodConfig() const;
    uint8_t getFoodCount() const { return FOOD_COUNT; }

    const char* getStatusJson();
    const char* getBagJson();
    const char* getShopJson();
    const char* getStagesJson();

    // Reward history ring buffer
    struct RewardEntry {
        uint32_t timestamp;
        uint16_t points;
        char reason[16];
    };
    void addRewardEntry(uint16_t points, const char* reason);
    uint8_t getRewardHistory(RewardEntry* out, uint8_t maxCount) const;

private:
    GameState _state;
    FoodConfig _foods[FOOD_COUNT];
    char _jsonBuffer[512];

    static const uint8_t HISTORY_SIZE = 20;
    RewardEntry _history[HISTORY_SIZE];
    uint8_t _historyIdx = 0;
    uint8_t _historyCount = 0;

    void initFoods();
    uint16_t thresholdForStage(uint8_t stage) const;
};
