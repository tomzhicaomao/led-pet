#include "environment.h"

static const char* SCENE_NAMES[SCENE_COUNT] = {
    "Indoor", "Garden", "Forest", "Cave", "Sky"
};

static const uint16_t BG_COLORS[SCENE_COUNT][TIME_COUNT] = {
    {0xFFFF, 0xFFE0, 0xF81F, 0x001F},
    {0x07E0, 0x07FF, 0x051F, 0x0010},
    {0x03E0, 0x03FF, 0x021F, 0x0008},
    {0x7BEF, 0x7BCF, 0x7B8C, 0x7BCF},
    {0x87FF, 0x07FF, 0xFD1F, 0x001F}
};

Environment::Environment()
    : _scene(SCENE_INDOOR)
    , _timeOfDay(TIME_AFTERNOON)
{
}

void Environment::begin() {
    _scene = SCENE_INDOOR;
    _timeOfDay = TIME_AFTERNOON;
}

void Environment::update(uint8_t hour) {
    _timeOfDay = calculateTimeOfDay(hour);
}

uint16_t Environment::getBackgroundColor() const {
    return BG_COLORS[_scene][_timeOfDay];
}

const char* Environment::getSceneName() const {
    return SCENE_NAMES[_scene];
}

TimeOfDay Environment::calculateTimeOfDay(uint8_t hour) const {
    if (hour >= 6 && hour < 12) return TIME_MORNING;
    if (hour >= 12 && hour < 18) return TIME_AFTERNOON;
    if (hour >= 18 && hour < 22) return TIME_EVENING;
    return TIME_NIGHT;
}
