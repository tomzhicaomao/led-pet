#pragma once
#include <Arduino.h>
#include <IPAddress.h>

class PetWiFiManager {
public:
    bool begin(const char* apName = "LED-Pet");
    IPAddress getLocalIP();
    bool isConnected();
    void resetSettings();
};
