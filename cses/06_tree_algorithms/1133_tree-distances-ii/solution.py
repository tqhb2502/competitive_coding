# Tree Distances II - CSES 1133
# https://cses.fi/problemset/task/1133
# For each node, sum of distances to all other nodes.
# Rerooting DP, iterative traversal (no recursion), pure Python stdlib only.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    if n == 1:
        sys.stdout.buffer.write(b"0\n")
        return

    # Build adjacency list.
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # Iterative BFS from root = 1 to get parent, order, depth.
    parent = [0] * (n + 1)
    depth = [0] * (n + 1)
    order = [0] * n  # BFS order of visited nodes
    visited = bytearray(n + 1)

    root = 1
    visited[root] = 1
    order[0] = root
    head = 0
    tail = 1
    while head < tail:
        u = order[head]
        head += 1
        du = depth[u]
        pu = parent[u]
        for w in adj[u]:
            if not visited[w]:
                visited[w] = 1
                parent[w] = u
                depth[w] = du + 1
                order[tail] = w
                tail += 1

    # Subtree sizes: process order in reverse (children before parents).
    size = [1] * (n + 1)
    for i in range(n - 1, 0, -1):
        v = order[i]
        size[parent[v]] += size[v]

    # ans[root] = sum of depths.
    ans = [0] * (n + 1)
    s = 0
    for i in range(n):
        s += depth[order[i]]
    ans[root] = s

    # Rerooting: process order top-down (parents before children).
    for i in range(1, n):
        v = order[i]
        p = parent[v]
        ans[v] = ans[p] + n - 2 * size[v]

    out = " ".join(str(ans[i]) for i in range(1, n + 1))
    sys.stdout.buffer.write(out.encode() + b"\n")


main()
