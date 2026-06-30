#pragma once
#include "pet_data.h"
#include <cstdint>

#define HUNGER_MS  28800000UL  // 8 hours
#define SLEEP_START 21
#define SLEEP_END   7

enum PetPrimaryState : uint8_t {
    PSTATE_IDLE     = 0,
    PSTATE_HUNGRY   = 1,
    PSTATE_EATING   = 2,
    PSTATE_HAPPY    = 3,
    PSTATE_SLEEPING = 4,
    PSTATE_EVOLVING = 5,
};

enum IdleSubState : uint8_t {
    ISUB_ROAMING   = 0,
    ISUB_SITTING   = 1,
    ISUB_CURIOSITY = 2,
};

class PetAI {
public:
    PetAI();

    void begin(const PetSpecies* species);
    void update(uint32_t nowMs, uint8_t hour, bool ntpValid);

    // External triggers
    void onFeed();
    void onReward();
    void onEvolve();
    void onWake();

    // State queries
    PetPrimaryState primaryState() const { return _primaryState; }
    IdleSubState idleSubState() const { return _idleSub; }
    PetState& petState() { return _petState; }
    const PetState& petState() const { return _petState; }

    bool isHungry() const;
    bool isSleepTime(uint8_t hour) const;

    // Set game state pointer for hunger/feed tracking
    void setLastFeedTime(uint32_t t) { _petState.lastFeedTime = t; }
    uint32_t lastFeedTime() const { return _petState.lastFeedTime; }

    // Reward queuing (for sleep-time rewards)
    bool hasQueuedReward() const { return _queuedReward; }
    void clearQueuedReward() { _queuedReward = false; }

private:
    PetPrimaryState _primaryState = PSTATE_IDLE;
    IdleSubState _idleSub = ISUB_ROAMING;

    PetState _petState;
    const PetSpecies* _species = nullptr;

    uint32_t _stateEnterTime = 0;
    uint32_t _subStateTimer = 0;
    uint32_t _roamTargetTimer = 0;

    bool _queuedReward = false;

    void transition(PetPrimaryState newState, uint32_t nowMs);
    void pickRoamTarget(uint32_t nowMs);
    void pickNewIdleSubState(uint32_t nowMs);
    void updateMovement();
};
