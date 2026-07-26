import time
from typing import Callable

import paho.mqtt.client as mqtt

from config import *
from parser import *
from writer import FileWriter

MessageHandler = Callable[[str, str], None]

class MQTTClient:
    def __init__(self, broker: str, port: int, topics: list[str]):
        self._broker = broker
        self._port = port
        self._topics = topics
        self._handler: MessageHandler | None = None

        self._client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
        self._client.on_connect = self._on_connect
        self._client.on_disconnect = self._on_disconnect
        self._client.on_message = self._on_message

    def set_message_handler(self, handler: MessageHandler) -> None:
        self._handler = handler

    def setup(self)->None:
        logger.info("Connecting to MQTT broker %s:%d", self._broker, self._port)

        max_attempts = 5
        backoff_seconds = 2
        for attempt in range(1, max_attempts + 1):
            try:
                self._client.connect(self._broker, self._port)
                return
            except OSError as e:
                logger.warning(
                    "Failed to connect to MQTT broker %s:%d (attempt %d/%d): %s",
                    self._broker, self._port, attempt, max_attempts, e,
                )
                if attempt == max_attempts:
                    raise
                time.sleep(backoff_seconds)
                backoff_seconds *= 2

    def disconnect(self) -> None:
        self._client.disconnect()

    def maintain(self) -> None:
        self._client.loop_forever()

    def _on_connect(self, client, userdata, flags, reason_code, properties:None):
        if reason_code == 0:
            logger.info("Connected to MQTT broker %s:%d", self._broker, self._port)
            for topic in self._topics:
                client.subscribe(topic)
                logger.info("Subscribed to topic: %s", topic)
        else:
            logger.warning("Failed to connect to MQTT broker %s:%d, reason: %s", self._broker, self._port, reason_code)
            return

    def _on_disconnect(self, client, userdata, reason_code, properties:None):
        logger.warning("Disconnected from MQTT broker %s:%d, reason: %s", self._broker, self._port, reason_code)


    def _on_message(self, client, userdata, message):
        if self._handler:
            payload = message.payload.decode("utf-8", errors="replace")
            self._handler(message.topic, payload)
        else:
            logger.warning("No message handler set")
            return

    def process_message(self, topic: str, payload: str, writer: FileWriter) -> None:
        if topic != MQTT_DATA_TOPIC:
            logger.debug("Ignoring message on non-data topic %s", topic)
            return

        try:
            record = parse_payload(payload)
        except PayloadValidationError as e:
            logger.error("Invalid payload on topic %s: %s", topic, e)
            return

        try:
            writer.append(record)
        except Exception:
            logger.exception("Error writing record to file (topic %s)", topic)
            return

        logger.info("Processed message on topic %s: %s", topic, record)