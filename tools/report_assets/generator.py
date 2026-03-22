from __future__ import annotations

import json

from .models import SolutionPayload
from .node_loader import NodeCsvRepository
from .paths import ReportProjectLayout
from .progress_io import AnnealingProgressCsvRepository, JsonArrayWriter
from .svg_route_parser import SvgRoutesGraphParser


class ReportAssetGenerator:
    """
    Orchestrates conversion of on-disk CSV/SVG artifacts into JSON consumed by the web report.
    """

    def __init__(self, layout: ReportProjectLayout) -> None:
        self._layout = layout

    def write_solution_json_if_svg_present(self) -> bool:
        svg = self._layout.exp_a_routes_svg
        if not svg.is_file():
            return False

        nodes = NodeCsvRepository(self._layout.vrp_nodes_csv).load_all()
        payload: SolutionPayload = SvgRoutesGraphParser(nodes).parse_file(svg)
        out = self._layout.exp_a_solution_json
        out.write_text(json.dumps(payload.as_json_dict(), indent=0), encoding="utf-8")
        print(f"Wrote {out} ({payload.route_count} routes)")
        return True

    def write_progress_json_if_csv_present(self) -> bool:
        csv_path = self._layout.exp_a_progress_csv
        if not csv_path.is_file():
            return False

        rows = AnnealingProgressCsvRepository(csv_path).load_rows()
        out = self._layout.exp_a_progress_json
        JsonArrayWriter.write_compact(out, rows)
        print(f"Wrote {out} ({len(rows)} points)")
        return True

    def run(self) -> None:
        self.write_solution_json_if_svg_present()
        self.write_progress_json_if_csv_present()
