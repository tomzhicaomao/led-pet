"""Test core game logic: feeding, evolution, points, PIN protection."""

import pytest
from helpers.mock_esp32 import MockESP32


@pytest.fixture
def esp():
    return MockESP32()


class TestFeeding:
    def test_feed_normal_food_increases_feed_count(self, esp):
        esp.game.state["foodBag"][0] = 5
        r = esp.feed(0)
        assert r["ok"] is True
        assert esp.game.state["feedCount"] == 1  # apple = 1 feed value

    def test_feed_reduces_bag_count(self, esp):
        esp.game.state["foodBag"][0] = 3
        esp.feed(0)
        assert esp.game.state["foodBag"][0] == 2

    def test_feed_with_empty_bag_fails(self, esp):
        r = esp.feed(0)
        assert r["ok"] is False

    def test_feed_drumstick_gives_2_feed_value(self, esp):
        esp.game.state["foodBag"][1] = 1
        esp.feed(1)
        assert esp.game.state["feedCount"] == 2

    def test_feed_cake_gives_3_feed_value(self, esp):
        esp.game.state["foodBag"][2] = 1
        esp.feed(2)
        assert esp.game.state["feedCount"] == 3

    def test_feed_star_gives_5_feed_value(self, esp):
        esp.game.state["foodBag"][3] = 1
        esp.feed(3)
        assert esp.game.state["feedCount"] == 5

    def test_feed_invalid_type_fails(self, esp):
        r = esp.feed(99)
        assert r["ok"] is False


class TestEvolution:
    def test_stage0_to_1_requires_3_feed_count(self, esp):
        esp.game.state["feedCount"] = 3
        esp.game.state["foodBag"][4] = 1
        r = esp.feed(4)  # evolution stone
        assert r["evolved"] is True
        assert r["newStage"] == 1
        assert esp.game.state["feedCount"] == 0

    def test_evolution_fails_if_feed_count_insufficient(self, esp):
        esp.game.state["feedCount"] = 2
        esp.game.state["foodBag"][4] = 1
        r = esp.feed(4)
        assert r["evolved"] is False
        assert esp.game.state["stage"] == 0
        # stone should be returned
        assert esp.game.state["foodBag"][4] == 1

    def test_stage1_to_2_requires_8_feed_count(self, esp):
        esp.game.state["stage"] = 1
        esp.game.state["feedCount"] = 8
        esp.game.state["foodBag"][4] = 1
        r = esp.feed(4)
        assert r["evolved"] is True
        assert r["newStage"] == 2

    def test_stage2_to_3_requires_15_feed_count(self, esp):
        esp.game.state["stage"] = 2
        esp.game.state["feedCount"] = 15
        esp.game.state["foodBag"][4] = 1
        r = esp.feed(4)
        assert r["evolved"] is True
        assert r["newStage"] == 3

    def test_stage3_to_4_requires_25_feed_count(self, esp):
        esp.game.state["stage"] = 3
        esp.game.state["feedCount"] = 25
        esp.game.state["foodBag"][4] = 1
        r = esp.feed(4)
        assert r["evolved"] is True
        assert r["newStage"] == 4

    def test_final_stage_cannot_evolve(self, esp):
        esp.game.state["stage"] = 4
        esp.game.state["feedCount"] = 99
        esp.game.state["foodBag"][4] = 1
        r = esp.feed(4)
        assert r["evolved"] is False


class TestShop:
    def test_buy_food_deducts_points(self, esp):
        esp.game.state["points"] = 100
        ok = esp.buy(0)  # apple = 3 points
        assert ok is True
        assert esp.game.state["points"] == 97

    def test_buy_food_increases_inventory(self, esp):
        esp.game.state["points"] = 100
        esp.buy(0)
        assert esp.game.state["foodBag"][0] == 1

    def test_buy_food_insufficient_points_fails(self, esp):
        esp.game.state["points"] = 0
        ok = esp.buy(0)
        assert ok is False
        assert esp.game.state["foodBag"][0] == 0

    def test_buy_food_stack_limit_99(self, esp):
        esp.game.state["points"] = 1000
        esp.game.state["foodBag"][0] = 99
        ok = esp.buy(0)
        assert ok is False

    def test_buy_food_invalid_type_fails(self, esp):
        esp.game.state["points"] = 100
        ok = esp.buy(99)
        assert ok is False


class TestPoints:
    def test_add_points_with_correct_pin(self, esp):
        ok = esp.send_reward(5, "0000")
        assert ok is True
        assert esp.game.state["points"] == 5

    def test_add_points_with_wrong_pin_fails(self, esp):
        ok = esp.send_reward(5, "9999")
        assert ok is False
        assert esp.game.state["points"] == 0

    def test_add_points_with_custom_pin(self, esp):
        esp.game.state["parentPIN"] = "1234"
        ok = esp.send_reward(10, "1234")
        assert ok is True
        assert esp.game.state["points"] == 10

    def test_change_pin_succeeds(self, esp):
        ok = esp.change_pin("0000", "5678")
        assert ok is True
        assert esp.game.state["parentPIN"] == "5678"

    def test_change_pin_wrong_old_pin_fails(self, esp):
        ok = esp.change_pin("9999", "5678")
        assert ok is False
        assert esp.game.state["parentPIN"] == "0000"
