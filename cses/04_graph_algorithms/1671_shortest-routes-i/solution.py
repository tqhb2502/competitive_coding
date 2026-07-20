# Shortest Routes I - CSES 1671
# https://cses.fi/problemset/task/1671
# Single-source shortest path with non-negative weights -> Dijkstra + heapq.

import sys
from heapq import heappush, heappop


def main():
    # Convert every token to int in one C-level pass (much faster than calling
    # int() per token in a Python loop), then slice out the edge columns.
    nums = list(map(int, sys.stdin.buffer.read().split()))
    n = nums[0]
    m = nums[1]
    us = nums[2::3]                # edge sources a_i
    vs = nums[3::3]                # edge targets b_i
    ws = nums[4::3]                # edge weights  c_i

    # Build graph in CSR (compressed sparse row) form for compactness/speed.
    head = [0] * (n + 2)          # degree counting then prefix sums
    for a in us:
        head[a + 1] += 1
    for i in range(1, n + 2):
        head[i] += head[i - 1]

    to = [0] * m
    wt = [0] * m
    fill = head[:]                 # current write position per node
    for i in range(m):
        a = us[i]
        p = fill[a]
        to[p] = vs[i]
        wt[p] = ws[i]
        fill[a] = p + 1

    INF = float('inf')
    dist = [INF] * (n + 1)
    dist[1] = 0
    heap = [(0, 1)]

    # Local references for speed.
    _push = heappush
    _pop = heappop
    while heap:
        d, u = _pop(heap)
        if d > dist[u]:
            continue
        start = head[u]
        end = head[u + 1]
        for e in range(start, end):
            v = to[e]
            nd = d + wt[e]
            if nd < dist[v]:
                dist[v] = nd
                _push(heap, (nd, v))

    out = ' '.join(map(str, dist[1:n + 1]))
    sys.stdout.write(out + '\n')


main()
