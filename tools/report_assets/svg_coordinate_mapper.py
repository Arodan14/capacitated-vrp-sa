from __future__ import annotations

from typing import Sequence

from .models import NodeRecord


class SvgLayoutMapper:
    """
    Maps dataset coordinates to SVG pixel space using the same padding and scale
    as SolutionVisualizer::saveRoutesSVG (900x900, padding 50).
    """

    def __init__(
        self,
        nodes: Sequence[NodeRecord],
        width: float = 900.0,
        height: float = 900.0,
        padding: float = 50.0,
    ) -> None:
        if not nodes:
            raise ValueError("nodes must not be empty")

        self._nodes = tuple(nodes)
        self._width = width
        self._height = height
        self._padding = padding

        xs = [n.x for n in self._nodes]
        ys = [n.y for n in self._nodes]
        self._min_x = min(xs)
        self._max_x = max(xs)
        self._min_y = min(ys)
        self._max_y = max(ys)
        self._scale_x = (width - 2 * padding) / (self._max_x - self._min_x + 1)
        self._scale_y = (height - 2 * padding) / (self._max_y - self._min_y + 1)

    def to_screen(self, node: NodeRecord) -> tuple[float, float]:
        sx = self._padding + (node.x - self._min_x) * self._scale_x
        sy = self._height - self._padding - (node.y - self._min_y) * self._scale_y
        return sx, sy

    def nearest_node_id(self, px: float, py: float) -> int:
        best_id = self._nodes[0].id
        best_d = float("inf")
        for node in self._nodes:
            sx, sy = self.to_screen(node)
            d = (sx - px) ** 2 + (sy - py) ** 2
            if d < best_d:
                best_d = d
                best_id = node.id
        return best_id
