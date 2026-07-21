# Distance Queries — CSES 1135
# https://cses.fi/problemset/task/1135
#
# Tree distance via LCA (binary lifting):
#   dist(a, b) = depth[a] + depth[b] - 2 * depth[LCA(a, b)]
# parent/depth computed by iterative BFS (no recursion; n up to 2e5).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # Store raw edges first (build CSR adjacency afterwards).
    edges_a = [0] * (n - 1) if n > 1 else []
    edges_b = [0] * (n - 1) if n > 1 else []
    deg = [0] * (n + 1)
    for i in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        edges_a[i] = a
        edges_b[i] = b
        deg[a] += 1
        deg[b] += 1

    # Build CSR adjacency.
    start = [0] * (n + 2)
    for v in range(1, n + 1):
        start[v + 1] = start[v] + deg[v]
    adj = [0] * (2 * (n - 1)) if n > 1 else []
    pos = start[:]  # copy write cursors
    for i in range(n - 1):
        a = edges_a[i]; b = edges_b[i]
        adj[pos[a]] = b; pos[a] += 1
        adj[pos[b]] = a; pos[b] += 1

    # Iterative BFS from node 1 to get parent and depth.
    depth = [0] * (n + 1)
    parent = [0] * (n + 1)
    visited = bytearray(n + 1)
    order = [0] * n
    order[0] = 1
    visited[1] = 1
    parent[1] = 0
    depth[1] = 0
    qh = 0
    qt = 1
    while qh < qt:
        u = order[qh]; qh += 1
        du = depth[u]
        for e in range(start[u], start[u + 1]):
            w = adj[e]
            if not visited[w]:
                visited[w] = 1
                parent[w] = u
                depth[w] = du + 1
                order[qt] = w; qt += 1

    # Binary lifting table.
    LOG = 1
    while (1 << LOG) < n:
        LOG += 1
    if LOG < 1:
        LOG = 1

    up = [parent]  # up[0] = parent
    for k in range(1, LOG):
        prev = up[k - 1]
        cur = [0] * (n + 1)
        for v in range(1, n + 1):
            cur[v] = prev[prev[v]]
        up.append(cur)

    out = []
    for _ in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        da = depth[a]; db = depth[b]
        # Bring the deeper one up.
        if da < db:
            a, b = b, a
            da, db = db, da
        diff = da - db
        k = 0
        while diff:
            if diff & 1:
                a = up[k][a]
            diff >>= 1
            k += 1
        if a == b:
            l = a
        else:
            for k in range(LOG - 1, -1, -1):
                ua = up[k][a]
                ub = up[k][b]
                if ua != ub:
                    a = ua
                    b = ub
            l = parent[a]
        out.append(da + db - 2 * depth[l])

    sys.stdout.buffer.write(("\n".join(map(str, out)) + ("\n" if out else "")).encode())


if __name__ == "__main__":
    main()
