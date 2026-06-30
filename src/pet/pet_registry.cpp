#include "pet_registry.h"
#include "species/pet_dragon.h"

PetRegistry& PetRegistry::instance() {
    static PetRegistry reg;
    return reg;
}

const PetSpecies* PetRegistry::get(uint8_t index) const {
    return (index < _count) ? _species[index] : nullptr;
}

uint8_t PetRegistry::count() const {
    return _count;
}

void PetRegistry::registerSpecies(const PetSpecies* species) {
    if (_count < MAX_SPECIES && species) {
        _species[_count++] = species;
    }
}
