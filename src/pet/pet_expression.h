#pragma once
#include <cstdint>

// 表情类型
enum ExpressionType : uint8_t {
    EXPR_NONE = 0,      // 无表情
    EXPR_HUNGRY = 1,    // 饥饿（食物气泡）
    EXPR_HAPPY = 2,     // 快乐（爱心）
    EXPR_TIRED = 3,     // 困倦（zzz）
    EXPR_LONELY = 4,    // 孤独（思念气泡）
    EXPR_EXCITED = 5,   // 兴奋（跳跃）
    EXPR_SAD = 6,       // 悲伤（泪水）
    EXPR_FATIGUED = 7,  // 疲劳（困倦）
    EXPR_COUNT = 8
};

// 表情系统
class ExpressionSystem {
public:
    ExpressionSystem();

    // 更新表情
    void update(ExpressionType type, uint8_t intensity = 100);

    // 获取当前表情
    ExpressionType current() const { return _current; }

    // 表情强度（0-100）
    uint8_t intensity() const { return _intensity; }

    // 表情持续时间
    uint32_t duration(uint32_t nowMs) const;

    // 表情是否活跃
    bool isActive() const { return _current != EXPR_NONE; }

    // 获取表情名称
    const char* name() const;
    const char* name(ExpressionType type) const;

    // 序列化
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer);

private:
    ExpressionType _current;
    uint8_t _intensity;
    uint32_t _startTime;
};
