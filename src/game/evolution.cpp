#include "evolution.h"
#include <cstring>

static const char* PATH_NAMES[PATH_COUNT] = {
    "Kind", "Playful", "Wild", "Wise"
};

Evolution::Evolution()
    : _stage(0)
    , _feedCount(0)
    , _totalFeedCount(0)
    , _path(PATH_KIND)
    , _pathDetermined(false)
{
}

void Evolution::begin() {
    _stage = 0;
    _feedCount = 0;
    _totalFeedCount = 0;
    _path = PATH_KIND;
    _pathDetermined = false;
}

void Evolution::update(uint8_t feedCount, uint8_t bond, uint8_t happiness, uint8_t energy) {
    _feedCount = feedCount;
    _totalFeedCount += feedCount;

    if (!_pathDetermined && _stage == 2) {
        calculatePath(bond, happiness, energy);
    }
}

bool Evolution::canEvolve() const {
    if (_stage >= 4) return false;
    uint16_t threshold = EVOLUTION_THRESHOLDS[_stage].feedCount;
    return _feedCount >= threshold;
}

bool Evolution::evolve(bool hasStone) {
    if (!canEvolve()) return false;
    if (EVOLUTION_THRESHOLDS[_stage].requiresStone && !hasStone) return false;
    _stage++;
    _feedCount = 0;
    return true;
}

void Evolution::calculatePath(uint8_t bond, uint8_t happiness, uint8_t energy) {
    uint8_t scores[PATH_COUNT] = {0};
    scores[PATH_KIND] = bond * 2 + (100 - abs(bond - happiness));
    scores[PATH_PLAYFUL] = happiness * 2 + (100 - abs(happiness - energy));
    scores[PATH_WILD] = (100 - bond) + energy * 2;
    scores[PATH_WISE] = _totalFeedCount / 10 + (100 - abs(bond - energy));

    uint8_t maxScore = 0;
    _path = PATH_KIND;
    for (uint8_t i = 0; i < PATH_COUNT; i++) {
        if (scores[i] > maxScore) {
            maxScore = scores[i];
            _path = static_cast<EvolutionPath>(i);
        }
    }
    _pathDetermined = true;
}

const char* Evolution::getPathName() const {
    return getPathName(_path);
}

const char* Evolution::getPathName(EvolutionPath path) const {
    if (path >= PATH_COUNT) return "Unknown";
    return PATH_NAMES[path];
}

void Evolution::serialize(uint8_t* buffer) const {
    buffer[0] = _stage;
    buffer[1] = _feedCount;
    memcpy(buffer + 2, &_totalFeedCount, 2);
    buffer[4] = static_cast<uint8_t>(_path);
    buffer[5] = _pathDetermined ? 1 : 0;
}

void Evolution::deserialize(const uint8_t* buffer) {
    _stage = buffer[0];
    _feedCount = buffer[1];
    memcpy(&_totalFeedCount, buffer + 2, 2);
    _path = static_cast<EvolutionPath>(buffer[4]);
    _pathDetermined = buffer[5] == 1;
}
