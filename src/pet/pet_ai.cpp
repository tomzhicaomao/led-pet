#include "pet_ai.h"
#include <Arduino.h>

PetAI::PetAI() {}

void PetAI::begin(const PetSpecies* species) {
    _species = species;
    _petState.posX = 140;
    _petState.posY = 120;
    _petState.targetX = 140;
    _petState.targetY = 120;
    _petState.anim = ANIM_IDLE;
    _primaryState = PSTATE_IDLE;
    _stateEnterTime = millis();
}

void PetAI::update(uint32_t nowMs, uint8_t hour, bool ntpValid) {
    // Process queued reward first
    if (_queuedReward && _primaryState != PSTATE_SLEEPING) {
        _queuedReward = false;
        transition(PSTATE_HAPPY, nowMs);
    }

    // State-specific logic
    switch (_primaryState) {
    case PSTATE_IDLE:
        // Check hunger
        if (isHungry()) {
            transition(PSTATE_HUNGRY, nowMs);
            break;
        }
        // Check sleep time
        if (ntpValid && isSleepTime(hour)) {
            transition(PSTATE_SLEEPING, nowMs);
            break;
        }
        // Update idle sub-state
        if (nowMs - _subStateTimer >= 30000 + random(30000)) {
            pickNewIdleSubState(nowMs);
        }
        // Handle movement
        updateMovement();
        break;

    case PSTATE_HUNGRY:
        if (ntpValid && isSleepTime(hour)) {
            transition(PSTATE_SLEEPING, nowMs);
            break;
        }
        if (!isHungry()) {
            transition(PSTATE_IDLE, nowMs);
        }
        break;

    case PSTATE_EATING:
        // Return to idle after animation completes (non-looping)
        if (nowMs - _stateEnterTime >= 2000) {
            transition(PSTATE_HAPPY, nowMs);
        }
        break;

    case PSTATE_HAPPY:
        if (nowMs - _stateEnterTime >= 3000) {
            transition(PSTATE_IDLE, nowMs);
        }
        break;

    case PSTATE_SLEEPING:
        if (ntpValid && !isSleepTime(hour)) {
            transition(PSTATE_IDLE, nowMs);
            onWake();
        }
        break;

    case PSTATE_EVOLVING:
        if (nowMs - _stateEnterTime >= 4000) {
            transition(PSTATE_HAPPY, nowMs);
        }
        break;
    }

    // Advance animation frame
    const PetFrame* frame = _petState.currentFrame(_species);
    if (frame && (nowMs - _petState.lastFrameTime >= frame->durationMs)) {
        _petState.lastFrameTime = nowMs;
        _petState.advanceFrame(_species);
    }
}

void PetAI::onFeed() {
    _petState.lastFeedTime = millis();
    transition(PSTATE_EATING, millis());
}

void PetAI::onReward() {
    if (_primaryState == PSTATE_SLEEPING) {
        _queuedReward = true;
    } else {
        transition(PSTATE_HAPPY, millis());
    }
}

void PetAI::onEvolve() {
    transition(PSTATE_EVOLVING, millis());
}

void PetAI::onWake() {
    _petState.posX = random(40, 280);
    _petState.posY = random(100, 180);
    _petState.targetX = _petState.posX;
    _petState.targetY = _petState.posY;
}

bool PetAI::isHungry() const {
    if (_petState.lastFeedTime == UINT32_MAX) return true;  // never fed
    uint32_t now = millis();
    if (_petState.lastFeedTime > now) return false;  // crossed reboot boundary
    return (now - _petState.lastFeedTime) >= HUNGER_MS;
}

bool PetAI::isSleepTime(uint8_t hour) const {
    return (hour >= SLEEP_START || hour < SLEEP_END);
}

void PetAI::transition(PetPrimaryState newState, uint32_t nowMs) {
    if (_primaryState == newState) return;
    _primaryState = newState;
    _stateEnterTime = nowMs;
    _petState.resetAnimation();

    switch (newState) {
    case PSTATE_IDLE:     _petState.anim = ANIM_IDLE; break;
    case PSTATE_HUNGRY:   _petState.anim = ANIM_IDLE; break;
    case PSTATE_EATING:   _petState.anim = ANIM_EATING; break;
    case PSTATE_HAPPY:    _petState.anim = ANIM_HAPPY; break;
    case PSTATE_SLEEPING: _petState.anim = ANIM_SLEEPING; break;
    case PSTATE_EVOLVING: _petState.anim = ANIM_HAPPY; break;
    }
}

void PetAI::pickNewIdleSubState(uint32_t nowMs) {
    _subStateTimer = nowMs;
    uint8_t r = random(100);
    if (r < 60) {
        _idleSub = ISUB_ROAMING;
        pickRoamTarget(nowMs);
    } else if (r < 90) {
        _idleSub = ISUB_SITTING;
        _petState.moving = false;
    } else {
        _idleSub = ISUB_CURIOSITY;
        _petState.targetX = random(40, 280);
        _petState.targetY = random(60, 200);
        _petState.moving = true;
    }
}

void PetAI::pickRoamTarget(uint32_t nowMs) {
    _roamTargetTimer = nowMs;
    _petState.targetX = random(40, 280);
    _petState.targetY = random(60, 200);
    _petState.moving = true;
}

void PetAI::updateMovement() {
    if (!_petState.moving) return;
    int16_t dx = _petState.targetX - _petState.posX;
    int16_t dy = _petState.targetY - _petState.posY;

    if (abs(dx) <= 1 && abs(dy) <= 1) {
        _petState.posX = _petState.targetX;
        _petState.posY = _petState.targetY;
        _petState.moving = false;
        return;
    }

    if (abs(dx) > 1) _petState.posX += (dx > 0) ? 1 : -1;
    if (abs(dy) > 1) _petState.posY += (dy > 0) ? 1 : -1;
}
