#include "buzzer.h"
#include <Arduino.h>

// Melodies
static const Buzzer::Note feedNotes[] = {
    {523, 120}, {659, 120},
};
static const Buzzer::Note rewardNotes[] = {
    {523, 100}, {659, 100}, {784, 100}, {1047, 100}, {784, 100}, {1047, 200},
};
static const Buzzer::Note evolveNotes[] = {
    {392, 150}, {523, 150}, {659, 150}, {784, 150}, {1047, 300},
};
static const Buzzer::Note errorNotes[] = {
    {200, 200}, {150, 200},
};

static const Buzzer::Melody melodies[] = {
    {feedNotes,   2},
    {rewardNotes, 6},
    {evolveNotes, 5},
    {errorNotes,  2},
};

Buzzer::Buzzer() : _noteIndex(0), _noteStartTime(0),
    _currentMelody(nullptr), _silent(false) {}

void Buzzer::begin() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void Buzzer::update(uint32_t nowMs) {
    if (!_currentMelody || _silent) return;
    if (_noteIndex >= _currentMelody->count) {
        stopBuzzer();
        _currentMelody = nullptr;
        return;
    }
    const auto& note = _currentMelody->notes[_noteIndex];
    if (nowMs - _noteStartTime >= note.durationMs) {
        _noteIndex++;
        if (_noteIndex < _currentMelody->count) {
            _noteStartTime = nowMs;
            tone(BUZZER_PIN, _currentMelody->notes[_noteIndex].freq);
        } else {
            stopBuzzer();
            _currentMelody = nullptr;
        }
    }
}

void Buzzer::play(ToneID tone) {
    if (tone < 4) startMelody(&melodies[tone], millis());
}

void Buzzer::setSilent(bool silent) {
    _silent = silent;
    if (silent) stopBuzzer();
}

bool Buzzer::isPlaying() const {
    return _currentMelody != nullptr;
}

void Buzzer::startMelody(const Melody* mel, uint32_t nowMs) {
    _currentMelody = mel;
    _noteIndex = 0;
    _noteStartTime = nowMs;
    if (!_silent && mel->count > 0) {
        tone(BUZZER_PIN, mel->notes[0].freq);
    }
}

void Buzzer::stopBuzzer() {
    noTone(BUZZER_PIN);
}
