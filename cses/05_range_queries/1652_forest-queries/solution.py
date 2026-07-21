# Forest Queries - CSES 1652
# https://cses.fi/problemset/task/1652
# 2D prefix sum (static grid), O(1) per rectangle query.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    STAR = 42  # ord('*')

    # P[i] = prefix-sum row of length n+1; P[0] is all zeros (1-indexed).
    P = [[0] * (n + 1)]
    prev = P[0]
    for _ in range(n):
        row = data[idx]; idx += 1
        cur = [0] * (n + 1)
        s = 0
        for j in range(n):
            if row[j] == STAR:
                s += 1
            cur[j + 1] = prev[j + 1] + s
        P.append(cur)
        prev = cur

    out = []
    ap = out.append
    for _ in range(q):
        y1 = int(data[idx]); x1 = int(data[idx + 1])
        y2 = int(data[idx + 2]); x2 = int(data[idx + 3])
        idx += 4
        ap(P[y2][x2] - P[y1 - 1][x2] - P[y2][x1 - 1] + P[y1 - 1][x1 - 1])

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
