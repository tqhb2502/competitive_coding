# Tree Distances I - https://cses.fi/problemset/task/1132
# For each node, find the maximum distance to any other node.
# The farthest node from any node is always one of the two diameter endpoints.
# So: BFS to find endpoint A, BFS from A -> distA and endpoint B, BFS from B -> distB.
# Answer[v] = max(distA[v], distB[v]).  All BFS are iterative (no recursion).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    if n == 1:
        sys.stdout.buffer.write(b"0\n")
        return

    # Build CSR adjacency using flat arrays for speed.
    deg = [0] * (n + 1)
    us = [0] * (n - 1)
    vs = [0] * (n - 1)
    for i in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        us[i] = a
        vs[i] = b
        deg[a] += 1
        deg[b] += 1

    start = [0] * (n + 2)
    for v in range(1, n + 1):
        start[v + 1] = start[v] + deg[v]
    adj = [0] * (2 * (n - 1))
    pos = start[:]  # copy write positions
    for i in range(n - 1):
        a = us[i]; b = vs[i]
        adj[pos[a]] = b; pos[a] += 1
        adj[pos[b]] = a; pos[b] += 1

    def bfs(src):
        dist = [-1] * (n + 1)
        queue = [0] * n
        head = 0
        tail = 0
        dist[src] = 0
        queue[tail] = src; tail += 1
        far = src
        while head < tail:
            u = queue[head]; head += 1
            du = dist[u]
            for j in range(start[u], start[u + 1]):
                w = adj[j]
                if dist[w] == -1:
                    dist[w] = du + 1
                    if dist[w] > dist[far]:
                        far = w
                    queue[tail] = w; tail += 1
        return dist, far

    # Step 1: find one diameter endpoint A.
    _, A = bfs(1)
    # Step 2: from A get distA and the other endpoint B.
    distA, B = bfs(A)
    # Step 3: from B get distB.
    distB, _ = bfs(B)

    out = [0] * n
    for v in range(1, n + 1):
        da = distA[v]
        db = distB[v]
        out[v - 1] = da if da > db else db

    sys.stdout.buffer.write((" ".join(map(str, out)) + "\n").encode())


main()
