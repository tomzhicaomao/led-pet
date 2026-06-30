#pragma once
#include <cstdint>

class Storage {
public:
    Storage();

    bool begin();
    bool save(uint8_t slot, const uint8_t* data, uint16_t size);
    bool load(uint8_t slot, uint8_t* data, uint16_t size);
    bool erase(uint8_t slot);
    bool eraseAll();

private:
    bool _initialized;
};
