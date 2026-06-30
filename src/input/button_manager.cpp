#include "button_manager.h"
#include <Arduino.h>

ButtonManager::ButtonManager() {
    _btns[BTN_A] = {BTN_A_PIN, 0, false, false, BTN_EVT_NONE};
    _btns[BTN_B] = {BTN_B_PIN, 0, false, false, BTN_EVT_NONE};
    _btns[BTN_C] = {BTN_C_PIN, 0, false, false, BTN_EVT_NONE};
}

void ButtonManager::begin() {
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        pinMode(_btns[i].pin, INPUT_PULLUP);
    }
}

void ButtonManager::update(uint32_t nowMs) {
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        bool reading = !digitalRead(_btns[i].pin);  // active low

        if (reading && !_btns[i].pressed) {
            // Just pressed
            _btns[i].pressed = true;
            _btns[i].pressTime = nowMs;
        } else if (!reading && _btns[i].pressed) {
            // Just released
            _btns[i].pressed = false;
            uint32_t held = nowMs - _btns[i].pressTime;
            if (held >= BTN_DEBOUNCE_MS) {
                _btns[i].pendingEvent = (held >= BTN_LONG_PRESS_MS)
                    ? BTN_EVT_LONG : BTN_EVT_SHORT;
                _btns[i].eventPending = true;
            }
        }
    }
}

bool ButtonManager::hasEvent(ButtonID& btn, ButtonEvent& evt) {
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        if (_btns[i].eventPending) {
            btn = static_cast<ButtonID>(i);
            evt = _btns[i].pendingEvent;
            _btns[i].eventPending = false;
            _btns[i].pendingEvent = BTN_EVT_NONE;
            return true;
        }
    }
    return false;
}
