# Flight Routes - https://cses.fi/problemset/task/1196
# K shortest path prices from city 1 to city n (directed, positive weights,
# routes may repeat vertices/edges; equal-price routes are distinct).
# Extended Dijkstra: pop each vertex up to k times; the i-th valid pop of a
# vertex gives its i-th shortest distance.

import sys
from heapq import heappush, heappop


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    k = int(data[idx]); idx += 1

    # Adjacency list as list of lists of (v, w).
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        adj[a].append((b, c))

    cnt = [0] * (n + 1)          # times each vertex has been finalized
    heap = [(0, 1)]              # (distance, vertex)
    results = []

    # Bind hot-loop functions to locals to trim per-iteration lookup overhead.
    push = heappush
    pop = heappop

    while heap and len(results) < k:
        d, u = pop(heap)
        if cnt[u] >= k:
            continue
        cnt[u] += 1
        if u == n:
            results.append(d)
            if len(results) == k:
                break
        for v, w in adj[u]:
            if cnt[v] < k:
                push(heap, (d + w, v))

    sys.stdout.write(' '.join(map(str, results)))


main()
