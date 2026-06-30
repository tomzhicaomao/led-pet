#include "gtest/gtest.h"
#include "../../src/core/personality.h"

class PersonalityTest : public ::testing::Test {
protected:
    void SetUp() override {
        personality = Personality();
    }

    Personality personality;
};

TEST_F(PersonalityTest, Initialization) {
    EXPECT_EQ(personality.getTrait(TRAIT_PLAYFUL), 50);
    EXPECT_EQ(personality.getTrait(TRAIT_SHY), 50);
    EXPECT_EQ(personality.getTrait(TRAIT_CURIOUS), 50);
    EXPECT_EQ(personality.getTrait(TRAIT_LAZY), 50);
    EXPECT_EQ(personality.getTrait(TRAIT_BRAVE), 50);
    EXPECT_EQ(personality.getTrait(TRAIT_GENTLE), 50);
}

TEST_F(PersonalityTest, UpdateTrait) {
    personality.updateTrait(TRAIT_PLAYFUL, 10);
    EXPECT_EQ(personality.getTrait(TRAIT_PLAYFUL), 60);

    personality.updateTrait(TRAIT_PLAYFUL, -20);
    EXPECT_EQ(personality.getTrait(TRAIT_PLAYFUL), 40);
}

TEST_F(PersonalityTest, TraitBounds) {
    // 测试最小值
    personality.updateTrait(TRAIT_PLAYFUL, -100);
    EXPECT_EQ(personality.getTrait(TRAIT_PLAYFUL), 0);

    // 测试最大值
    personality.updateTrait(TRAIT_SHY, 100);
    EXPECT_EQ(personality.getTrait(TRAIT_SHY), 100);
}

TEST_F(PersonalityTest, DevelopFromInteraction) {
    // 多次玩耍 → 爱玩+
    personality.developFromInteraction(INTERACT_PLAY);
    personality.developFromInteraction(INTERACT_PLAY);
    personality.developFromInteraction(INTERACT_PLAY);

    EXPECT_GT(personality.getTrait(TRAIT_PLAYFUL), 50);

    // 多次休息 → 懒惰+
    Personality personality2;
    personality2.developFromInteraction(INTERACT_REST);
    personality2.developFromInteraction(INTERACT_REST);
    personality2.developFromInteraction(INTERACT_REST);

    EXPECT_GT(personality2.getTrait(TRAIT_LAZY), 50);
}

TEST_F(PersonalityTest, GetDominantTrait) {
    // 初始状态，所有特质相等
    EXPECT_EQ(personality.getDominantTrait(), TRAIT_PLAYFUL);

    // 增加温柔特质
    personality.updateTrait(TRAIT_GENTLE, 30);
    EXPECT_EQ(personality.getDominantTrait(), TRAIT_GENTLE);
}

TEST_F(PersonalityTest, GetTraitName) {
    EXPECT_STREQ(personality.getTraitName(TRAIT_PLAYFUL), "Playful");
    EXPECT_STREQ(personality.getTraitName(TRAIT_SHY), "Shy");
    EXPECT_STREQ(personality.getTraitName(TRAIT_CURIOUS), "Curious");
    EXPECT_STREQ(personality.getTraitName(TRAIT_LAZY), "Lazy");
    EXPECT_STREQ(personality.getTraitName(TRAIT_BRAVE), "Brave");
    EXPECT_STREQ(personality.getTraitName(TRAIT_GENTLE), "Gentle");
    EXPECT_STREQ(personality.getTraitName(TRAIT_COUNT), "Unknown");
}

TEST_F(PersonalityTest, SerializeDeserialize) {
    personality.updateTrait(TRAIT_PLAYFUL, 20);
    personality.updateTrait(TRAIT_SHY, -10);

    uint8_t buffer[TRAIT_COUNT];
    personality.serialize(buffer);

    Personality personality2;
    personality2.deserialize(buffer);

    EXPECT_EQ(personality2.getTrait(TRAIT_PLAYFUL), 70);
    EXPECT_EQ(personality2.getTrait(TRAIT_SHY), 40);
}

TEST_F(PersonalityTest, IntensityEffect) {
    Personality p1;
    p1.developFromInteraction(INTERACT_PLAY, 1);

    Personality p2;
    p2.developFromInteraction(INTERACT_PLAY, 3);

    EXPECT_GT(p2.getTrait(TRAIT_PLAYFUL), p1.getTrait(TRAIT_PLAYFUL));
}
