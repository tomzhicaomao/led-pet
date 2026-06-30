#pragma once
#include "game_state.h"

class Persistence {
public:
    static bool save(uint8_t petIndex, const GameState& state);
    static bool load(uint8_t petIndex, GameState& state);
    static bool saveMeta(uint8_t activePet);
    static bool loadMeta(uint8_t& activePet);
    static void reset(GameState& state);
    static void eraseAll();
};
