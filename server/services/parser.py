from config import *


class PayloadValidationError(ValueError):
    pass

def validate_payload(fields: list[str]) -> None:
    if len(fields) != PAYLOAD_SIZE:
        raise PayloadValidationError(
            f"Expected {PAYLOAD_SIZE} fields, got {len(fields)}"
        )


def parse_payload(payload: str) -> dict:
    fields = payload.strip().split(",")
    validate_payload(fields)

    record = {}

    for (name, field_type), data in zip(PAYLOAD_FORMAT.items(), fields):
        try:
            record[name] = field_type(data)
        except (TypeError, ValueError) as e:
            raise PayloadValidationError(
                f"Invalid value for field '{name}': {data!r} ({e})"
            ) from e

    return record