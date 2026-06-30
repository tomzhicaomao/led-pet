#include "game_state.h"
#include <Preferences.h>
#include <Arduino.h>
#include <cstring>

static const char* NVS_NS = "pet";
static const char* NVS_KEY_PREFIX = "save_";

GameEngine::GameEngine() { initFoods(); }

void GameEngine::initFoods() {
    _foods[FOOD_APPLE]     = {FOOD_APPLE,     "Apple",     3,  1, false};
    _foods[FOOD_DRUMSTICK] = {FOOD_DRUMSTICK, "Drumstick", 5,  2, false};
    _foods[FOOD_CAKE]      = {FOOD_CAKE,      "Cake",      8,  3, false};
    _foods[FOOD_STAR]      = {FOOD_STAR,      "Star",      12, 5, false};
    _foods[FOOD_STONE]     = {FOOD_STONE,     "Ev.Stone",  20, 0, true};
}

void GameEngine::init() {
    Preferences prefs;
    char key[16];
    snprintf(key, sizeof(key), "%s%d", NVS_KEY_PREFIX, 0);
    if (!prefs.begin(NVS_NS, true) || prefs.getBytesLength(key) == 0) {
        prefs.end();
        reset();
        return;
    }
    prefs.getBytes(key, &_state, sizeof(_state));
    prefs.end();
    if (_state.initialized) return;
    reset();
}

void GameEngine::save() {
    Preferences prefs;
    char key[16];
    snprintf(key, sizeof(key), "%s%d", NVS_KEY_PREFIX, _state.currentPet);
    prefs.begin(NVS_NS, false);
    prefs.putBytes(key, &_state, sizeof(_state));
    prefs.end();
}

void GameEngine::reset() {
    _state = GameState();
    strncpy(_state.parentPIN, "0000", 4);
    _state.parentPIN[4] = '\0';
    _state.initialized = true;
    _state.lastFeedTime = UINT32_MAX;
    save();
}

GameState& GameEngine::getState() { return _state; }
const GameState& GameEngine::getState() const { return _state; }

uint16_t GameEngine::thresholdForStage(uint8_t stage) const {
    const uint16_t t[] = {3, 8, 15, 25, 0};
    return stage >= 4 ? 65535 : t[stage];
}

bool GameEngine::canEvolve() const {
    return _state.currentStage < 4 &&
           _state.feedCount >= thresholdForStage(_state.currentStage);
}

bool GameEngine::isHungry() const {
    if (_state.lastFeedTime == UINT32_MAX) return true;  // never fed
    uint32_t now = millis();
    if (_state.lastFeedTime > now) return false;  // crossed reboot boundary
    return (now - _state.lastFeedTime) > 28800000UL;
}

bool GameEngine::addPoints(uint32_t points, const char* pin) {
    if (strcmp(pin, _state.parentPIN) != 0) return false;
    _state.points += points;
    save();
    return true;
}

bool GameEngine::buyFood(uint8_t foodType) {
    if (foodType >= FOOD_COUNT) return false;
    if (_state.points < _foods[foodType].price) return false;
    if (_state.foodBag[foodType] >= MAX_FOOD_STACK) return false;
    _state.points -= _foods[foodType].price;
    _state.foodBag[foodType]++;
    save();
    return true;
}

bool GameEngine::feed(uint8_t foodType, bool& evolved, uint8_t& newStage) {
    evolved = false;
    newStage = _state.currentStage;
    if (foodType >= FOOD_COUNT || _state.foodBag[foodType] == 0) return false;
    _state.foodBag[foodType]--;
    _state.lastFeedTime = millis();

    if (_foods[foodType].isEvolutionStone) {
        if (canEvolve()) {
            _state.currentStage++;
            _state.feedCount = 0;
            evolved = true;
            newStage = _state.currentStage;
        } else {
            _state.foodBag[foodType]++;  // return stone
            return false;
        }
    } else {
        _state.feedCount += _foods[foodType].feedValue;
    }
    save();
    return true;
}

const FoodConfig* GameEngine::getFoodConfig() const { return _foods; }

void GameEngine::addRewardEntry(uint16_t points, const char* reason) {
    _history[_historyIdx] = {millis(), points, ""};
    strncpy(_history[_historyIdx].reason, reason, 15);
    _history[_historyIdx].reason[15] = '\0';
    _historyIdx = (_historyIdx + 1) % HISTORY_SIZE;
    if (_historyCount < HISTORY_SIZE) _historyCount++;
}

uint8_t GameEngine::getRewardHistory(RewardEntry* out, uint8_t maxCount) const {
    uint8_t n = min(_historyCount, maxCount);
    for (uint8_t i = 0; i < n; i++) {
        uint8_t idx = (_historyIdx + HISTORY_SIZE - n + i) % HISTORY_SIZE;
        out[i] = _history[idx];
    }
    return n;
}

const char* GameEngine::getStatusJson() {
    snprintf(_jsonBuffer, sizeof(_jsonBuffer),
        "{\"name\":\"Dragon\",\"stage\":%d,\"state\":\"%s\",\"points\":%lu,\"feedCount\":%lu,"
        "\"bagCounts\":[%d,%d,%d,%d,%d]}",
        _state.currentStage, isHungry() ? "hungry" : "happy",
        _state.points, _state.feedCount,
        _state.foodBag[0], _state.foodBag[1], _state.foodBag[2],
        _state.foodBag[3], _state.foodBag[4]);
    return _jsonBuffer;
}

const char* GameEngine::getBagJson() {
    snprintf(_jsonBuffer, sizeof(_jsonBuffer),
        "{\"items\":[%d,%d,%d,%d,%d]}",
        _state.foodBag[0], _state.foodBag[1],
        _state.foodBag[2], _state.foodBag[3], _state.foodBag[4]);
    return _jsonBuffer;
}

const char* GameEngine::getShopJson() {
    snprintf(_jsonBuffer, sizeof(_jsonBuffer),
        "[{\"name\":\"%s\",\"price\":%d},{\"name\":\"%s\",\"price\":%d},"
        "{\"name\":\"%s\",\"price\":%d},{\"name\":\"%s\",\"price\":%d},"
        "{\"name\":\"%s\",\"price\":%d}]",
        _foods[0].name, _foods[0].price,
        _foods[1].name, _foods[1].price,
        _foods[2].name, _foods[2].price,
        _foods[3].name, _foods[3].price,
        _foods[4].name, _foods[4].price);
    return _jsonBuffer;
}

const char* GameEngine::getStagesJson() {
    const char* names[] = {"Egg","Baby","Teen","Adult","Final"};
    const uint16_t thresh[] = {3,8,15,25,0};
    snprintf(_jsonBuffer, sizeof(_jsonBuffer),
        "[{\"name\":\"%s\",\"threshold\":%d},{\"name\":\"%s\",\"threshold\":%d},"
        "{\"name\":\"%s\",\"threshold\":%d},{\"name\":\"%s\",\"threshold\":%d},"
        "{\"name\":\"%s\",\"threshold\":%d}]",
        names[0], thresh[0], names[1], thresh[1],
        names[2], thresh[2], names[3], thresh[3],
        names[4], thresh[4]);
    return _jsonBuffer;
}
