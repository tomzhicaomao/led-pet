#pragma once
#include "pet_data.h"
#include "../display/sprite_renderer.h"

// Convert a PetFrame to a generic Sprite for the renderer
inline Sprite petFrameToSprite(const PetFrame* frame) {
    if (!frame) return {nullptr, 0, 0};
    return {frame->data, frame->w, frame->h};
}
