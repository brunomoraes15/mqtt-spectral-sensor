PAYLOAD_FORMAT = {
    "timestamp": int,
    "sensor_id": int,
    "mac_address": str,
    "again": float,
    "astep": float,
    "int_time": int,

    "f1": float,
    "f2": float,
    "f3": float,
    "f4": float,
    "f5": float,
    "f6": float,
    "f7": float,
    "f8": float,

    "clear": float,
    "nir": float
}

PAYLOAD_SIZE = len(PAYLOAD_FORMAT)