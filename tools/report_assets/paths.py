from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class ReportProjectLayout:
    """Filesystem layout for the VRP repo and report assets."""

    root: Path

    @classmethod
    def from_package_file(cls, package_module_file: Path) -> ReportProjectLayout:
        """
        Resolve repo root from any module file under tools/report_assets/
        (e.g. cli.py -> parents[2] is the repository root).
        """
        root = package_module_file.resolve().parents[2]
        return cls(root=root)

    @property
    def vrp_nodes_csv(self) -> Path:
        return self.root / "VRP(nodes).csv"

    @property
    def report_assets_dir(self) -> Path:
        return self.root / "docs" / "report" / "assets"

    @property
    def exp_a_routes_svg(self) -> Path:
        return self.report_assets_dir / "exp_a_routes_graph.svg"

    @property
    def exp_a_solution_json(self) -> Path:
        return self.report_assets_dir / "exp_a_solution.json"

    @property
    def exp_a_progress_csv(self) -> Path:
        return self.report_assets_dir / "exp_a_annealing_progress.csv"

    @property
    def exp_a_progress_json(self) -> Path:
        return self.report_assets_dir / "exp_a_annealing_progress.json"
