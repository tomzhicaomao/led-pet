import requests

class ESP32Client:
    def __init__(self, host: str, port: int = 80):
        self.base_url = f"http://{host}:{port}"

    def get(self, path): return requests.get(f"{self.base_url}{path}", timeout=5).json()
    def post(self, path, data=None): return requests.post(f"{self.base_url}{path}", json=data, timeout=5).json()
    def get_pet_status(self): return self.get("/api/pet")
    def feed_pet(self, ft=0): return self.post("/api/pet/feed", {"foodType": ft})
    def add_points(self, pts=5, pin="0000"): return self.post("/api/points/add", {"points": pts, "pin": pin})
    def buy_item(self, iid): return self.post("/api/shop/buy", {"itemId": iid})
    def get_shop_items(self): return self.get("/api/shop")
    def get_pet_bag(self): return self.get("/api/pet/bag")
    def get_stages(self): return self.get("/api/pet/stages")
