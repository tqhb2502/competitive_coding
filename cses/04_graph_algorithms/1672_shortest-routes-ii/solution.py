# Shortest Routes II - CSES 1672
# https://cses.fi/problemset/task/1672
#
# All-pairs shortest path on an undirected, positively weighted graph (c >= 1),
# with n <= 500 => use Floyd-Warshall in O(n^3).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); m = int(data[pos + 1]); q = int(data[pos + 2])
    pos += 3

    # INF must exceed any real reachable distance:
    # the longest path <= (n-1) edges * 1e9 = ~5e11 < 2^40.
    # Choose INF = 1<<40 (~1.1e12) so any sum passing through a missing edge (INF)
    # stays >= INF (never creates a fake shortcut), and fits within a small int.
    INF = 1 << 40

    dist = [[INF] * n for _ in range(n)]
    for i in range(n):
        dist[i][i] = 0

    for _ in range(m):
        a = int(data[pos]) - 1
        b = int(data[pos + 1]) - 1
        c = int(data[pos + 2])
        pos += 3
        # There may be multiple edges between the same pair -> keep the smallest.
        if c < dist[a][b]:
            dist[a][b] = c
            dist[b][a] = c

    # Floyd-Warshall. The inner loop is written as a list comprehension + zip
    # (runs at C speed) to be as fast as possible in pure Python.
    for k in range(n):
        dk = dist[k]
        for di in dist:
            dik = di[k]
            if dik < INF:
                # di[j] = min(di[j], dik + dk[j])
                di[:] = [dij if dij < (s := dik + dkj) else s
                         for dij, dkj in zip(di, dk)]

    out = []
    append = out.append
    for _ in range(q):
        a = int(data[pos]) - 1
        b = int(data[pos + 1]) - 1
        pos += 2
        d = dist[a][b]
        append(str(d) if d < INF else "-1")

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
