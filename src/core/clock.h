#pragma once
#include <cstdint>

class Clock {
public:
    Clock();

    // 初始化（从 NTP 或 RTC）
    void begin();

    // 获取当前时间（毫秒）
    uint32_t now() const;

    // 获取当前小时（0-23）
    uint8_t hour() const;

    // 时间段判断
    bool isMorning() const;   // 6-12
    bool isAfternoon() const; // 12-18
    bool isEvening() const;   // 18-22
    bool isNight() const;     // 22-6

    // 计算时间差
    uint32_t elapsed(uint32_t since) const;

    // NTP 同步
    void syncNTP();
    bool isNTPValid() const;

    // 设置模拟时间（测试用）
    void setMockTime(uint32_t timeMs);
    void setMockHour(uint8_t hour);

private:
    uint32_t _offset;
    bool _ntpValid;
    uint32_t _mockTime;
    uint8_t _mockHour;
    bool _useMock;
};
