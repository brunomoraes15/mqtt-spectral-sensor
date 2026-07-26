import os
import logging

MQTT_BROKER = os.getenv("MQTT_BROKER", "localhost")
MQTT_PORT = int(os.getenv("MQTT_PORT", "1883"))
MQTT_TOPICS = ["sensor/data", "sensor/command", "sensor/config"]
MQTT_DATA_TOPIC = "sensor/data"
OUTPUT_DIR = os.getenv("OUTPUT_DIR", "sensor/data")
LOG_LEVEL = os.getenv("LOG_LEVEL", "INFO")

logger = logging.getLogger(__name__)

PAYLOAD_FORMAT = {
    "timestamp": int,
    "sensor_id": int,
    "mac_address": str,
    "again": float,
    "astep": float,
    "int_time": int,

    "f1": float,
    "f2": float,
    "f3": float,
    "f4": float,
    "f5": float,
    "f6": float,
    "f7": float,
    "f8": float,

    "clear": float,
    "nir": float
}

PAYLOAD_SIZE = len(PAYLOAD_FORMAT)
