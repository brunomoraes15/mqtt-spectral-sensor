
from typing import Callable, List, NamedTuple

class pkg_col(NamedTuple):
    name: str
    data: Callable[[str], object]

# Order matches Package::build_packgage() exactly.
pkg_columns: List[pkg_col] = [
    pkg_col("timestamp_ms", int),
    pkg_col("sensor_id", int),
    pkg_col("mac_address", str),
    pkg_col("gain", int),
    pkg_col("astep", int),
    pkg_col("integration_time_us", float),
    pkg_col("f1", int),
    pkg_col("f2", int),
    pkg_col("f3", int),
    pkg_col("f4", int),
    pkg_col("f5", int),
    pkg_col("f6", int),
    pkg_col("f7", int),
    pkg_col("f8", int),
    pkg_col("clear", int),
    pkg_col("nir", int),
]

FIELD_NAMES: List[str] = [f.name for f in pkg_columns]
EXPECTED_FIELD_COUNT: int = len(pkg_columns)