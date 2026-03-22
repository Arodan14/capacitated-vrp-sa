from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class NodeRecord:
    id: int
    x: int
    y: int
    demand: int

    def as_json_dict(self) -> dict:
        return {"id": self.id, "x": self.x, "y": self.y, "demand": self.demand}


@dataclass(frozen=True)
class ProgressRow:
    step: int
    temperature: float
    best_cost: float

    def as_chart_dict(self) -> dict:
        return {"step": self.step, "temperature": self.temperature, "bestCost": self.best_cost}


@dataclass(frozen=True)
class SolutionPayload:
    nodes: tuple[NodeRecord, ...]
    routes: tuple[tuple[int, ...], ...]

    @property
    def route_count(self) -> int:
        return len(self.routes)

    def as_json_dict(self) -> dict:
        return {
            "nodes": [n.as_json_dict() for n in self.nodes],
            "routes": [list(route) for route in self.routes],
        }
