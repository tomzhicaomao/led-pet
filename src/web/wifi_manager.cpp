#include "wifi_manager.h"
#include <WiFiManager.h>
#include <WiFi.h>

bool PetWiFiManager::begin(const char* apName) {
    Serial.println("WiFi: Starting connection...");
    WiFi.mode(WIFI_STA);
    ::WiFiManager wm;
    wm.setConnectTimeout(30);
    wm.setConfigPortalTimeout(180);
    if (!wm.autoConnect(apName)) {
        Serial.println("WiFi: Failed to connect!");
        return false;
    }
    Serial.print("WiFi: Connected. IP: ");
    Serial.println(WiFi.localIP());
    return true;
}

IPAddress PetWiFiManager::getLocalIP() { return WiFi.localIP(); }
bool PetWiFiManager::isConnected() { return WiFi.status() == WL_CONNECTED; }
void PetWiFiManager::resetSettings() { ::WiFiManager wm; wm.resetSettings(); }
