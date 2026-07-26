import csv
import logging
import os
import threading
from dataclasses import dataclass
from datetime import datetime
from typing import Any, TextIO

from config import *

@dataclass
class _Handle:
    date: str
    file: TextIO
    writer: Any

logger = logging.getLogger(__name__)

class FileWriter:
    def __init__(self, output_dir: str):
        self.output_dir = output_dir
        os.makedirs(self.output_dir, exist_ok=True)

        self._handles: dict[int, _Handle] = {}

        self._lock = threading.Lock()

    def append(self, record: dict) -> None:
        with self._lock:
            writer, file = self._get_writer(record)

            row = [record[field] for field in PAYLOAD_FORMAT]

            writer.writerow(row)
            file.flush()

    def close(self) -> None:
        with self._lock:
            for file, _ in self._handles.values():
                file.close()

            self._handles.clear()

    def _get_writer(self, record: dict) -> _Handle:
        sensor_id = record["sensor_id"]

        date = datetime.now().strftime("%Y-%m-%d")

        existing = self._handles.get(sensor_id)
        if existing is not None:
            if existing.date == date:
                return existing
            existing.file.close()

        path = os.path.join(self.output_dir, f"{sensor_id}-{date}.csv")

        is_new = not os.path.exists(path)

        file = open(path, mode="a", newline="", encoding="utf-8")
        writer = csv.writer(file)

        if is_new:
            writer.writerow(list(PAYLOAD_FORMAT.keys()))
            file.flush()
            logger.info("Created file: %s", path)

        handle = _Handle(date=date, file=file, writer=writer)
        self._handles[sensor_id] = handle

        return handle