#pragma once
#include "../pet/pet.h"
#include "../game/economy.h"
#include "../game/evolution.h"
#include "../game/achievement.h"
#include "../display/expression_renderer.h"
#include "../audio/sound_engine.h"
#include "../core/clock.h"

class App {
public:
    App();

    // 初始化
    void begin();

    // 主循环
    void loop();

    // 交互接口
    void feed();
    void pet();
    void play();
    void rest();
    void buyFood(uint8_t foodType);
    void buyEvolutionStone();
    bool addReward(uint16_t points, const char* pin);

    // 状态查询
    PetState getPetState() const { return _pet.getState(); }
    uint8_t getNeedValue(NeedType type) const { return _pet.getNeedValue(type); }
    uint16_t getPoints() const { return _economy.getPoints(); }
    uint8_t getFoodCount(uint8_t type) const { return _economy.getFoodCount(type); }
    uint8_t getEvolutionStage() const { return _evolution.getStage(); }
    const char* getEvolutionPath() const { return _evolution.getPathName(); }
    ExpressionType getExpression() const { return _pet.getCurrentExpression(); }

    // 渲染
    void render();

private:
    Pet _pet;
    Economy _economy;
    Evolution _evolution;
    AchievementSystem _achievements;
    ExpressionRenderer _expressionRenderer;
    SoundEngine _soundEngine;
    Clock _clock;

    uint32_t _lastUpdate;
    uint32_t _lastRender;

    void update();
    void checkAchievements();
};
