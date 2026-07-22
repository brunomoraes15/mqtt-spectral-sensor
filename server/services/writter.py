from datetime import datetime
from pathlib import Path
import csv
import logging
from datetime import datetime
from pathlib import Path
from typing import Dict

from package_format import FIELD_NAMES

logger = logging.getLogger(__name__)

def resolve_path(data_dir: str, mac_address: str, receipt_time: datetime) -> Path:
    day = receipt_time.strftime("%Y-%m-%d")
    return Path(data_dir) / mac_address / f"{day}.csv"


class CsvWriter:
    def __init__(self, data_dir: str):
        self._data_dir = data_dir

    def write(self, record: Dict[str, object], receipt_time: datetime) -> None:
        mac_address = record["mac_address"]
        path = resolve_path(self._data_dir, mac_address, receipt_time)

        try:
            path.parent.mkdir(parents=True, exist_ok=True)
            file_exists = path.exists()

            with open(path, mode="a", newline="") as f:
                writer = csv.DictWriter(f, fieldnames=FIELD_NAMES)
                if not file_exists:
                    writer.writeheader()
                writer.writerow(record)

        except OSError as exc:
            logger.error("failed to write record to %s: %s", path, exc)