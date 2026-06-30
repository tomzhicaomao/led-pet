#include "gtest/gtest.h"
#include "../../src/core/clock.h"

class ClockTest : public ::testing::Test {
protected:
    void SetUp() override {
        clock.begin();
    }

    Clock clock;
};

TEST_F(ClockTest, Initialization) {
    EXPECT_GT(clock.now(), 0);
    EXPECT_EQ(clock.hour(), 12);
}

TEST_F(ClockTest, TimePeriods) {
    clock.setMockHour(8);
    EXPECT_TRUE(clock.isMorning());
    EXPECT_FALSE(clock.isAfternoon());
    EXPECT_FALSE(clock.isEvening());
    EXPECT_FALSE(clock.isNight());

    clock.setMockHour(14);
    EXPECT_FALSE(clock.isMorning());
    EXPECT_TRUE(clock.isAfternoon());

    clock.setMockHour(20);
    EXPECT_TRUE(clock.isEvening());

    clock.setMockHour(23);
    EXPECT_TRUE(clock.isNight());
}

TEST_F(ClockTest, Elapsed) {
    clock.setMockTime(1000);
    uint32_t since = clock.now();
    clock.setMockTime(5000);
    EXPECT_EQ(clock.elapsed(since), 4000);
}

TEST_F(ClockTest, NTPSync) {
    EXPECT_FALSE(clock.isNTPValid());
    clock.syncNTP();
    EXPECT_TRUE(clock.isNTPValid());
}
