#include <Arduino.h>
#include "app/app.h"

App app;

bool btnAState = false;
bool btnBState = false;
bool btnCState = false;
uint32_t btnALastPress = 0;
uint32_t btnBLastPress = 0;
uint32_t btnCLastPress = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("\nLED Pet - New Edition");

    pinMode(BTN_A_PIN, INPUT_PULLUP);
    pinMode(BTN_B_PIN, INPUT_PULLUP);
    pinMode(BTN_C_PIN, INPUT_PULLUP);

    app.begin();
    Serial.println("Setup complete. Pet is alive!");
}

void loop() {
    bool btnA = digitalRead(BTN_A_PIN) == LOW;
    bool btnB = digitalRead(BTN_B_PIN) == LOW;
    bool btnC = digitalRead(BTN_C_PIN) == LOW;
    uint32_t now = millis();

    if (btnA && !btnAState) {
        btnALastPress = now;
    } else if (!btnA && btnAState) {
        uint32_t duration = now - btnALastPress;
        if (duration < 1000) {
            app.feed();
            Serial.println("Feed!");
        } else {
            app.pet();
            Serial.println("Pet!");
        }
    }
    btnAState = btnA;

    if (btnB && !btnBState) {
        btnBLastPress = now;
    } else if (!btnB && btnBState) {
        uint32_t duration = now - btnBLastPress;
        if (duration < 1000) {
            app.play();
            Serial.println("Play!");
        } else {
            app.buyFood(0);
            Serial.println("Buy food!");
        }
    }
    btnBState = btnB;

    if (btnC && !btnCState) {
        btnCLastPress = now;
    } else if (!btnC && btnCState) {
        uint32_t duration = now - btnCLastPress;
        if (duration < 1000) {
            app.rest();
            Serial.println("Rest!");
        }
    }
    btnCState = btnC;

    app.loop();
    delay(10);
}
