#pragma once
#include <cstdint>

#define BUZZER_PIN 25

enum ToneID : uint8_t {
    TONE_FEED    = 0,
    TONE_REWARD  = 1,
    TONE_EVOLVE  = 2,
    TONE_ERROR   = 3,
};

class Buzzer {
public:
    Buzzer();

    void begin();
    void update(uint32_t nowMs);

    void play(ToneID tone);
    void setSilent(bool silent);
    bool isPlaying() const;

private:
    struct Note {
        uint16_t freq;
        uint16_t durationMs;
    };

    struct Melody {
        const Note* notes;
        uint8_t count;
    };

    uint8_t _noteIndex;
    uint32_t _noteStartTime;
    const Melody* _currentMelody;
    bool _silent;

    void startMelody(const Melody* mel, uint32_t nowMs);
    void stopBuzzer();
};
