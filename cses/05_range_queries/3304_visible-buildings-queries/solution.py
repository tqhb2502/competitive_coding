# Visible Buildings Queries - https://cses.fi/problemset/task/3304
# For a query [a, b]: starting at building a and moving right, a building is
# "visible" if it is strictly taller than every building before it inside the
# range. The chain of visible buildings is exactly: start at a, then repeatedly
# jump to the next building strictly greater than the current one (next greater
# element to the right, "nge"). We binary-lift over these nge pointers so each
# query counts, in O(log n), how many chain positions stay within [a, b].

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    h = [0] * (n + 2)
    for i in range(1, n + 1):
        h[i] = int(data[pos]); pos += 1

    INF = n + 1  # sentinel position, always > b since b <= n

    # nge[i] = smallest j > i with h[j] > h[i], else INF (strict).
    nge = [INF] * (n + 2)
    stack = []
    for i in range(n, 0, -1):
        hi = h[i]
        while stack and h[stack[-1]] <= hi:
            stack.pop()
        nge[i] = stack[-1] if stack else INF
        stack.append(i)
    nge[INF] = INF  # absorbing state for the sentinel

    # Binary lifting table: up[k][i] = position after 2^k nge-jumps from i.
    LOG = 1
    while (1 << LOG) < n + 1:
        LOG += 1
    up = [nge]
    for k in range(1, LOG):
        prev = up[k - 1]
        cur = [0] * (n + 2)
        for i in range(1, n + 2):
            cur[i] = prev[prev[i]]
        up.append(cur)

    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        node = a
        ans = 1  # building a itself is always visible
        for k in range(LOG - 1, -1, -1):
            nxt = up[k][node]
            if nxt <= b:
                node = nxt
                ans += 1 << k
        ap(ans)

    sys.stdout.buffer.write(("\n".join(map(str, out)) + "\n").encode())


main()
