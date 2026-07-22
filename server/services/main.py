
import logging

from config import config
from mqtt_subscriber import PersistenceSubscriber
from writter import *


def main() -> None:
    logging.basicConfig(
        level=config.log_level,
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
    )

    writer = CsvWriter(data_dir=config.data_dir)
    subscriber = PersistenceSubscriber(config=config, writer=writer)
    subscriber.run_forever()


if __name__ == "__main__":
    main()