#include "pet_data.h"

void PetState::resetAnimation() {
    frameIndex = 0;
    lastFrameTime = 0;
}

const PetFrame* PetState::currentFrame(const PetSpecies* species) const {
    if (!species || stage >= species->stageCount) return nullptr;
    const auto& anim = species->stages[stage].animations[anim];
    if (frameIndex >= anim.frameCount) return nullptr;
    return &anim.frames[frameIndex];
}

void PetState::advanceFrame(const PetSpecies* species) {
    if (!species || stage >= species->stageCount) return;
    const auto& anim = species->stages[stage].animations[anim];
    frameIndex++;
    if (frameIndex >= anim.frameCount) {
        frameIndex = anim.loop ? 0 : anim.frameCount - 1;
    }
}
