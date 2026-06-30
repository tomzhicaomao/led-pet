#include "gtest/gtest.h"
#include "../../src/pet/pet_state.h"

class StateTest : public ::testing::Test {
protected:
    void SetUp() override {
        stateMachine = PetStateMachine();
    }

    PetStateMachine stateMachine;
};

TEST_F(StateTest, Initialization) {
    EXPECT_EQ(stateMachine.currentState(), STATE_IDLE);
    EXPECT_EQ(stateMachine.previousState(), STATE_IDLE);
    EXPECT_EQ(stateMachine.stateEnterTime(), 0);
}

TEST_F(StateTest, TransitionTo) {
    stateMachine.transitionTo(STATE_HUNGRY, 1000);
    EXPECT_EQ(stateMachine.currentState(), STATE_HUNGRY);
    EXPECT_EQ(stateMachine.previousState(), STATE_IDLE);
    EXPECT_EQ(stateMachine.stateEnterTime(), 1000);
}

TEST_F(StateTest, TransitionToSameState) {
    stateMachine.transitionTo(STATE_IDLE, 1000);
    stateMachine.transitionTo(STATE_IDLE, 2000);
    EXPECT_EQ(stateMachine.currentState(), STATE_IDLE);
    EXPECT_EQ(stateMachine.stateEnterTime(), 1000); // 应该保持第一次的时间
}

TEST_F(StateTest, StateDuration) {
    stateMachine.transitionTo(STATE_EATING, 1000);
    EXPECT_EQ(stateMachine.stateDuration(5000), 4000);
}

TEST_F(StateTest, StateName) {
    EXPECT_STREQ(stateMachine.stateName(), "Idle");
    EXPECT_STREQ(stateMachine.stateName(STATE_HUNGRY), "Hungry");
    EXPECT_STREQ(stateMachine.stateName(STATE_HAPPY), "Happy");
    EXPECT_STREQ(stateMachine.stateName(STATE_EATING), "Eating");
    EXPECT_STREQ(stateMachine.stateName(STATE_PLAYING), "Playing");
    EXPECT_STREQ(stateMachine.stateName(STATE_SLEEPING), "Sleeping");
    EXPECT_STREQ(stateMachine.stateName(STATE_DISTRESSED), "Distressed");
    EXPECT_STREQ(stateMachine.stateName(STATE_CURIOUS), "Curious");
    EXPECT_STREQ(stateMachine.stateName(STATE_EVOLVING), "Evolving");
    EXPECT_STREQ(stateMachine.stateName(STATE_BRANCHING), "Branching");
    EXPECT_STREQ(stateMachine.stateName(STATE_COUNT), "Unknown");
}
