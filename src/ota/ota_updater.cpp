#include "ota_updater.h"
#include <HTTPUpdateServer.h>
#include <ESPmDNS.h>

static HTTPUpdateServer httpUpdater;

void OTAUpdater::begin(WebServer& server, const char* user, const char* pass) {
    httpUpdater.setup(&server, user, pass);
    MDNS.begin("led-pet");
    MDNS.addService("http", "tcp", 80);
    Serial.println("OTA: http://led-pet.local/update");
}

bool OTAUpdater::isUpdating() { return _updating; }
