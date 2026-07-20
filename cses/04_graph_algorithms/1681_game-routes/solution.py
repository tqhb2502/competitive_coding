# Game Routes - https://cses.fi/problemset/task/1681
# Count number of paths from level 1 to level n in a DAG, modulo 1e9+7.
# Approach: Kahn's topological sort (iterative BFS) + DP.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    MOD = 1000000007

    adj = [[] for _ in range(n + 1)]
    indeg = [0] * (n + 1)

    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        indeg[b] += 1

    dp = [0] * (n + 1)
    dp[1] = 1

    dq = deque()
    for v in range(1, n + 1):
        if indeg[v] == 0:
            dq.append(v)

    while dq:
        u = dq.popleft()
        du = dp[u]
        for v in adj[u]:
            dp[v] = (dp[v] + du) % MOD
            indeg[v] -= 1
            if indeg[v] == 0:
                dq.append(v)

    sys.stdout.write(str(dp[n] % MOD) + "\n")


main()
