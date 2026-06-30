#pragma once
#include <cstdint>

// Need 类型
enum NeedType : uint8_t {
    NEED_HUNGER = 0,    // 饥饿
    NEED_HAPPINESS = 1, // 快乐
    NEED_ENERGY = 2,    // 能量
    NEED_BOND = 3,      // 羁绊
    NEED_COUNT = 4
};

// 序列化大小常量
static const uint8_t NEED_SERIAL_SIZE = 9;  // Need 序列化大小
static const uint8_t NEED_SYSTEM_SERIAL_SIZE = NEED_COUNT * NEED_SERIAL_SIZE;  // NeedSystem 序列化大小

// Need 配置
struct NeedConfig {
    uint8_t initialValue;      // 初始值 (0-100)
    uint8_t decayRate;         // 衰减率 (每分钟)
    uint8_t criticalThreshold; // 危急阈值
    uint8_t recoveryRate;      // 恢复率
};

// Need 类
class Need {
public:
    Need(NeedType type, const NeedConfig& config);
    Need() = default;

    // 更新需求值
    void update(uint32_t nowMs);

    // 恢复需求
    void recover(uint8_t amount);

    // 消耗需求
    void drain(uint8_t amount);

    // 状态查询
    uint8_t value() const { return _value; }
    bool isCritical() const { return _value <= _criticalThreshold; }
    bool isSatisfied() const { return _value >= 80; }
    NeedType type() const { return _type; }

    // 序列化
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    NeedType _type = NEED_HUNGER;
    uint8_t _value = 100;
    uint8_t _decayRate = 1;
    uint8_t _criticalThreshold = 20;
    uint8_t _recoveryRate = 10;
    uint32_t _lastUpdateTime = 0;
};

// NeedSystem 类
class NeedSystem {
public:
    NeedSystem();

    // 初始化
    void begin();

    // 更新所有需求
    void update(uint32_t nowMs);

    // 获取单个需求
    Need& getNeed(NeedType type);
    const Need& getNeed(NeedType type) const;

    // 整体健康度
    uint8_t getOverallHealth() const;

    // 序列化
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    Need _needs[NEED_COUNT];
};
