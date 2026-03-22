from __future__ import annotations

from pathlib import Path

from .generator import ReportAssetGenerator
from .paths import ReportProjectLayout


def main() -> None:
    layout = ReportProjectLayout.from_package_file(Path(__file__))
    ReportAssetGenerator(layout).run()


if __name__ == "__main__":
    main()
