import os
from dataclasses import dataclass


@dataclass(frozen=True)
class Config:
    MQTT_BROKER:         str = os.environ.get("MQTT_BROKER", "localhost")
    MQTT_PORT:          int = int(os.environ.get("MQTT_PORT", "1883"))
    MQTT_TOPIC:         str = os.environ.get("MQTT_TOPIC", "sensor/data")
    MQTT_KEEPALIVE:     int = int(os.environ.get("MQTT_KEEPALIVE", "60"))

    data_dir: str = os.environ.get("DATA_DIR", "./server/data")
    log_level: str = os.environ.get("LOG_LEVEL", "INFO")


config = Config()

