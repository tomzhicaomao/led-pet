#pragma once
#include <cstdint>

// 宠物状态
enum PetState : uint8_t {
    STATE_IDLE = 0,       // 空闲
    STATE_HUNGRY = 1,     // 饥饿
    STATE_HAPPY = 2,      // 快乐
    STATE_EATING = 3,     // 进食
    STATE_PLAYING = 4,    // 玩耍
    STATE_SLEEPING = 5,   // 睡眠
    STATE_DISTRESSED = 6, // 痛苦
    STATE_CURIOUS = 7,    // 好奇
    STATE_EVOLVING = 8,   // 进化
    STATE_BRANCHING = 9,  // 分支
    STATE_COUNT = 10
};

// 状态机类
class PetStateMachine {
public:
    PetStateMachine();

    // 状态转换
    void transitionTo(PetState newState, uint32_t nowMs = 0);

    // 状态查询
    PetState currentState() const { return _current; }
    PetState previousState() const { return _previous; }
    uint32_t stateEnterTime() const { return _enterTime; }

    // 状态持续时间
    uint32_t stateDuration(uint32_t nowMs) const;

    // 获取状态名称
    const char* stateName() const;
    const char* stateName(PetState state) const;

private:
    PetState _current;
    PetState _previous;
    uint32_t _enterTime;
};
