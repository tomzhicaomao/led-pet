#include "clock.h"
#ifdef ARDUINO
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#endif

Clock::Clock()
    : _offset(0)
    , _ntpValid(false)
    , _mockTime(0)
    , _mockHour(12)
    , _useMock(false)
{
}

void Clock::begin() {
#ifdef ARDUINO
    // 在实际硬件上，这里会初始化 NTP 客户端
    // 为简化，我们使用模拟时间
#endif
    _useMock = true;
    _mockTime = 1000000; // 初始时间 1000 秒
    _mockHour = 12;      // 默认中午
}

uint32_t Clock::now() const {
#ifdef ARDUINO
    if (!_useMock) {
        return millis() + _offset;
    }
#endif
    return _mockTime;
}

uint8_t Clock::hour() const {
#ifdef ARDUINO
    if (!_useMock && _ntpValid) {
        // 在实际硬件上，这里会从 NTP 获取小时
        return 12; // 默认中午
    }
#endif
    return _mockHour;
}

bool Clock::isMorning() const {
    uint8_t h = hour();
    return (h >= 6 && h < 12);
}

bool Clock::isAfternoon() const {
    uint8_t h = hour();
    return (h >= 12 && h < 18);
}

bool Clock::isEvening() const {
    uint8_t h = hour();
    return (h >= 18 && h < 22);
}

bool Clock::isNight() const {
    uint8_t h = hour();
    return (h >= 22 || h < 6);
}

uint32_t Clock::elapsed(uint32_t since) const {
    uint32_t current = now();
    if (current >= since) {
        return current - since;
    }
    // 处理溢出
    return (UINT32_MAX - since) + current + 1;
}

void Clock::syncNTP() {
#ifdef ARDUINO
    // 在实际硬件上，这里会同步 NTP 时间
    _ntpValid = true;
#else
    _ntpValid = true;
#endif
}

bool Clock::isNTPValid() const {
    return _ntpValid;
}

void Clock::setMockTime(uint32_t timeMs) {
    _mockTime = timeMs;
    _useMock = true;
}

void Clock::setMockHour(uint8_t hour) {
    _mockHour = hour % 24;
    _useMock = true;
}
