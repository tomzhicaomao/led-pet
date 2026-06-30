#pragma once
#include <cstdint>

enum SoundType : uint8_t {
    SOUND_FEED = 0,
    SOUND_PET = 1,
    SOUND_PLAY = 2,
    SOUND_EVOLVE = 3,
    SOUND_ERROR = 4,
    SOUND_ACHIEVEMENT = 5,
    SOUND_COUNT = 6
};

class SoundEngine {
public:
    SoundEngine();

    void begin();
    void play(SoundType sound);
    void update(uint32_t nowMs);
    void stop();
    void setVolume(uint8_t volume);
    bool isPlaying() const { return _playing; }

private:
    bool _playing;
    uint8_t _volume;
    SoundType _currentSound;
    uint32_t _startTime;
    uint16_t _toneDuration;
    uint16_t _toneFrequency;
};
