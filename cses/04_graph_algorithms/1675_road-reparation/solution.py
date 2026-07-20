# Road Reparation - https://cses.fi/problemset/task/1675
# Minimum Spanning Tree via Kruskal + DSU.
# If the graph is not connected -> print "IMPOSSIBLE".

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Read edges: store as (c, a, b) so sorting is by cost.
    edges = []
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        c = int(data[idx]); idx += 1
        edges.append((c, a, b))

    edges.sort()

    # DSU with path compression + union by size (arrays, 1-indexed).
    parent = list(range(n + 1))
    size = [1] * (n + 1)

    def find(x):
        # Iterative path compression (avoids recursion).
        root = x
        while parent[root] != root:
            root = parent[root]
        while parent[x] != root:
            parent[x], x = root, parent[x]
        return root

    total = 0
    used = 0
    for c, a, b in edges:
        ra = find(a)
        rb = find(b)
        if ra != rb:
            # Union by size.
            if size[ra] < size[rb]:
                ra, rb = rb, ra
            parent[rb] = ra
            size[ra] += size[rb]
            total += c
            used += 1
            if used == n - 1:
                break

    if used == n - 1:
        sys.stdout.write(str(total) + "\n")
    else:
        sys.stdout.write("IMPOSSIBLE\n")


if __name__ == "__main__":
    main()
