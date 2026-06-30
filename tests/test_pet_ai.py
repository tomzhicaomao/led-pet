"""Test pet AI state machine logic."""

import pytest
from helpers.mock_esp32 import MockESP32


class MockPetAI:
    """Mirrors the C++ PetAI state machine logic in Python for testing."""

    HUNGER_MS = 28800000  # 8 hours in ms
    SLEEP_START = 21
    SLEEP_END = 7

    def __init__(self):
        self.state = "IDLE"
        self.sub_state = "ROAMING"
        self.last_feed_time = 0
        self.queued_reward = False
        self.evolved = False

    def is_hungry(self, now_ms):
        if self.last_feed_time == 0:
            return True
        return (now_ms - self.last_feed_time) >= self.HUNGER_MS

    def is_sleep_time(self, hour):
        return hour >= self.SLEEP_START or hour < self.SLEEP_END

    def update(self, now_ms, hour, ntp_valid):
        # Process queued reward
        if self.queued_reward and self.state != "SLEEPING":
            self.queued_reward = False
            self.state = "HAPPY"

        if self.state == "IDLE":
            if self.is_hungry(now_ms):
                self.state = "HUNGRY"
            elif ntp_valid and self.is_sleep_time(hour):
                self.state = "SLEEPING"
        elif self.state == "HUNGRY":
            if ntp_valid and self.is_sleep_time(hour):
                self.state = "SLEEPING"
            elif not self.is_hungry(now_ms):
                self.state = "IDLE"
        elif self.state == "SLEEPING":
            if ntp_valid and not self.is_sleep_time(hour):
                self.state = "IDLE"

    def on_feed(self):
        self.last_feed_time = 999999999
        self.state = "EATING"

    def on_reward(self):
        if self.state == "SLEEPING":
            self.queued_reward = True
        else:
            self.state = "HAPPY"

    def on_evolve(self):
        self.state = "EVOLVING"
        self.evolved = True


@pytest.fixture
def ai():
    return MockPetAI()


class TestStateTransitions:
    def test_starts_in_idle(self, ai):
        assert ai.state == "IDLE"

    def test_transitions_to_hungry_when_never_fed(self, ai):
        ai.update(1000, 12, True)
        assert ai.state == "HUNGRY"

    def test_transitions_to_hungry_after_8_hours(self, ai):
        ai.last_feed_time = 1000
        ai.update(ai.HUNGER_MS + 2000, 12, True)
        assert ai.state == "HUNGRY"

    def test_stays_idle_if_recently_fed(self, ai):
        ai.last_feed_time = 1000
        ai.update(2000, 12, True)
        assert ai.state == "IDLE"

    def test_transitions_to_sleeping_at_night(self, ai):
        ai.last_feed_time = 1000
        ai.update(2000, 22, True)
        assert ai.state == "SLEEPING"

    def test_wakes_up_at_7am(self, ai):
        ai.state = "SLEEPING"
        ai.update(2000, 7, True)
        assert ai.state == "IDLE"

    def test_no_sleep_without_ntp(self, ai):
        ai.last_feed_time = 1000
        ai.update(2000, 22, False)
        assert ai.state == "IDLE"  # No NTP = no sleep


class TestFeeding:
    def test_feed_triggers_eating(self, ai):
        ai.on_feed()
        assert ai.state == "EATING"

    def test_feed_updates_last_feed_time(self, ai):
        ai.on_feed()
        assert ai.last_feed_time > 0


class TestReward:
    def test_reward_during_idle_triggers_happy(self, ai):
        ai.on_reward()
        assert ai.state == "HAPPY"

    def test_reward_during_sleep_is_queued(self, ai):
        ai.state = "SLEEPING"
        ai.on_reward()
        assert ai.queued_reward is True
        assert ai.state == "SLEEPING"

    def test_queued_reward_plays_after_wake(self, ai):
        ai.state = "SLEEPING"
        ai.on_reward()
        # Simulate waking
        ai.state = "IDLE"
        ai.update(2000, 8, True)
        assert ai.state == "HAPPY"
        assert ai.queued_reward is False


class TestEvolution:
    def test_evolve_sets_evolving_state(self, ai):
        ai.on_evolve()
        assert ai.state == "EVOLVING"
        assert ai.evolved is True
