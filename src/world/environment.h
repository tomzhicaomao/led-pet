#pragma once
#include <cstdint>

enum SceneType : uint8_t {
    SCENE_INDOOR = 0,
    SCENE_GARDEN = 1,
    SCENE_FOREST = 2,
    SCENE_CAVE = 3,
    SCENE_SKY = 4,
    SCENE_COUNT = 5
};

enum TimeOfDay : uint8_t {
    TIME_MORNING = 0,
    TIME_AFTERNOON = 1,
    TIME_EVENING = 2,
    TIME_NIGHT = 3,
    TIME_COUNT = 4
};

class Environment {
public:
    Environment();

    void begin();
    void update(uint8_t hour);

    SceneType getScene() const { return _scene; }
    TimeOfDay getTimeOfDay() const { return _timeOfDay; }
    uint16_t getBackgroundColor() const;
    const char* getSceneName() const;

    void setScene(SceneType scene) { _scene = scene; }

private:
    SceneType _scene;
    TimeOfDay _timeOfDay;

    TimeOfDay calculateTimeOfDay(uint8_t hour) const;
};
