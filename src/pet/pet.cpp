#include "pet.h"

Pet::Pet()
    : _config(DEFAULT_CONFIG)
    , _consecutiveInteractions(0)
    , _lastInteractionTime(0)
    , _fatigued(false)
{
}

void Pet::begin() {
    _needs.begin();
    _state.transitionTo(STATE_IDLE, 0);
    _expression.update(EXPR_NONE);
}

void Pet::update(uint32_t nowMs) {
    // 更新需求
    _needs.update(nowMs);

    // 检查疲劳
    checkFatigue();

    // 更新状态
    updateState();

    // 更新表情
    updateExpression();
}

void Pet::feed() {
    if (_fatigued) return;

    handleInteraction();
    _needs.getNeed(NEED_HUNGER).recover(20);
    _personality.developFromInteraction(INTERACT_FEED);
    _state.transitionTo(STATE_EATING);
}

void Pet::pet() {
    if (_fatigued) return;

    handleInteraction();
    _needs.getNeed(NEED_BOND).recover(10);
    _personality.developFromInteraction(INTERACT_PET);
    _state.transitionTo(STATE_HAPPY);
}

void Pet::play() {
    if (_fatigued) return;

    handleInteraction();
    _needs.getNeed(NEED_HAPPINESS).recover(15);
    _needs.getNeed(NEED_ENERGY).drain(5); // 玩耍消耗能量
    _personality.developFromInteraction(INTERACT_PLAY);
    _state.transitionTo(STATE_PLAYING);
}

void Pet::rest() {
    handleInteraction();
    _needs.getNeed(NEED_ENERGY).recover(25);
    _personality.developFromInteraction(INTERACT_REST);
    _state.transitionTo(STATE_SLEEPING);
}

uint8_t Pet::getNeedValue(NeedType type) const {
    return _needs.getNeed(type).value();
}

bool Pet::isNeedCritical(NeedType type) const {
    return _needs.getNeed(type).isCritical();
}

void Pet::updateState() {
    uint8_t hunger = _needs.getNeed(NEED_HUNGER).value();
    uint8_t happiness = _needs.getNeed(NEED_HAPPINESS).value();
    uint8_t energy = _needs.getNeed(NEED_ENERGY).value();

    // 根据需求值决定状态
    if (hunger <= 20) {
        _state.transitionTo(STATE_HUNGRY);
    } else if (happiness >= 80) {
        _state.transitionTo(STATE_HAPPY);
    } else if (energy <= 25) {
        _state.transitionTo(STATE_SLEEPING);
    } else {
        _state.transitionTo(STATE_IDLE);
    }
}

void Pet::updateExpression() {
    uint8_t hunger = _needs.getNeed(NEED_HUNGER).value();
    uint8_t happiness = _needs.getNeed(NEED_HAPPINESS).value();
    uint8_t energy = _needs.getNeed(NEED_ENERGY).value();
    uint8_t bond = _needs.getNeed(NEED_BOND).value();

    // 根据需求值决定表情
    if (_fatigued) {
        _expression.update(EXPR_FATIGUED, 100);
    } else if (hunger <= 20) {
        _expression.update(EXPR_HUNGRY, 100 - hunger);
    } else if (happiness >= 80) {
        _expression.update(EXPR_HAPPY, happiness);
    } else if (energy <= 25) {
        _expression.update(EXPR_TIRED, 100 - energy);
    } else if (bond <= 10) {
        _expression.update(EXPR_LONELY, 100 - bond);
    } else {
        _expression.update(EXPR_NONE);
    }
}

void Pet::checkFatigue() {
    if (_fatigued) {
        // 检查疲劳是否解除
        uint32_t elapsed = millis() - _lastInteractionTime;
        if (elapsed >= _config.fatigueCooldownMs) {
            _fatigued = false;
            _consecutiveInteractions = 0;
        }
    }
}

void Pet::handleInteraction() {
    _consecutiveInteractions++;
    _lastInteractionTime = millis();

    if (_consecutiveInteractions >= _config.fatigueThreshold) {
        _fatigued = true;
    }
}

void Pet::serialize(uint8_t* buffer) const {
    uint16_t offset = 0;

    // 序列化需求系统
    _needs.serialize(buffer + offset);
    offset += NEED_COUNT * 9;

    // 序列化性格系统
    _personality.serialize(buffer + offset);
    offset += TRAIT_COUNT;

    // 序列化状态机
    buffer[offset] = static_cast<uint8_t>(_state.currentState());
    offset += 1;

    // 序列化表情系统
    _expression.serialize(buffer + offset);
    offset += 2;

    // 序列化疲劳系统
    buffer[offset] = _consecutiveInteractions;
    offset += 1;
    buffer[offset] = _fatigued ? 1 : 0;
    offset += 1;
}

void Pet::deserialize(const uint8_t* buffer) {
    uint16_t offset = 0;

    // 反序列化需求系统
    _needs.deserialize(buffer + offset);
    offset += NEED_COUNT * 9;

    // 反序列化性格系统
    _personality.deserialize(buffer + offset);
    offset += TRAIT_COUNT;

    // 反序列化状态机
    _state.transitionTo(static_cast<PetState>(buffer[offset]));
    offset += 1;

    // 反序列化表情系统
    _expression.deserialize(buffer + offset);
    offset += 2;

    // 反序列化疲劳系统
    _consecutiveInteractions = buffer[offset];
    offset += 1;
    _fatigued = buffer[offset] == 1;
}
