# Dynamic Range Minimum Queries - https://cses.fi/problemset/task/1649
# Point update + range minimum query using an iterative (bottom-up) segment tree.
# Pure Python standard library only.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # segment tree size = next power of two >= n
    size = 1
    while size < n:
        size <<= 1

    INF = 1 << 62
    tree = [INF] * (2 * size)

    # place leaves
    base = size
    for i in range(n):
        tree[base + i] = int(data[idx]); idx += 1

    # build internal nodes
    for p in range(size - 1, 0, -1):
        l = tree[2 * p]
        r = tree[2 * p + 1]
        tree[p] = l if l < r else r

    out = []
    out_append = out.append

    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'1':
            k = int(data[idx]); idx += 1
            u = int(data[idx]); idx += 1
            # point update: position k (1-indexed) -> u
            pos = size + k - 1
            tree[pos] = u
            pos >>= 1
            while pos:
                l = tree[2 * pos]
                r = tree[2 * pos + 1]
                tree[pos] = l if l < r else r
                pos >>= 1
        else:
            a = int(data[idx]); idx += 1
            b = int(data[idx]); idx += 1
            # range min over closed interval [a, b] (1-indexed) -> half-open [a-1, b)
            lo = a - 1 + size
            hi = b + size
            res = INF
            while lo < hi:
                if lo & 1:
                    v = tree[lo]
                    if v < res:
                        res = v
                    lo += 1
                if hi & 1:
                    hi -= 1
                    v = tree[hi]
                    if v < res:
                        res = v
                lo >>= 1
                hi >>= 1
            out_append(res)

    sys.stdout.buffer.write(b'\n'.join(str(x).encode() for x in out))
    if out:
        sys.stdout.buffer.write(b'\n')


main()
