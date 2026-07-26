import functools

from logger import*
from mqtt import *
from parser import *
from writer import *
import config

def main() -> None:
    setup_logging(config.LOG_LEVEL)
 
    writer = FileWriter(config.OUTPUT_DIR)
    client = MQTTClient(config.MQTT_BROKER, config.MQTT_PORT, config.MQTT_TOPICS)
 
    handler = functools.partial(client.process_message, writer=writer)
    client.set_message_handler(handler)
 
    try:
        client.setup()
        client.maintain()
    except KeyboardInterrupt:
        logger.info("encerrando servidor...")
    finally:
        writer.close()
 
 
if __name__ == "__main__":
    main()
