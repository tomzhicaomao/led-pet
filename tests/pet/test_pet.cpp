#include "gtest/gtest.h"
#include "../../src/pet/pet.h"

class PetTest : public ::testing::Test {
protected:
    void SetUp() override {
        pet.begin();
    }

    Pet pet;
};

TEST_F(PetTest, Initialization) {
    EXPECT_EQ(pet.getState(), STATE_IDLE);
    EXPECT_EQ(pet.getNeedValue(NEED_HUNGER), 100);
    EXPECT_EQ(pet.getNeedValue(NEED_HAPPINESS), 100);
    EXPECT_EQ(pet.getNeedValue(NEED_ENERGY), 100);
    EXPECT_EQ(pet.getNeedValue(NEED_BOND), 50);
    EXPECT_FALSE(pet.isFatigued());
}

TEST_F(PetTest, Feed) {
    pet.feed();
    EXPECT_EQ(pet.getState(), STATE_EATING);
    EXPECT_GT(pet.getNeedValue(NEED_HUNGER), 100);
}

TEST_F(PetTest, Pet) {
    pet.pet();
    EXPECT_EQ(pet.getState(), STATE_HAPPY);
    EXPECT_GT(pet.getNeedValue(NEED_BOND), 50);
}

TEST_F(PetTest, Play) {
    pet.play();
    EXPECT_EQ(pet.getState(), STATE_PLAYING);
    EXPECT_GT(pet.getNeedValue(NEED_HAPPINESS), 100);
    EXPECT_LT(pet.getNeedValue(NEED_ENERGY), 100);
}

TEST_F(PetTest, Rest) {
    pet.rest();
    EXPECT_EQ(pet.getState(), STATE_SLEEPING);
    EXPECT_GT(pet.getNeedValue(NEED_ENERGY), 100);
}

TEST_F(PetTest, Fatigue) {
    // 连续互动 5 次
    for (int i = 0; i < 5; i++) {
        pet.play();
    }

    EXPECT_TRUE(pet.isFatigued());
    EXPECT_EQ(pet.getConsecutiveInteractions(), 5);
}

TEST_F(PetTest, FatiguePreventsInteraction) {
    // 触发疲劳
    for (int i = 0; i < 5; i++) {
        pet.play();
    }

    // 尝试继续互动
    uint8_t happinessBefore = pet.getNeedValue(NEED_HAPPINESS);
    pet.feed();
    EXPECT_EQ(pet.getNeedValue(NEED_HAPPINESS), happinessBefore); // 应该没有变化
}

TEST_F(PetTest, ExpressionUpdate) {
    // 初始状态
    EXPECT_EQ(pet.getCurrentExpression(), EXPR_NONE);

    // 饥饿时显示饥饿表情
    pet.update(80 * 60000); // 80 分钟
    EXPECT_EQ(pet.getCurrentExpression(), EXPR_HUNGRY);
}

TEST_F(PetTest, SerializeDeserialize) {
    pet.feed();
    pet.pet();

    uint8_t buffer[256];
    pet.serialize(buffer);

    Pet pet2;
    pet2.deserialize(buffer);

    EXPECT_EQ(pet2.getState(), pet.getState());
    EXPECT_EQ(pet2.getNeedValue(NEED_HUNGER), pet.getNeedValue(NEED_HUNGER));
    EXPECT_EQ(pet2.getNeedValue(NEED_BOND), pet.getNeedValue(NEED_BOND));
}
