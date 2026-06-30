#pragma once
#include <cstdint>
#include "../core/config.h"
#include "../core/personality.h"
#include "../pet/pet_expression.h"

// 进化路径
enum EvolutionPath : uint8_t {
    PATH_KIND = 0,     // 高羁绊 + 均衡护理
    PATH_PLAYFUL = 1,  // 高快乐 + 多玩耍
    PATH_WILD = 2,     // 低羁绊 + 高能量
    PATH_WISE = 3,     // 规律喂食 + 稳定护理
    PATH_COUNT = 4
};

// 进化系统
class Evolution {
public:
    Evolution();

    void begin();
    void update(uint8_t feedCount, uint8_t bond, uint8_t happiness, uint8_t energy);
    bool canEvolve() const;
    bool evolve(bool hasStone);
    uint8_t getStage() const { return _stage; }
    EvolutionPath getPath() const { return _path; }
    const char* getPathName() const;
    const char* getPathName(EvolutionPath path) const;
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    uint8_t _stage;
    uint8_t _feedCount;
    uint16_t _totalFeedCount;
    EvolutionPath _path;
    bool _pathDetermined;

    void calculatePath(uint8_t bond, uint8_t happiness, uint8_t energy);
};
