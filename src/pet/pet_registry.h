#pragma once
#include "pet_data.h"

class PetRegistry {
public:
    static PetRegistry& instance();

    const PetSpecies* get(uint8_t index) const;
    uint8_t count() const;

    void registerSpecies(const PetSpecies* species);

private:
    PetRegistry() = default;
    static const uint8_t MAX_SPECIES = 8;
    const PetSpecies* _species[MAX_SPECIES] = {};
    uint8_t _count = 0;
};
