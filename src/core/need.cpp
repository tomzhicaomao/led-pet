#include "need.h"
#include <algorithm>
#include <cstring>

// 需求衰减配置 (每分钟衰减率)
// 饥饿: 1点/10分钟 = 0.1点/分钟
// 快乐: 1点/30分钟 ≈ 0.033点/分钟
// 能量: 1点/20分钟 = 0.05点/分钟
// 羁绊: -1点/2小时 ≈ 0.008点/分钟
static const NeedConfig DEFAULT_CONFIGS[NEED_COUNT] = {
    {100, 1, 20, 10},  // 饥饿：初始100, 衰减率1(每10分钟), 危急20, 恢复10
    {100, 1, 30, 15},  // 快乐：初始100, 衰减率1(每30分钟), 危急30, 恢复15
    {100, 1, 25, 20},  // 能量：初始100, 衰减率1(每20分钟), 危急25, 恢复20
    {50,  1, 10, 5},   // 羁绊：初始50, 衰减率1(每2小时), 危急10, 恢复5
};

// Need 类实现
Need::Need(NeedType type, const NeedConfig& config)
    : _type(type)
    , _value(config.initialValue)
    , _decayRate(config.decayRate)
    , _criticalThreshold(config.criticalThreshold)
    , _recoveryRate(config.recoveryRate)
    , _lastUpdateTime(0)
{
}

void Need::update(uint32_t nowMs) {
    if (_lastUpdateTime == 0) {
        _lastUpdateTime = nowMs;
        return;
    }

    uint32_t elapsed = nowMs - _lastUpdateTime;
    uint32_t decayInterval = getDecayInterval();

    // 计算衰减次数
    uint32_t decayCount = elapsed / decayInterval;
    if (decayCount > 0) {
        uint8_t decayAmount = decayCount * _decayRate;
        if (decayAmount > _value) {
            _value = 0;
        } else {
            _value -= decayAmount;
        }
        _lastUpdateTime += decayCount * decayInterval;
    }
}

uint32_t Need::getDecayInterval() const {
    switch (_type) {
        case NEED_HUNGER:   return 600000;  // 10 分钟
        case NEED_HAPPINESS: return 1800000; // 30 分钟
        case NEED_ENERGY:   return 1200000;  // 20 分钟
        case NEED_BOND:     return 7200000;  // 2 小时
        default:            return 600000;
    }
}

void Need::recover(uint8_t amount) {
    uint16_t newValue = _value + amount;
    _value = (newValue > 100) ? 100 : static_cast<uint8_t>(newValue);
}

void Need::drain(uint8_t amount) {
    if (amount > _value) {
        _value = 0;
    } else {
        _value -= amount;
    }
}

void Need::serialize(uint8_t* buffer) const {
    buffer[0] = static_cast<uint8_t>(_type);
    buffer[1] = _value;
    buffer[2] = _decayRate;
    buffer[3] = _criticalThreshold;
    buffer[4] = _recoveryRate;
    memcpy(buffer + 5, &_lastUpdateTime, 4);
}

void Need::deserialize(const uint8_t* buffer) {
    _type = static_cast<NeedType>(buffer[0]);
    _value = buffer[1];
    _decayRate = buffer[2];
    _criticalThreshold = buffer[3];
    _recoveryRate = buffer[4];
    memcpy(&_lastUpdateTime, buffer + 5, 4);
}

// NeedSystem 类实现
NeedSystem::NeedSystem()
    : _needs{
        Need(NEED_HUNGER, DEFAULT_CONFIGS[0]),
        Need(NEED_HAPPINESS, DEFAULT_CONFIGS[1]),
        Need(NEED_ENERGY, DEFAULT_CONFIGS[2]),
        Need(NEED_BOND, DEFAULT_CONFIGS[3])
    }
{
}

void NeedSystem::begin() {
    // 初始化所有需求
    for (uint8_t i = 0; i < NEED_COUNT; i++) {
        _needs[i] = Need(static_cast<NeedType>(i), DEFAULT_CONFIGS[i]);
    }
}

void NeedSystem::update(uint32_t nowMs) {
    for (uint8_t i = 0; i < NEED_COUNT; i++) {
        _needs[i].update(nowMs);
    }
}

Need& NeedSystem::getNeed(NeedType type) {
    return _needs[static_cast<uint8_t>(type)];
}

const Need& NeedSystem::getNeed(NeedType type) const {
    return _needs[static_cast<uint8_t>(type)];
}

uint8_t NeedSystem::getOverallHealth() const {
    uint16_t total = 0;
    for (uint8_t i = 0; i < NEED_COUNT; i++) {
        total += _needs[i].value();
    }
    return total / NEED_COUNT;
}

void NeedSystem::serialize(uint8_t* buffer) const {
    for (uint8_t i = 0; i < NEED_COUNT; i++) {
        _needs[i].serialize(buffer + i * 9);
    }
}

void NeedSystem::deserialize(const uint8_t* buffer) {
    for (uint8_t i = 0; i < NEED_COUNT; i++) {
        _needs[i].deserialize(buffer + i * 9);
    }
}
