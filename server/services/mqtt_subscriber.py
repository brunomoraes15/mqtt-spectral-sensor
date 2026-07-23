import logging
from datetime import datetime, timezone

import paho.mqtt.client as mqtt

from config import Config
from parser import *
from writter import *

logger = logging.getLogger(__name__)


class PersistenceSubscriber:
    def __init__(self, config: Config, writer: CsvWriter):
        self._config = config
        self._writer = writer
        self._client = mqtt.Client(client_id=config.mqtt_client_id)
        self._client.on_connect = self._on_connect
        self._client.on_message = self._on_message
        self._client.on_disconnect = self._on_disconnect

    def run_forever(self) -> None:
        logger.info(
            "connecting to MQTT broker %s:%s",
            self._config.MQTT_BROKER,
            self._config.MQTT_PORT,
        )
        self._client.connect(
            self._config.MQTT_BROKER,
            self._config.MQTT_PORT,
            keepalive=self._config.MQTT_KEEPALIVE,
        )
        self._client.loop_forever()

    def _on_connect(self, client, userdata, flags, rc):
        if rc != 0:
            logger.error("MQTT connect failed with code %s", rc)
            return
        logger.info("connected, subscribing to '%s'", self._config.MQTT_TOPIC)
        client.subscribe(self._config.MQTT_TOPIC)

    def _on_disconnect(self, client, userdata, rc):
        logger.warning("disconnected from MQTT broker (code %s)", rc)

    def _on_message(self, client, userdata, msg):
        receipt_time = datetime.now(timezone.utc)
        try:
            payload = msg.payload.decode("utf-8", errors="replace")
        except Exception as exc:  # defensive: never let decoding kill the loop
            logger.warning("failed to decode payload on topic %s: %s", msg.topic, exc)
            return

        result = validate_payload(payload)
        if not result.is_valid:
            logger.warning(
                "rejected malformed payload on topic %s: %s (payload=%r)",
                msg.topic,
                result.reason,
                payload,
            )
            return

        try:
            record = parse_fields(result.fields)
            self._writer.write(record, receipt_time)
        except Exception as exc:
            # Catch-all by design: a single bad message must never stop the
            # subscriber loop (requirement: handle malformed messages
            # without crashing the application).
            logger.error(
                "failed to process message on topic %s: %s", msg.topic, exc
            )