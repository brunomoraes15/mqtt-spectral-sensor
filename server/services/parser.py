
from typing import Dict, List
from package_format import pkg_columns
from dataclasses import dataclass
from typing import List, Optional
from package_format import EXPECTED_FIELD_COUNT, pkg_columns


@dataclass
class ValidationResult:
    is_valid: bool
    reason: Optional[str] = None
    fields: Optional[List[str]] = None


def validate_payload(payload: str) -> ValidationResult:
    """Check structural and type validity of a raw CSV payload string."""
    if payload is None or payload.strip() == "":
        return ValidationResult(False, "empty payload")

    raw_fields = payload.strip().split(",")

    if len(raw_fields) != EXPECTED_FIELD_COUNT:
        return ValidationResult(
            False,
            f"expected {EXPECTED_FIELD_COUNT} fields, got {len(raw_fields)}",
        )

    for field, raw_value in zip(pkg_columns, raw_fields):
        try:
            field.cast(raw_value)
        except (ValueError, TypeError) as exc:
            return ValidationResult(
                False, f"field '{field.name}' invalid ({raw_value!r}): {exc}"
            )

    return ValidationResult(True, fields=raw_fields)

def parse_fields(raw_fields: List[str]) -> Dict[str, object]:
    """Convert a list of raw string fields (already validated) into a
    structured record keyed by schema field name."""
    return {field.name: field.data(value) for field, value in zip(pkg_columns, raw_fields)}