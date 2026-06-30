"""Test reward API: PIN auth, reason tags, history."""

import pytest
from helpers.mock_esp32 import MockESP32


@pytest.fixture
def esp():
    return MockESP32()


class TestRewardAPI:
    def test_send_reward_adds_points(self, esp):
        ok = esp.send_reward(5, "0000")
        assert ok is True
        assert esp.game.state["points"] == 5

    def test_send_reward_wrong_pin_fails(self, esp):
        ok = esp.send_reward(5, "9999")
        assert ok is False
        assert esp.game.state["points"] == 0

    def test_reward_adds_to_history(self, esp):
        esp.send_reward(3, "0000", "reading")
        assert len(esp.game.history) >= 1
        assert esp.game.history[-1]["points"] == 3

    def test_multiple_rewards_accumulate(self, esp):
        esp.send_reward(5, "0000")
        esp.send_reward(3, "0000")
        esp.send_reward(10, "0000")
        assert esp.game.state["points"] == 18

    def test_pin_change_then_reward(self, esp):
        esp.change_pin("0000", "1234")
        ok = esp.send_reward(5, "1234")
        assert ok is True
        ok = esp.send_reward(5, "0000")
        assert ok is False


class TestPINManagement:
    def test_default_pin_is_0000(self, esp):
        assert esp.game.state["parentPIN"] == "0000"

    def test_change_pin_with_correct_old_pin(self, esp):
        ok = esp.change_pin("0000", "5678")
        assert ok is True
        assert esp.game.state["parentPIN"] == "5678"

    def test_change_pin_with_wrong_old_pin(self, esp):
        ok = esp.change_pin("9999", "5678")
        assert ok is False
        assert esp.game.state["parentPIN"] == "0000"

    def test_pin_survives_reset(self, esp):
        esp.change_pin("0000", "4321")
        esp.game.reset()
        assert esp.game.state["parentPIN"] == "0000"


class TestPersistence:
    def test_fresh_state_is_clean(self, esp):
        assert esp.game.state["points"] == 0
        assert esp.game.state["stage"] == 0
        assert esp.game.state["feedCount"] == 0

    def test_reset_clears_all_state(self, esp):
        esp.send_reward(50, "0000")
        esp.buy(0)
        esp.feed(0)
        esp.game.reset()
        assert esp.game.state["points"] == 0
        assert esp.game.state["foodBag"] == [0, 0, 0, 0, 0]
