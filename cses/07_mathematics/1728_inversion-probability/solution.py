# Inversion Probability - CSES 1728
# https://cses.fi/problemset/task/1728
#
# E[inversions] = sum_{i<j} P(x_i > x_j) (linearity of expectation).
# For pair (i, j): P(x_i > x_j) = (ordered pairs with u > v) / (r_i * r_j),
# where count = sum_{v=1}^{r_j} max(0, r_i - v).

import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    r = [int(x) for x in data[1:1 + n]]

    ans = 0.0
    for i in range(n):
        ri = r[i]
        for j in range(i + 1, n):
            rj = r[j]
            # count = number of (u, v), u in [1, ri], v in [1, rj], u > v
            # = sum_{v=1}^{min(rj, ri-1)} (ri - v)
            m = min(rj, ri - 1)
            if m > 0:
                # sum_{v=1}^{m} (ri - v) = m*ri - m*(m+1)//2
                count = m * ri - m * (m + 1) // 2
                ans += count / (ri * rj)

    sys.stdout.write("%.6f\n" % ans)


main()
