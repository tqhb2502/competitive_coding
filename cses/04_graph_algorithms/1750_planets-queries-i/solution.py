# Planets Queries I - CSES 1750
# https://cses.fi/problemset/task/1750
# Binary lifting on a functional graph (each node has out-degree 1).
# up[j][v] = node reached after 2^j teleports from v.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # t: 0-indexed target of each planet's teleporter
    t = [int(data[idx + i]) - 1 for i in range(n)]
    idx += n

    LOG = 30  # 2^30 > 1e9 >= k_max

    # up[j] = list of size n; up[0] = t.
    # up[j][v] == up[j-1][up[j-1][v]] == prev[prev[v]] for every v, so the whole
    # level is list(map(prev.__getitem__, prev)) -- much faster than a Python loop.
    up = [t]
    for _ in range(1, LOG):
        prev = up[-1]
        up.append(list(map(prev.__getitem__, prev)))

    # Read all queries into flat arrays (0-indexed starts).
    xs = [int(data[idx + 2 * i]) - 1 for i in range(q)]
    ks = [int(data[idx + 2 * i + 1]) for i in range(q)]

    # Answer all queries at once, one bit level at a time: at level j advance
    # every query whose bit j is set by up[j]. Batching avoids a per-query
    # Python while-loop and keeps the hot work inside C-level comprehensions.
    cur = xs
    for j in range(LOG):
        upj = up[j]
        cur = [upj[c] if (kk >> j) & 1 else c for c, kk in zip(cur, ks)]

    sys.stdout.write("\n".join(map(str, (c + 1 for c in cur))) + ("\n" if cur else ""))


if __name__ == "__main__":
    main()
