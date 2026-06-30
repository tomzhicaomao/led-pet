#include "persistence.h"
#include <Preferences.h>
#include <cstring>

static const char* NVS_NS = "pet";
static const char* NVS_KEY_PREFIX = "save_";
static const char* NVS_META_KEY = "meta";
static const uint32_t SAVE_VERSION = 2;

struct SaveData {
    uint32_t version;
    GameState state;
};

struct MetaData {
    uint32_t version;
    uint8_t activePet;
};

bool Persistence::save(uint8_t petIndex, const GameState& state) {
    Preferences prefs;
    if (!prefs.begin(NVS_NS, false)) return false;
    char key[16];
    snprintf(key, sizeof(key), "%s%d", NVS_KEY_PREFIX, petIndex);
    SaveData data = {SAVE_VERSION, state};
    bool ok = prefs.putBytes(key, &data, sizeof(data));
    prefs.end();
    return ok;
}

bool Persistence::load(uint8_t petIndex, GameState& state) {
    Preferences prefs;
    if (!prefs.begin(NVS_NS, true)) return false;
    char key[16];
    snprintf(key, sizeof(key), "%s%d", NVS_KEY_PREFIX, petIndex);
    size_t len = prefs.getBytesLength(key);
    if (len == 0 || len != sizeof(SaveData)) { prefs.end(); return false; }
    SaveData data;
    prefs.getBytes(key, &data, sizeof(data));
    if (data.version > SAVE_VERSION) { prefs.end(); return false; }

    // Migrate from old save format (0-sentinel → UINT32_MAX)
    state = data.state;
    if (state.lastFeedTime == 0) state.lastFeedTime = UINT32_MAX;
    // Handle cross-reboot time discontinuity using signed arithmetic
    else {
        int32_t diff = static_cast<int32_t>(millis() - state.lastFeedTime);
        if (diff < 0) state.lastFeedTime = millis();
    }

    prefs.end();
    return true;
}

bool Persistence::saveMeta(uint8_t activePet) {
    Preferences prefs;
    if (!prefs.begin(NVS_NS, false)) return false;
    MetaData data = {SAVE_VERSION, activePet};
    bool ok = prefs.putBytes(NVS_META_KEY, &data, sizeof(data));
    prefs.end();
    return ok;
}

bool Persistence::loadMeta(uint8_t& activePet) {
    Preferences prefs;
    if (!prefs.begin(NVS_NS, true)) return false;
    size_t len = prefs.getBytesLength(NVS_META_KEY);
    if (len != sizeof(MetaData)) { prefs.end(); return false; }
    MetaData data;
    prefs.getBytes(NVS_META_KEY, &data, sizeof(data));
    activePet = data.activePet;
    prefs.end();
    return true;
}

void Persistence::reset(GameState& state) {
    state = GameState();
    strncpy(state.parentPIN, "0000", 4);
    state.parentPIN[4] = '\0';
    state.initialized = true;
    state.lastFeedTime = UINT32_MAX;
}

void Persistence::eraseAll() {
    Preferences prefs;
    prefs.begin(NVS_NS, false);
    prefs.clear();
    prefs.end();
}
