# Course Schedule - CSES 1679
# https://cses.fi/problemset/task/1679
# Topological sort using Kahn's algorithm (BFS on in-degrees).
# If a valid ordering of all n courses exists, print it; otherwise "IMPOSSIBLE".

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    adj = [[] for _ in range(n + 1)]
    indeg = [0] * (n + 1)

    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        # course a must be completed before course b: edge a -> b
        adj[a].append(b)
        indeg[b] += 1

    # Start with all courses that have no prerequisites.
    q = deque(v for v in range(1, n + 1) if indeg[v] == 0)

    order = []
    while q:
        u = q.popleft()
        order.append(u)
        for v in adj[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)

    if len(order) == n:
        sys.stdout.write(' '.join(map(str, order)) + '\n')
    else:
        # A cycle exists -> no valid schedule.
        sys.stdout.write('IMPOSSIBLE\n')


main()
