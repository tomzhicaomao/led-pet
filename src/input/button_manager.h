#pragma once
#include <cstdint>
#include "../core/config.h"

enum ButtonID : uint8_t {
    BTN_A = 0,
    BTN_B = 1,
    BTN_C = 2,
    BTN_COUNT = 3,
};

enum ButtonEvent : uint8_t {
    BTN_EVT_NONE       = 0,
    BTN_EVT_SHORT      = 1,
    BTN_EVT_LONG       = 2,
};

class ButtonManager {
public:
    ButtonManager();

    void begin();
    void update(uint32_t nowMs);

    bool hasEvent(ButtonID& btn, ButtonEvent& evt);

private:
    struct BtnState {
        uint8_t pin;
        uint32_t pressTime;
        bool pressed;
        bool eventPending;
        ButtonEvent pendingEvent;
    };

    BtnState _btns[BTN_COUNT];
};
