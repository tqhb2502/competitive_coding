# Maximum Manhattan Distances
# https://cses.fi/problemset/task/3410
#
# Manhattan distance |x1-x2| + |y1-y2| = max(|u1-u2|, |v1-v2|)
# with the 45-degree rotation u = x + y, v = x - y.
# Hence the maximum Manhattan distance over all pairs equals
#   max(maxU - minU, maxV - minV).
# We add points one by one and keep running min/max of u and v (exact integers).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    INF = float("inf")
    min_u = INF
    max_u = -INF
    min_v = INF
    max_v = -INF

    out = []
    for _ in range(n):
        x = int(data[idx]); y = int(data[idx + 1]); idx += 2
        u = x + y
        v = x - y
        if u < min_u:
            min_u = u
        if u > max_u:
            max_u = u
        if v < min_v:
            min_v = v
        if v > max_v:
            max_v = v
        du = max_u - min_u
        dv = max_v - min_v
        out.append(str(du if du > dv else dv))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
