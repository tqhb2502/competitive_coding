# Longest Flight Route - CSES 1680
# https://cses.fi/problemset/task/1680
# Longest path in a DAG from node 1 to node n via topological sort (Kahn) + DP.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # adjacency list + indegree for Kahn's topological sort
    adj = [[] for _ in range(n + 1)]
    indeg = [0] * (n + 1)
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        indeg[b] += 1

    # dist[v] = max number of cities on a path from 1 to v (-1 = unreachable)
    dist = [-1] * (n + 1)
    parent = [0] * (n + 1)
    dist[1] = 1

    # Kahn's algorithm: process nodes in topological order
    dq = deque(v for v in range(1, n + 1) if indeg[v] == 0)
    while dq:
        u = dq.popleft()
        du = dist[u]
        for v in adj[u]:
            if du != -1 and du + 1 > dist[v]:
                dist[v] = du + 1
                parent[v] = u
            indeg[v] -= 1
            if indeg[v] == 0:
                dq.append(v)

    out = sys.stdout
    if dist[n] == -1:
        out.write("IMPOSSIBLE\n")
        return

    # reconstruct path from n back to 1
    path = []
    cur = n
    while cur != 0:
        path.append(cur)
        if cur == 1:
            break
        cur = parent[cur]
    path.reverse()

    out.write(str(dist[n]))
    out.write("\n")
    out.write(" ".join(map(str, path)))
    out.write("\n")


if __name__ == "__main__":
    main()
