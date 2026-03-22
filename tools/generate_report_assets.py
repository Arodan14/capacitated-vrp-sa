"""
Regenerate docs/report/assets JSON used by the static report.
Run from repo root: python tools/generate_report_assets.py

Implementation: tools/report_assets/ (modular package).
"""
from __future__ import annotations

import sys
from pathlib import Path

_TOOLS_DIR = Path(__file__).resolve().parent
if str(_TOOLS_DIR) not in sys.path:
    sys.path.insert(0, str(_TOOLS_DIR))

from report_assets.cli import main

if __name__ == "__main__":
    main()
