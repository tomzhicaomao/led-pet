#include "pet_state.h"

// 状态名称
static const char* STATE_NAMES[STATE_COUNT] = {
    "Idle", "Hungry", "Happy", "Eating", "Playing",
    "Sleeping", "Distressed", "Curious", "Evolving", "Branching"
};

PetStateMachine::PetStateMachine()
    : _current(STATE_IDLE)
    , _previous(STATE_IDLE)
    , _enterTime(0)
{
}

void PetStateMachine::transitionTo(PetState newState, uint32_t nowMs) {
    if (newState == _current) return;

    _previous = _current;
    _current = newState;
    _enterTime = nowMs;
}

uint32_t PetStateMachine::stateDuration(uint32_t nowMs) const {
    if (_enterTime == 0) return 0;
    return nowMs - _enterTime;
}

const char* PetStateMachine::stateName() const {
    return stateName(_current);
}

const char* PetStateMachine::stateName(PetState state) const {
    if (state >= STATE_COUNT) return "Unknown";
    return STATE_NAMES[static_cast<uint8_t>(state)];
}
