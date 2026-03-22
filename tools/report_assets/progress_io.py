from __future__ import annotations

import csv
import json
from pathlib import Path

from .models import ProgressRow


class AnnealingProgressCsvRepository:
    """Reads annealing history CSV produced by the C++ solver."""

    def __init__(self, csv_path: Path) -> None:
        self._csv_path = csv_path

    def load_rows(self) -> tuple[ProgressRow, ...]:
        if not self._csv_path.is_file():
            raise FileNotFoundError(f"Progress CSV not found: {self._csv_path}")

        rows: list[ProgressRow] = []
        with self._csv_path.open(newline="", encoding="utf-8") as handle:
            reader = csv.DictReader(handle)
            for record in reader:
                rows.append(
                    ProgressRow(
                        step=int(record["step"]),
                        temperature=float(record["temperature"]),
                        best_cost=float(record["best_cost"]),
                    )
                )
        return tuple(rows)


class JsonArrayWriter:
    """Writes JSON arrays with compact formatting suitable for browser fetch()."""

    @staticmethod
    def write_compact(path: Path, rows: tuple[ProgressRow, ...]) -> None:
        payload = [row.as_chart_dict() for row in rows]
        path.write_text(json.dumps(payload), encoding="utf-8")
