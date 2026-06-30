#pragma once
#include <cstdint>
#include "../display/sprite_renderer.h"

#define MAX_FRAMES_PER_ANIM 8
#define MAX_ANIMS_PER_STAGE 4
#define STAGE_COUNT 5

struct PetFrame {
    const uint16_t* data;
    uint16_t w;
    uint16_t h;
    uint16_t durationMs;
};

struct PetAnimation {
    const char* name;
    const PetFrame* frames;
    uint8_t frameCount;
    bool loop;
};

struct PetStage {
    const char* name;
    uint16_t feedThreshold;
    PetAnimation animations[MAX_ANIMS_PER_STAGE];
};

enum AnimType : uint8_t {
    ANIM_IDLE     = 0,
    ANIM_EATING   = 1,
    ANIM_HAPPY    = 2,
    ANIM_SLEEPING = 3,
    ANIM_COUNT    = 4
};

enum EvolveStage : uint8_t {
    STAGE_EGG   = 0,
    STAGE_BABY  = 1,
    STAGE_TEEN  = 2,
    STAGE_ADULT = 3,
    STAGE_FINAL = 4,
};

enum PetMood : uint8_t {
    MOOD_HAPPY   = 0,
    MOOD_HUNGRY  = 1,
    MOOD_SLEEPY  = 2,
    MOOD_EXCITED = 3,
};

struct PetSpecies {
    const char* name;
    uint16_t primaryColor;
    uint16_t accentColor;
    uint8_t stageCount;
    const PetStage* stages;
};

struct PetState {
    uint8_t currentSpecies = 0;
    EvolveStage stage = STAGE_EGG;
    AnimType anim = ANIM_IDLE;
    PetMood mood = MOOD_HAPPY;
    uint8_t frameIndex = 0;
    uint32_t lastFrameTime = 0;
    uint32_t lastFeedTime = UINT32_MAX;

    int16_t posX = 140;
    int16_t posY = 140;
    int16_t targetX = 140;
    int16_t targetY = 140;
    bool moving = false;

    void resetAnimation();
    const PetFrame* currentFrame(const PetSpecies* species) const;
    void advanceFrame(const PetSpecies* species);
};
