# Distinct Values Queries — CSES 1734
# https://cses.fi/problemset/task/1734
# Offline + Fenwick tree (BIT): sort queries by right endpoint, keep only the
# last occurrence of each value marked with 1, answer range-sum on [a, b].

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    arr = data[idx:idx + n]
    idx += n

    # Queries: store (b, a, original_index), sort by b ascending.
    queries = []
    for k in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        queries.append((b, a, k))
    queries.sort()

    # Fenwick tree (1-indexed) over positions 1..n.
    tree = [0] * (n + 1)

    def update(i, delta):
        while i <= n:
            tree[i] += delta
            i += i & (-i)

    def prefix(i):
        s = 0
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        return s

    ans = [0] * q
    last = {}          # value (as bytes) -> last position seen
    pos = 1            # next array position to add (1-indexed)

    for b, a, k in queries:
        # Add all positions up to b that are not yet added.
        while pos <= b:
            v = arr[pos - 1]
            prev = last.get(v)
            if prev is not None:
                update(prev, -1)
            update(pos, 1)
            last[v] = pos
            pos += 1
        ans[k] = prefix(b) - prefix(a - 1)

    out = b"\n".join(str(x).encode() for x in ans)
    sys.stdout.buffer.write(out + b"\n")


main()
