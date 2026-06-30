#pragma once
#include <Arduino.h>
#include <WebServer.h>

class OTAUpdater {
public:
    void begin(WebServer& server, const char* user = "admin", const char* pass = "ledpet123");
    bool isUpdating();
private:
    bool _updating = false;
};
