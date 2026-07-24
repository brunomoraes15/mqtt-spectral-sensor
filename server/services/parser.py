from config import *

def validate_payload(fields: list[str]) -> None:
    print(len(fields))
    if len(fields) != PAYLOAD_SIZE:
        raise Exception(
            f"Expected {PAYLOAD_SIZE} fields, got {len(fields)}"
        )

def parse_payload(payload: str) -> dict:
    fields = payload.strip().split(",")
    validate_payload(fields)

    record = {}

    for (name, type), data in zip(PAYLOAD_FORMAT.items(), fields):
        record[name] = type(data)

    validate_payload(record)
    return record

