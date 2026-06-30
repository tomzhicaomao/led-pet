#include "economy.h"
#include <cstring>

Economy::Economy()
    : _points(0)
    , _evolutionStones(0)
{
    strcpy(_parentPIN, "0000");
    memset(_foodBag, 0, sizeof(_foodBag));
}

void Economy::begin() {
    _points = 100;
    _evolutionStones = 0;
    memset(_foodBag, 0, sizeof(_foodBag));
}

bool Economy::addPoints(uint16_t points, const char* pin) {
    if (strcmp(pin, _parentPIN) != 0) return false;
    uint32_t newPoints = _points + points;
    _points = (newPoints > 9999) ? 9999 : static_cast<uint16_t>(newPoints);
    return true;
}

bool Economy::spendPoints(uint16_t points) {
    if (_points < points) return false;
    _points -= points;
    return true;
}

bool Economy::buyFood(uint8_t foodType) {
    if (foodType >= 5) return false;
    uint16_t price = FOOD_CONFIGS[foodType].price;
    if (_points < price || _foodBag[foodType] >= 99) return false;
    _points -= price;
    _foodBag[foodType]++;
    return true;
}

uint8_t Economy::getFoodCount(uint8_t foodType) const {
    if (foodType >= 5) return 0;
    return _foodBag[foodType];
}

bool Economy::useFood(uint8_t foodType) {
    if (foodType >= 5 || _foodBag[foodType] == 0) return false;
    _foodBag[foodType]--;
    return true;
}

bool Economy::buyEvolutionStone() {
    if (_points < 20 || _evolutionStones >= 99) return false;
    _points -= 20;
    _evolutionStones++;
    return true;
}

bool Economy::useEvolutionStone() {
    if (_evolutionStones == 0) return false;
    _evolutionStones--;
    return true;
}

void Economy::serialize(uint8_t* buffer) const {
    memcpy(buffer, &_points, 2);
    memcpy(buffer + 2, _foodBag, 5);
    buffer[7] = _evolutionStones;
    memcpy(buffer + 8, _parentPIN, 5);
}

void Economy::deserialize(const uint8_t* buffer) {
    memcpy(&_points, buffer, 2);
    memcpy(_foodBag, buffer + 2, 5);
    _evolutionStones = buffer[7];
    memcpy(_parentPIN, buffer + 8, 5);
}
