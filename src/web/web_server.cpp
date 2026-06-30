#include "web_server.h"
#include "../game/game_state.h"
#include "../pet/pet_ai.h"
#include <cstring>

void PetWebServer::begin() {
    if (!SPIFFS.begin(true)) Serial.println("SPIFFS mount failed!");
    setupRoutes();
    _server.begin();
    Serial.println("HTTP server started on port 80");
}

void PetWebServer::setGameEngine(GameEngine* engine) { _engine = engine; }
void PetWebServer::setPetAI(PetAI* ai) { _ai = ai; }

void PetWebServer::addCorsHeaders() {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void PetWebServer::serveStatic(const char* path, const char* contentType) {
    if (SPIFFS.exists(path)) {
        File file = SPIFFS.open(path, "r");
        _server.streamFile(file, contentType);
        file.close();
    } else {
        _server.send(404, "text/plain", "Not found");
    }
}

void PetWebServer::setupRoutes() {
    _server.on("/", HTTP_GET, [this]() { serveStatic("/index.html", "text/html"); });
    _server.on("/style.css", HTTP_GET, [this]() { serveStatic("/style.css", "text/css"); });
    _server.on("/app.js", HTTP_GET, [this]() { serveStatic("/app.js", "application/javascript"); });

    // CORS preflight for all /api/* routes
    _server.on("/api/status", HTTP_OPTIONS, [this]() { addCorsHeaders(); _server.send(204); });
    _server.on("/api/reward", HTTP_OPTIONS, [this]() { addCorsHeaders(); _server.send(204); });
    _server.on("/api/pin", HTTP_OPTIONS, [this]() { addCorsHeaders(); _server.send(204); });
    _server.on("/api/history", HTTP_OPTIONS, [this]() { addCorsHeaders(); _server.send(204); });

    // GET /api/status
    _server.on("/api/status", HTTP_GET, [this]() {
        addCorsHeaders();
        if (_engine) {
            _server.send(200, "application/json", _engine->getStatusJson());
            return;
        }
        _server.send(200, "application/json", "{\"name\":\"Dragon\",\"stage\":0,\"state\":\"happy\",\"points\":0}");
    });

    // POST /api/reward
    _server.on("/api/reward", HTTP_POST, [this]() {
        addCorsHeaders();
        if (!_engine) { _server.send(200, "application/json", "{\"ok\":false}"); return; }
        uint32_t pts = _server.hasArg("points") ? _server.arg("points").toInt() : 5;
        const char* pin = _server.hasArg("pin") ? _server.arg("pin").c_str() : "0000";
        const char* reason = _server.hasArg("reason") ? _server.arg("reason").c_str() : "";
        bool ok = _engine->addPoints(pts, pin);
        if (ok) {
            _engine->addRewardEntry(pts, reason);
            if (_ai) _ai->onReward();
        }
        char buf[64];
        snprintf(buf, sizeof(buf), "{\"ok\":%s}", ok ? "true" : "false");
        _server.send(200, "application/json", buf);
    });

    // POST /api/pin
    _server.on("/api/pin", HTTP_POST, [this]() {
        addCorsHeaders();
        if (!_engine) { _server.send(200, "application/json", "{\"ok\":false}"); return; }
        const char* oldPin = _server.hasArg("oldPin") ? _server.arg("oldPin").c_str() : "";
        const char* newPin = _server.hasArg("newPin") ? _server.arg("newPin").c_str() : "0000";
        if (strcmp(oldPin, _engine->getState().parentPIN) != 0) {
            _server.send(200, "application/json", "{\"ok\":false,\"error\":\"Wrong PIN\"}");
            return;
        }
        strncpy(_engine->getState().parentPIN, newPin, 4);
        _engine->getState().parentPIN[4] = '\0';
        _engine->save();
        _server.send(200, "application/json", "{\"ok\":true}");
    });

    // GET /api/history
    _server.on("/api/history", HTTP_GET, [this]() {
        addCorsHeaders();
        if (!_engine) { _server.send(200, "application/json", "[]"); return; }
        GameEngine::RewardEntry entries[10];
        uint8_t n = _engine->getRewardHistory(entries, 10);
        String json = "[";
        for (uint8_t i = 0; i < n; i++) {
            if (i > 0) json += ",";
            char entry[128];
            snprintf(entry, sizeof(entry),
                "{\"ts\":%lu,\"points\":%d,\"reason\":\"%s\"}",
                entries[i].timestamp, entries[i].points, entries[i].reason);
            json += entry;
        }
        json += "]";
        _server.send(200, "application/json", json);
    });

    _server.onNotFound([this]() {
        addCorsHeaders();
        _server.send(404, "application/json", "{\"error\":\"Not found\"}");
    });
}

void PetWebServer::handleClient() { _server.handleClient(); }
