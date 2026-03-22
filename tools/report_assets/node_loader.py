from __future__ import annotations

import csv
from pathlib import Path

from .models import NodeRecord


class NodeCsvRepository:
    """Loads customer/depot nodes from the solver CSV format."""

    def __init__(self, csv_path: Path) -> None:
        self._csv_path = csv_path

    def load_all(self) -> tuple[NodeRecord, ...]:
        if not self._csv_path.is_file():
            raise FileNotFoundError(f"Node CSV not found: {self._csv_path}")

        records: list[NodeRecord] = []
        with self._csv_path.open(newline="", encoding="utf-8") as handle:
            reader = csv.reader(handle)
            next(reader, None)
            for row in reader:
                if not row or not str(row[0]).strip().isdigit():
                    continue
                records.append(
                    NodeRecord(
                        id=int(row[0]),
                        x=int(row[1]),
                        y=int(row[2]),
                        demand=int(row[3]),
                    )
                )

        records.sort(key=lambda n: n.id)
        return tuple(records)
