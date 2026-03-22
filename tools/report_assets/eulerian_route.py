from __future__ import annotations

from collections import defaultdict


class EulerianRouteBuilder:
    """
    Builds a closed walk starting at depot id 0 that uses each undirected edge once,
    when the multigraph is Eulerian (all vertices have even degree).
    """

    @staticmethod
    def closed_walk_from_depot_zero(edges: list[tuple[int, int]]) -> list[int] | None:
        if not edges:
            return None

        adj: dict[int, list[int]] = defaultdict(list)
        for a, b in edges:
            if a == b:
                continue
            adj[a].append(b)
            adj[b].append(a)

        for vertex in adj:
            if len(adj[vertex]) % 2 != 0:
                return None

        stack = [0]
        circuit: list[int] = []
        current = 0

        while stack:
            if adj[current]:
                nxt = adj[current].pop()
                for i, x in enumerate(adj[nxt]):
                    if x == current:
                        adj[nxt].pop(i)
                        break
                stack.append(current)
                current = nxt
            else:
                circuit.append(current)
                current = stack.pop() if stack else current

        circuit.reverse()
        if not circuit or circuit[0] != 0 or circuit[-1] != 0:
            return None
        return circuit
