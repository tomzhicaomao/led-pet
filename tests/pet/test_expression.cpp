#include "gtest/gtest.h"
#include "../../src/pet/pet_expression.h"

class ExpressionTest : public ::testing::Test {
protected:
    void SetUp() override {
        expression = ExpressionSystem();
    }

    ExpressionSystem expression;
};

TEST_F(ExpressionTest, Initialization) {
    EXPECT_EQ(expression.current(), EXPR_NONE);
    EXPECT_EQ(expression.intensity(), 0);
    EXPECT_FALSE(expression.isActive());
}

TEST_F(ExpressionTest, Update) {
    expression.update(EXPR_HUNGRY, 100);
    EXPECT_EQ(expression.current(), EXPR_HUNGRY);
    EXPECT_EQ(expression.intensity(), 100);
    EXPECT_TRUE(expression.isActive());
}

TEST_F(ExpressionTest, UpdateSameExpression) {
    expression.update(EXPR_HAPPY, 50);
    expression.update(EXPR_HAPPY, 80);
    EXPECT_EQ(expression.current(), EXPR_HAPPY);
    EXPECT_EQ(expression.intensity(), 80);
}

TEST_F(ExpressionTest, Name) {
    EXPECT_STREQ(expression.name(), "None");
    EXPECT_STREQ(expression.name(EXPR_HUNGRY), "Hungry");
    EXPECT_STREQ(expression.name(EXPR_HAPPY), "Happy");
    EXPECT_STREQ(expression.name(EXPR_TIRED), "Tired");
    EXPECT_STREQ(expression.name(EXPR_LONELY), "Lonely");
    EXPECT_STREQ(expression.name(EXPR_EXCITED), "Excited");
    EXPECT_STREQ(expression.name(EXPR_SAD), "Sad");
    EXPECT_STREQ(expression.name(EXPR_FATIGUED), "Fatigued");
    EXPECT_STREQ(expression.name(EXPR_COUNT), "Unknown");
}

TEST_F(ExpressionTest, SerializeDeserialize) {
    expression.update(EXPR_EXCITED, 75);

    uint8_t buffer[2];
    expression.serialize(buffer);

    ExpressionSystem expression2;
    expression2.deserialize(buffer);

    EXPECT_EQ(expression2.current(), EXPR_EXCITED);
    EXPECT_EQ(expression2.intensity(), 75);
}

TEST_F(ExpressionTest, DefaultIntensity) {
    expression.update(EXPR_HAPPY);
    EXPECT_EQ(expression.intensity(), 100); // 默认强度为 100
}
