#pragma once
#include "../core/need.h"
#include "../core/personality.h"
#include "../core/clock.h"
#include "../core/config.h"
#include "pet_state.h"
#include "pet_expression.h"

class Pet {
public:
    Pet();

    // 初始化
    void begin();

    // 更新宠物状态
    void update(uint32_t nowMs);

    // 交互接口
    void feed();      // 喂食
    void pet();       // 抚摸
    void play();      // 玩耍
    void rest();      // 休息

    // 状态查询
    PetState getState() const { return _state.currentState(); }
    uint8_t getNeedValue(NeedType type) const;
    bool isNeedCritical(NeedType type) const;

    // 表情系统
    ExpressionType getCurrentExpression() const { return _expression.current(); }

    // 疲劳系统
    bool isFatigued() const { return _fatigued; }
    uint8_t getConsecutiveInteractions() const { return _consecutiveInteractions; }

    // 获取配置
    const SystemConfig& getConfig() const { return _config; }

    // 序列化
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    NeedSystem _needs;
    Personality _personality;
    PetStateMachine _state;
    ExpressionSystem _expression;
    SystemConfig _config;

    // 疲劳系统
    uint8_t _consecutiveInteractions;
    uint32_t _lastInteractionTime;
    bool _fatigued;

    // 内部方法
    void updateState();
    void updateExpression();
    void checkFatigue();
    void handleInteraction();
};
