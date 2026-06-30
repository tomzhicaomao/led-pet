#include "gtest/gtest.h"
#include "../../src/core/need.h"

class NeedTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 初始化需求
        needConfig = {100, 1, 20, 10};
    }

    NeedConfig needConfig;
};

TEST_F(NeedTest, Initialization) {
    Need need(NEED_HUNGER, needConfig);
    EXPECT_EQ(need.value(), 100);
    EXPECT_FALSE(need.isCritical());
    EXPECT_TRUE(need.isSatisfied());
}

TEST_F(NeedTest, Decay) {
    Need need(NEED_HUNGER, needConfig);

    // 模拟 10 分钟衰减
    need.update(600000);
    EXPECT_EQ(need.value(), 99);

    // 模拟 100 分钟衰减
    need.update(6000000);
    EXPECT_EQ(need.value(), 90);
}

TEST_F(NeedTest, CriticalState) {
    Need need(NEED_HUNGER, needConfig);

    // 衰减到危急阈值
    need.update(80 * 60000); // 80 分钟
    EXPECT_EQ(need.value(), 20);
    EXPECT_TRUE(need.isCritical());
}

TEST_F(NeedTest, Recovery) {
    Need need(NEED_HUNGER, {50, 1, 20, 10});

    need.recover(30);
    EXPECT_EQ(need.value(), 80);
}

TEST_F(NeedTest, RecoveryOverflow) {
    Need need(NEED_HUNGER, {90, 1, 20, 10});

    need.recover(20);
    EXPECT_EQ(need.value(), 100); // 不应超过 100
}

TEST_F(NeedTest, SerializeDeserialize) {
    Need need(NEED_HUNGER, needConfig);
    need.update(100000); // 更新一些时间

    uint8_t buffer[9];
    need.serialize(buffer);

    Need need2(NEED_HUNGER, needConfig);
    need2.deserialize(buffer);

    EXPECT_EQ(need2.value(), need.value());
}

class NeedSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        needSystem.begin();
    }

    NeedSystem needSystem;
};

TEST_F(NeedSystemTest, Initialization) {
    EXPECT_EQ(needSystem.getNeed(NEED_HUNGER).value(), 100);
    EXPECT_EQ(needSystem.getNeed(NEED_HAPPINESS).value(), 100);
    EXPECT_EQ(needSystem.getNeed(NEED_ENERGY).value(), 100);
    EXPECT_EQ(needSystem.getNeed(NEED_BOND).value(), 50);
}

TEST_F(NeedSystemTest, Update) {
    needSystem.update(600000); // 10 分钟

    EXPECT_EQ(needSystem.getNeed(NEED_HUNGER).value(), 99);
    EXPECT_EQ(needSystem.getNeed(NEED_HAPPINESS).value(), 100); // 30 分钟衰减一次
}

TEST_F(NeedSystemTest, OverallHealth) {
    uint8_t health = needSystem.getOverallHealth();
    EXPECT_GE(health, 80); // 初始健康度应该较高
}

TEST_F(NeedSystemTest, SerializeDeserialize) {
    needSystem.update(100000);

    uint8_t buffer[NEED_COUNT * 9];
    needSystem.serialize(buffer);

    NeedSystem needSystem2;
    needSystem2.deserialize(buffer);

    EXPECT_EQ(needSystem2.getNeed(NEED_HUNGER).value(), needSystem.getNeed(NEED_HUNGER).value());
    EXPECT_EQ(needSystem2.getOverallHealth(), needSystem.getOverallHealth());
}
