#pragma once
#include <Arduino.h>
#include <WebServer.h>
#include <SPIFFS.h>

class GameEngine;
class PetAI;

class PetWebServer {
public:
    void begin();
    void handleClient();
    void setGameEngine(GameEngine* engine);
    void setPetAI(PetAI* ai);
    WebServer& getServer() { return _server; }

private:
    WebServer _server{80};
    GameEngine* _engine = nullptr;
    PetAI* _ai = nullptr;

    void setupRoutes();
    void serveStatic(const char* path, const char* contentType);
    void addCorsHeaders();
};
