import pytest


class TestPersistence:
    def test_mock_reset_clears_state(self, client, is_mock):
        if not is_mock: pytest.skip("Mock only")
        s = client.get_pet_status()
        assert s["points"] == 0 and s["stage"] == 0
