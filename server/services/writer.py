import csv
import logging
import os
import threading
from datetime import datetime
from typing import TextIO

from config import *



class FileWriter:
    def __init__(self, output_dir: str):
        self.output_dir = output_dir
        os.makedirs(self.output_dir, exist_ok=True)

        self._handles: dict[
            tuple[int, str],
            tuple[TextIO, csv.writer]
        ] = {}

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

    def _get_writer(self, record: dict) -> tuple[csv.writer, TextIO]:
        sensor_id = record["sensor_id"]
        date = datetime.fromtimestamp(record["timestamp"]).strftime("%Y-%m-%d")
        key = (sensor_id, date)

        if key in self._handles:
            file, writer = self._handles[key]
            return writer, file

        path = os.path.join(self.output_dir,f"{sensor_id}-{date}.csv")

        is_new = not os.path.exists(path)

        file = open(path, mode="a", newline="", encoding="utf-8")
        writer = csv.writer(file)

        if is_new:
            writer.writerow(list(PAYLOAD_FORMAT.keys()))
            file.flush()
            logger.info("Created file: %s", path)

        self._handles[key] = (file, writer)

        return writer, file