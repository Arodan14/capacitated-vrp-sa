from __future__ import annotations

import re
from pathlib import Path
from typing import Sequence

from .eulerian_route import EulerianRouteBuilder
from .models import NodeRecord, SolutionPayload
from .svg_coordinate_mapper import SvgLayoutMapper


class SvgRoutesGraphParser:
    """Parses line-based route SVG exports into a structured solution payload."""

    _LINE_PATTERN = re.compile(
        r"<line x1='([^']+)' y1='([^']+)' x2='([^']+)' y2='([^']+)' stroke='([^']+)'"
    )

    def __init__(self, nodes: Sequence[NodeRecord]) -> None:
        self._nodes = tuple(nodes)
        self._mapper = SvgLayoutMapper(self._nodes)

    def parse_file(self, svg_path: Path) -> SolutionPayload:
        text = svg_path.read_text(encoding="utf-8")
        return self.parse_text(text)

    def parse_text(self, svg_text: str) -> SolutionPayload:
        segments = self._stroke_segments(svg_text)
        routes: list[tuple[int, ...]] = []

        for segment_edges in segments:
            walk = EulerianRouteBuilder.closed_walk_from_depot_zero(segment_edges)
            if walk:
                routes.append(tuple(walk))

        return SolutionPayload(nodes=self._nodes, routes=tuple(routes))

    def _stroke_segments(self, svg_text: str) -> list[list[tuple[int, int]]]:
        segments: list[list[tuple[int, int]]] = []
        current: list[tuple[int, int]] = []
        current_stroke: str | None = None

        for x1, y1, x2, y2, stroke in self._LINE_PATTERN.findall(svg_text):
            if stroke != current_stroke:
                if current:
                    segments.append(current)
                current = []
                current_stroke = stroke

            a = self._mapper.nearest_node_id(float(x1), float(y1))
            b = self._mapper.nearest_node_id(float(x2), float(y2))
            if a != b:
                current.append((a, b))

        if current:
            segments.append(current)

        return segments
