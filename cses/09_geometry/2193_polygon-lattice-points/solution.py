# Polygon Lattice Points - CSES 2193
# https://cses.fi/problemset/task/2193
#
# Pick's theorem: A = i + b/2 - 1  =>  i = A - b/2 + 1
#   b = sum over edges of gcd(|dx|, |dy|)  (boundary lattice points)
#   2A = |sum (x_k*y_{k+1} - x_{k+1}*y_k)|  (shoelace, doubled area)
# All arithmetic is exact integer arithmetic (Python big ints), no floats.

import sys
from math import gcd


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    xs = [0] * n
    ys = [0] * n
    for k in range(n):
        xs[k] = int(data[idx]); idx += 1
        ys[k] = int(data[idx]); idx += 1

    two_area = 0          # doubled signed area (shoelace)
    boundary = 0          # b: lattice points on the boundary

    for k in range(n):
        x1 = xs[k]
        y1 = ys[k]
        j = k + 1 if k + 1 < n else 0
        x2 = xs[j]
        y2 = ys[j]

        two_area += x1 * y2 - x2 * y1

        dx = x2 - x1
        dy = y2 - y1
        if dx < 0:
            dx = -dx
        if dy < 0:
            dy = -dy
        boundary += gcd(dx, dy)

    if two_area < 0:
        two_area = -two_area

    # i = A - b/2 + 1 = (2A - b + 2) / 2 ; (2A - b) is always even (Pick).
    interior = (two_area - boundary + 2) // 2

    sys.stdout.write("%d %d\n" % (interior, boundary))


if __name__ == "__main__":
    main()
