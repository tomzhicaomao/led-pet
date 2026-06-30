#include "pet_dragon.h"
#include "../pet_data.h"

// Reuse test sprite as placeholder for all frames
extern const uint16_t test_sprite_dragon[];
#define DRAGON_W 32
#define DRAGON_H 32

// ---- Stage 0: EGG ----
static const PetFrame dragon_egg_idle[] = {
    {test_sprite_dragon, DRAGON_W, DRAGON_H, 400},
    {test_sprite_dragon, DRAGON_W, DRAGON_H, 400},
};
static const PetFrame dragon_egg_eat[] = {
    {test_sprite_dragon, DRAGON_W, DRAGON_H, 200},
    {test_sprite_dragon, DRAGON_W, DRAGON_H, 200},
};
static const PetFrame dragon_egg_happy[] = {
    {test_sprite_dragon, DRAGON_W, DRAGON_H, 250},
    {test_sprite_dragon, DRAGON_W, DRAGON_H, 250},
};
static const PetFrame dragon_egg_sleep[] = {
    {test_sprite_dragon, DRAGON_W, DRAGON_H, 800},
};

static const PetStage dragon_stages[STAGE_COUNT] = {
    {"Egg",   3,  {{{"idle", dragon_egg_idle, 2, true},
                     {"eat",  dragon_egg_eat,  2, false},
                     {"happy",dragon_egg_happy,2, false},
                     {"sleep",dragon_egg_sleep,1, true}}}},
    {"Baby",  8,  {{{"idle", dragon_egg_idle, 2, true},
                     {"eat",  dragon_egg_eat,  2, false},
                     {"happy",dragon_egg_happy,2, false},
                     {"sleep",dragon_egg_sleep,1, true}}}},
    {"Teen",  15, {{{"idle", dragon_egg_idle, 2, true},
                     {"eat",  dragon_egg_eat,  2, false},
                     {"happy",dragon_egg_happy,2, false},
                     {"sleep",dragon_egg_sleep,1, true}}}},
    {"Adult", 25, {{{"idle", dragon_egg_idle, 2, true},
                     {"eat",  dragon_egg_eat,  2, false},
                     {"happy",dragon_egg_happy,2, false},
                     {"sleep",dragon_egg_sleep,1, true}}}},
    {"Final", 0,  {{{"idle", dragon_egg_idle, 2, true},
                     {"eat",  dragon_egg_eat,  2, false},
                     {"happy",dragon_egg_happy,2, false},
                     {"sleep",dragon_egg_sleep,1, true}}}},
};

const PetSpecies dragon_species = {
    "Dragon",
    0xF800,  // primary: red
    0xFFE0,  // accent: yellow
    STAGE_COUNT,
    dragon_stages,
};
