"""Mock ESP32 for offline testing of game logic."""

class MockGameEngine:
    def __init__(self):
        self.state = {
            "points": 0,
            "stage": 0,
            "feedCount": 0,
            "foodBag": [0, 0, 0, 0, 0],
            "lastFeedTime": 0,
            "parentPIN": "0000",
            "currentPet": 0,
        }
        self.history = []

    def get_status(self):
        return {
            "name": "Dragon",
            "stage": self.state["stage"],
            "state": "happy",
            "points": self.state["points"],
            "feedCount": self.state["feedCount"],
        }

    def add_points(self, points, pin):
        if pin != self.state["parentPIN"]:
            return False
        self.state["points"] += points
        self.history.append({"points": points, "reason": "", "ts": 0})
        return True

    def buy_food(self, food_type):
        prices = [3, 5, 8, 12, 20]
        if food_type >= 5:
            return False
        if self.state["points"] < prices[food_type]:
            return False
        if self.state["foodBag"][food_type] >= 99:
            return False
        self.state["points"] -= prices[food_type]
        self.state["foodBag"][food_type] += 1
        return True

    def feed(self, food_type):
        feed_values = [1, 2, 3, 5, 0]
        thresholds = [3, 8, 15, 25]
        if food_type >= 5 or self.state["foodBag"][food_type] <= 0:
            return {"ok": False, "evolved": False, "newStage": self.state["stage"]}

        self.state["foodBag"][food_type] -= 1
        self.state["lastFeedTime"] = 999999

        # Evolution stone
        if food_type == 4:
            if self.state["stage"] < 4 and self.state["feedCount"] >= thresholds[self.state["stage"]]:
                self.state["stage"] += 1
                self.state["feedCount"] = 0
                return {"ok": True, "evolved": True, "newStage": self.state["stage"]}
            else:
                self.state["foodBag"][4] += 1  # return stone
                return {"ok": False, "evolved": False, "newStage": self.state["stage"]}

        # Normal food
        self.state["feedCount"] += feed_values[food_type]
        return {"ok": True, "evolved": False, "newStage": self.state["stage"]}

    def change_pin(self, old_pin, new_pin):
        if old_pin != self.state["parentPIN"]:
            return False
        self.state["parentPIN"] = new_pin
        return True

    def reset(self):
        self.__init__()


class MockESP32:
    def __init__(self):
        self.game = MockGameEngine()

    def get_pet_status(self):
        return self.game.get_status()

    def send_reward(self, points, pin="0000", reason=""):
        return self.game.add_points(points, pin)

    def feed(self, food_type):
        return self.game.feed(food_type)

    def buy(self, food_type):
        return self.game.buy_food(food_type)

    def change_pin(self, old_pin, new_pin):
        return self.game.change_pin(old_pin, new_pin)
