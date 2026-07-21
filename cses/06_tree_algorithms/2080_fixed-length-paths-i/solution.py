# Fixed-Length Paths I  (CSES 2080)
# https://cses.fi/problemset/task/2080
#
# Count the number of paths in a tree consisting of exactly k edges.
#
# Approach: O(n) long-path (heavy-path) decomposition.
#   Root the tree. For each vertex v keep cnt[v][d] = number of vertices at
#   distance d below v in subtree(v). Every path has a unique LCA L; it is
#   either a vertical path with L as an endpoint (length k, counted via the
#   number of descendants of L at distance k), or it joins two different
#   children of L (counted while merging children of L).
#   Using long-path decomposition the heavy child's cnt array is inherited in
#   O(1) (shared memory with an index offset), and only the light children are
#   iterated, giving total O(n) time and O(n) memory.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    ints = list(map(int, data))          # one C-level conversion of all tokens
    n = ints[0]
    k = ints[1]

    # Build adjacency list.
    adj = [[] for _ in range(n + 1)]
    pos = 2
    for _ in range(n - 1):
        a = ints[pos]; b = ints[pos + 1]; pos += 2
        adj[a].append(b)
        adj[b].append(a)

    if n == 1:
        sys.stdout.buffer.write(b"0\n")
        return

    # Iterative DFS from root 1: compute parent and a preorder list.
    parent = [0] * (n + 1)
    order = []
    order_append = order.append
    visited = bytearray(n + 1)
    st = [1]
    visited[1] = 1
    while st:
        u = st.pop()
        order_append(u)
        for v in adj[u]:
            if not visited[v]:
                visited[v] = 1
                parent[v] = u
                st.append(v)

    # height[v] = number of edges on the longest downward path from v.
    # heavy[v]  = child of v that attains that height (0 if v is a leaf).
    height = [0] * (n + 1)
    heavy = [0] * (n + 1)
    for u in reversed(order):          # children come before their parent here
        p = parent[u]
        if p:
            h = height[u] + 1
            if h > height[p]:
                height[p] = h
                heavy[p] = u

    # Light-children lists (all children except the heavy one) and the memory
    # pool.  Each heavy chain (top t .. deepest leaf) gets a contiguous block of
    # size height[t]+1.  Vertex at chain offset j uses Dptr[v] = base + j, so
    # cnt[v][i] = D[Dptr[v] + i] and Dptr[v] = Dptr[heavy[v]] - 1.
    light = [None] * (n + 1)
    Dptr = [0] * (n + 1)
    pool = 0
    for v in order:
        p = parent[v]
        if v == 1 or heavy[p] != v:    # v is the top of a heavy chain / a light child
            if v != 1:
                lp = light[p]
                if lp is None:
                    light[p] = [v]
                else:
                    lp.append(v)
            base = pool
            pool += height[v] + 1
            cur = v
            off = base
            while cur:
                Dptr[cur] = off
                off += 1
                cur = heavy[cur]
    D = [0] * (pool + 1)

    ans = 0
    base_idx = k - 1
    # Process vertices bottom-up (reverse preorder): all children of u are done
    # before u, and the heavy child's cnt array already sits in shared memory.
    for u in reversed(order):
        du = Dptr[u]
        hu = height[u]
        D[du] = 1                      # cnt[u][0] = u itself; inherits heavy child
        lu = light[u]
        if lu is None:
            if k <= hu:
                ans += D[du + k]
            continue
        for c in lu:                   # light children only
            dc = Dptr[c]
            ch = height[c]
            # Pairing: a node at distance h in c is at distance h+1 from u; pair it
            # with an earlier-merged node at distance idx = k-(h+1) (idx >= 1 so that
            # u itself is excluded and both endpoints stay strictly below u).
            for h in range(ch + 1):
                idx = base_idx - h
                if idx < 1:
                    break              # idx only decreases as h grows
                if idx <= hu:
                    ans += D[dc + h] * D[du + idx]
            # Merge child c into u's cnt array.
            dd = du + 1
            for h in range(ch + 1):
                D[dd + h] += D[dc + h]
        # Vertical paths with u as an endpoint and length exactly k.
        if k <= hu:
            ans += D[du + k]

    sys.stdout.buffer.write((str(ans) + "\n").encode())


main()
