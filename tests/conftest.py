import pytest
import os

def pytest_addoption(parser):
    parser.addoption("--esp32-host", default=os.getenv("ESP32_HOST", "192.168.4.1"))
    parser.addoption("--mock", action="store_true", default=True)

@pytest.fixture
def esp32_host(request):
    return request.config.getoption("--esp32-host")

@pytest.fixture
def is_mock(request):
    return request.config.getoption("--mock")

@pytest.fixture
def client(request, is_mock):
    from helpers.mock_esp32 import MockESP32
    return MockESP32()
