# Dynamic Range Sum Queries - CSES task 1648
# https://cses.fi/problemset/task/1648
# Fenwick tree (BIT): point update + range sum in O(log n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    # cur[i] = current value at 1-indexed position i
    cur = [0] * (n + 1)
    tree = [0] * (n + 1)

    # Build BIT with initial values.
    for i in range(1, n + 1):
        v = int(data[pos]); pos += 1
        cur[i] = v
        tree[i] += v
        j = i + (i & (-i))
        if j <= n:
            tree[j] += tree[i]

    out = []
    for _ in range(q):
        t = data[pos]; pos += 1
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        if t == b"1":
            # Set position a to value b -> apply delta.
            delta = b - cur[a]
            cur[a] = b
            i = a
            while i <= n:
                tree[i] += delta
                i += i & (-i)
        else:
            # Range sum over [a, b] = prefix(b) - prefix(a-1).
            s = 0
            i = b
            while i > 0:
                s += tree[i]
                i -= i & (-i)
            i = a - 1
            while i > 0:
                s -= tree[i]
                i -= i & (-i)
            out.append(s)

    sys.stdout.buffer.write(b"\n".join(str(x).encode() for x in out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
