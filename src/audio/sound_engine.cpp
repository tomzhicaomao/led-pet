#include "sound_engine.h"
#ifdef ARDUINO
#include <Arduino.h>
#define BUZZER_PIN 25
#endif

SoundEngine::SoundEngine()
    : _playing(false)
    , _volume(50)
    , _currentSound(SOUND_FEED)
    , _startTime(0)
    , _toneDuration(0)
    , _toneFrequency(0)
{
}

void SoundEngine::begin() {
#ifdef ARDUINO
    ledcSetup(0, 5000, 8);
    ledcAttachPin(BUZZER_PIN, 0);
#endif
    _playing = false;
}

void SoundEngine::play(SoundType sound) {
    _currentSound = sound;
    _startTime = millis();
    _playing = true;

    switch (sound) {
        case SOUND_FEED:      _toneFrequency = 1000; _toneDuration = 100; break;
        case SOUND_PET:       _toneFrequency = 800; _toneDuration = 150; break;
        case SOUND_PLAY:      _toneFrequency = 1200; _toneDuration = 100; break;
        case SOUND_EVOLVE:    _toneFrequency = 1500; _toneDuration = 200; break;
        case SOUND_ERROR:     _toneFrequency = 300; _toneDuration = 200; break;
        case SOUND_ACHIEVEMENT: _toneFrequency = 1400; _toneDuration = 150; break;
        default: _toneFrequency = 0; _toneDuration = 0; break;
    }

    if (_toneFrequency > 0) {
#ifdef ARDUINO
        ledcWriteTone(0, _toneFrequency);
#endif
    }
}

void SoundEngine::update(uint32_t nowMs) {
    if (!_playing) return;

    if (nowMs - _startTime >= _toneDuration) {
        stop();
    }
}

void SoundEngine::stop() {
#ifdef ARDUINO
    ledcWriteTone(0, 0);
#endif
    _playing = false;
}

void SoundEngine::setVolume(uint8_t volume) {
    _volume = volume;
}
