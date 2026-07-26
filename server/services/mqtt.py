from config import *
from typing import Callable

import paho.mqtt.client as mqtt
from services.parser import parse_payload, validate_payload
from services.writer import FileWriter

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
        self._client.connect(self._broker, self._port)

    def maintain(self) -> None:
        self._client.loop_forever()

    def _on_connect(self, client, userdata, flags, reason_code, properties:None):
        if reason_code == mqtt.ReasonCodes.SUCCESS:
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
        try:
            validate_payload(payload.strip().split(","))
            record = parse_payload(payload)

        except Exception as e:
            logger.error(f"Error while processing payload on topic {topic}: {str(e)}")
            return
        try:
            writer.append(record)
        except OSError as e:
            logger.error(f"Error writing record to file: {str(e)}")

        logger.info(f"Processed message on topic {topic}: {record}")