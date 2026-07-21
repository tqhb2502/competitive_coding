# Line Segments Trace II - https://cses.fi/problemset/task/3428
#
# Given n segments with integer endpoints and integer slope. For every integer x
# in [0, m], report the maximum y over all segments covering x (or -1 if none).
#
# Since the slope is an integer, y at every integer x is an integer, so we use
# exact integer arithmetic throughout (no floats).
#
# Each segment is a linear function y = a*x + b restricted to [x1, x2]. We need
# the upper envelope (max of the lines) evaluated at every integer point. Build a
# segment tree and decompose each [x1, x2] into O(log m) canonical nodes; a line
# stored at a node covers the node's whole x-range. For each node build the upper
# envelope with the convex hull trick (lines pre-sorted by increasing slope) and
# evaluate it at the integer points with a moving two-pointer. Complexity is
# ~ O((n + m) log m).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    segs = []
    for _ in range(n):
        x1 = int(data[pos]); y1 = int(data[pos + 1])
        x2 = int(data[pos + 2]); y2 = int(data[pos + 3])
        pos += 4
        a = (y2 - y1) // (x2 - x1)      # slope, guaranteed integer (exact)
        b = y1 - a * x1                  # intercept: y = a*x + b
        segs.append((a, b, x1, x2))

    # Sort by slope ascending so each segment-tree node's list stays sorted.
    segs.sort(key=lambda t: t[0])

    N = m + 1
    size = 1
    while size < N:
        size <<= 1

    tree = [None] * (2 * size)

    # Canonical decomposition of each segment interval [x1, x2] (inclusive),
    # appending its line to every canonical node.
    for a, b, x1, x2 in segs:
        lo = x1 + size
        hi = x2 + size
        line = (a, b)
        while lo <= hi:
            if lo & 1:
                lst = tree[lo]
                if lst is None:
                    tree[lo] = [line]
                else:
                    lst.append(line)
                lo += 1
            if not (hi & 1):
                lst = tree[hi]
                if lst is None:
                    tree[hi] = [line]
                else:
                    lst.append(line)
                hi -= 1
            lo >>= 1
            hi >>= 1

    # Precompute each node's covered x-range [nlo, nhi].
    nlo = [0] * (2 * size)
    nhi = [0] * (2 * size)
    for p in range(size, 2 * size):
        pp = p - size
        if pp < N:
            nlo[p] = pp
            nhi[p] = pp
        else:
            nlo[p] = N       # padding leaf (never part of a real canonical node)
            nhi[p] = -1
    for p in range(size - 1, 0, -1):
        nlo[p] = nlo[2 * p]
        nhi[p] = nhi[2 * p + 1]

    ans = [-1] * N

    for p in range(1, 2 * size):
        lst = tree[p]
        if not lst:
            continue

        # Build upper envelope (max) via convex hull trick.
        # lst is sorted by slope ascending.
        env_a = []
        env_b = []
        for a, b in lst:
            add = True
            while env_a:
                am = env_a[-1]
                bm = env_b[-1]
                if am == a:
                    # same slope: keep only the larger intercept
                    if bm >= b:
                        add = False
                        break
                    env_a.pop()
                    env_b.pop()
                    continue
                if len(env_a) >= 2:
                    al = env_a[-2]
                    bl = env_b[-2]
                    # top line is redundant if its crossover ordering fails.
                    if (bm - bl) * (am - a) >= (b - bm) * (al - am):
                        env_a.pop()
                        env_b.pop()
                        continue
                break
            if add:
                env_a.append(a)
                env_b.append(b)

        L = nlo[p]
        R = nhi[p]
        elen = len(env_a)
        j = 0
        for x in range(L, R + 1):
            # advance to the optimal line for this x (crossovers are increasing)
            while j + 1 < elen and env_a[j + 1] * x + env_b[j + 1] >= env_a[j] * x + env_b[j]:
                j += 1
            v = env_a[j] * x + env_b[j]
            if v > ans[x]:
                ans[x] = v

    sys.stdout.write(" ".join(map(str, ans)))
    sys.stdout.write("\n")


main()
