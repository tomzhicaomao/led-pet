#include "app.h"

App::App()
    : _lastUpdate(0)
    , _lastRender(0)
{
}

void App::begin() {
    _pet.begin();
    _economy.begin();
    _evolution.begin();
    _achievements.begin();
    _soundEngine.begin();
    _clock.begin();
    _lastUpdate = millis();
    _lastRender = millis();
}

void App::loop() {
    uint32_t now = millis();
    _soundEngine.update(now);
    if (now - _lastUpdate >= 1000) {
        update();
        _lastUpdate = now;
    }
    if (now - _lastRender >= 16) {
        render();
        _lastRender = now;
    }
}

void App::feed() {
    if (_economy.useFood(0)) {
        _pet.feed();
        _evolution.update(10, _pet.getNeedValue(NEED_BOND),
                         _pet.getNeedValue(NEED_HAPPINESS),
                         _pet.getNeedValue(NEED_ENERGY));
        checkAchievements();
    }
}

void App::pet() {
    _pet.pet();
    checkAchievements();
}

void App::play() {
    _pet.play();
    checkAchievements();
}

void App::rest() {
    _pet.rest();
}

void App::buyFood(uint8_t foodType) {
    _economy.buyFood(foodType);
}

void App::buyEvolutionStone() {
    _economy.buyEvolutionStone();
}

bool App::addReward(uint16_t points, const char* pin) {
    return _economy.addPoints(points, pin);
}

void App::update() {
    uint32_t now = millis();
    _pet.update(now);
    _evolution.update(0, _pet.getNeedValue(NEED_BOND),
                     _pet.getNeedValue(NEED_HAPPINESS),
                     _pet.getNeedValue(NEED_ENERGY));
    checkAchievements();
}

void App::checkAchievements() {
    _achievements.checkBondLevel(_pet.getNeedValue(NEED_BOND));
    _achievements.checkEvolution(_evolution.getStage());
    _achievements.checkCareMaster(
        _pet.getNeedValue(NEED_HUNGER),
        _pet.getNeedValue(NEED_HAPPINESS),
        _pet.getNeedValue(NEED_ENERGY),
        _pet.getNeedValue(NEED_BOND)
    );
}

void App::render() {
    _expressionRenderer.update(_pet.getCurrentExpression(), 100);
    _expressionRenderer.render(160, 120);
}
