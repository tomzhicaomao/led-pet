#pragma once
#include <cstdint>
#include "../core/config.h"

class Economy {
public:
    Economy();

    // 初始化
    void begin();

    // 积分操作
    bool addPoints(uint16_t points, const char* pin);
    bool spendPoints(uint16_t points);
    uint16_t getPoints() const { return _points; }

    // 商店操作
    bool buyFood(uint8_t foodType);
    uint8_t getFoodCount(uint8_t foodType) const;
    bool useFood(uint8_t foodType);

    // 进化石操作
    bool buyEvolutionStone();
    bool useEvolutionStone();

    // 序列化
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    uint16_t _points;
    uint8_t _foodBag[5];
    uint8_t _evolutionStones;
    char _parentPIN[5];
};
