import functools
import signal

import config
from logger import setup_logging
from mqtt import MQTTClient
from writer import FileWriter


def _handle_sigterm(client: MQTTClient, signum, frame) -> None:
    config.logger.info("Received SIGTERM, shutting down...")
    client.disconnect()


def main() -> None:
    setup_logging(config.LOG_LEVEL)

    writer = FileWriter(config.OUTPUT_DIR)
    client = MQTTClient(config.MQTT_BROKER, config.MQTT_PORT, config.MQTT_TOPICS)

    handler = functools.partial(client.process_message, writer=writer)
    client.set_message_handler(handler)

    signal.signal(signal.SIGTERM, functools.partial(_handle_sigterm, client))

    try:
        client.setup()
        client.maintain()
    except KeyboardInterrupt:
        config.logger.info("Shutting down server...")
    except Exception:
        config.logger.exception("Unexpected error, shutting down server")
    finally:
        writer.close()


if __name__ == "__main__":
    main()