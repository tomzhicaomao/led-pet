#include "storage.h"
#ifdef ARDUINO
#include <Preferences.h>
#endif

Storage::Storage()
    : _initialized(false)
{
}

bool Storage::begin() {
#ifdef ARDUINO
    _initialized = true;
#else
    _initialized = true;
#endif
    return _initialized;
}

bool Storage::save(uint8_t slot, const uint8_t* data, uint16_t size) {
    if (!_initialized) return false;

#ifdef ARDUINO
    Preferences prefs;
    char key[16];
    snprintf(key, sizeof(key), "save_%d", slot);
    prefs.begin("pet", false);
    prefs.putBytes(key, data, size);
    prefs.end();
    return true;
#else
    return true;
#endif
}

bool Storage::load(uint8_t slot, uint8_t* data, uint16_t size) {
    if (!_initialized) return false;

#ifdef ARDUINO
    Preferences prefs;
    char key[16];
    snprintf(key, sizeof(key), "save_%d", slot);
    prefs.begin("pet", true);

    if (prefs.getBytesLength(key) < size) {
        prefs.end();
        return false;
    }

    prefs.getBytes(key, data, size);
    prefs.end();
    return true;
#else
    return true;
#endif
}

bool Storage::erase(uint8_t slot) {
    if (!_initialized) return false;

#ifdef ARDUINO
    Preferences prefs;
    char key[16];
    snprintf(key, sizeof(key), "save_%d", slot);
    prefs.begin("pet", false);
    prefs.remove(key);
    prefs.end();
    return true;
#else
    return true;
#endif
}

bool Storage::eraseAll() {
    if (!_initialized) return false;

#ifdef ARDUINO
    Preferences prefs;
    prefs.begin("pet", false);
    prefs.clear();
    prefs.end();
    return true;
#else
    return true;
#endif
}
