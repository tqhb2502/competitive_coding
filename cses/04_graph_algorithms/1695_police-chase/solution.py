# Police Chase - CSES 1695
# https://cses.fi/problemset/task/1695
#
# Minimum edge cut on an undirected graph = max-flow (unit capacities) from
# vertex 1 to vertex n, solved with Dinic. The cut edges are found via the
# reachable set in the residual graph.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    # Edge lists for the flow network.
    to = []
    cap = []
    graph = [[] for _ in range(n + 1)]
    edges = []  # original undirected edges (a, b)

    def add_edge(u, v, c):
        # Undirected edge: both directions get capacity c, each being the
        # reverse (residual) of the other. Their indices are consecutive so
        # eid ^ 1 gives the reverse edge.
        graph[u].append(len(to)); to.append(v); cap.append(c)
        graph[v].append(len(to)); to.append(u); cap.append(c)

    for _ in range(m):
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        edges.append((a, b))
        add_edge(a, b, 1)

    s, t = 1, n

    level = [-1] * (n + 1)
    it = [0] * (n + 1)

    def bfs():
        for i in range(n + 1):
            level[i] = -1
        level[s] = 0
        q = deque([s])
        while q:
            u = q.popleft()
            for eid in graph[u]:
                v = to[eid]
                if cap[eid] > 0 and level[v] < 0:
                    level[v] = level[u] + 1
                    q.append(v)
        return level[t] >= 0

    def dfs(u, f):
        if u == t:
            return f
        gu = graph[u]
        while it[u] < len(gu):
            eid = gu[it[u]]
            v = to[eid]
            if cap[eid] > 0 and level[v] == level[u] + 1:
                d = dfs(v, f if f < cap[eid] else cap[eid])
                if d > 0:
                    cap[eid] -= d
                    cap[eid ^ 1] += d
                    return d
            it[u] += 1
        return 0

    sys.setrecursionlimit(10000)

    INF = float('inf')
    while bfs():
        for i in range(n + 1):
            it[i] = 0
        while dfs(s, INF) > 0:
            pass

    # Reachable set from s in the residual graph -> one side of the min cut.
    visited = [False] * (n + 1)
    visited[s] = True
    q = deque([s])
    while q:
        u = q.popleft()
        for eid in graph[u]:
            v = to[eid]
            if cap[eid] > 0 and not visited[v]:
                visited[v] = True
                q.append(v)

    cut = [(a, b) for (a, b) in edges if visited[a] != visited[b]]

    out = [str(len(cut))]
    out.extend("%d %d" % (a, b) for (a, b) in cut)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
