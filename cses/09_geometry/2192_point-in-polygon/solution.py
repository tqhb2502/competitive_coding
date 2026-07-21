# Point in Polygon - CSES task 2192
# https://cses.fi/problemset/task/2192
#
# For each query point decide whether it is INSIDE / OUTSIDE / BOUNDARY of a
# simple polygon.
#
# Technique: ray casting (even-odd rule) with exact integer arithmetic, plus an
# on-edge test using the integer cross product (== 0) and a bounding-box check.

import sys


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    xs = [0] * n
    ys = [0] * n
    for i in range(n):
        xs[i] = int(data[idx]); idx += 1
        ys[i] = int(data[idx]); idx += 1

    out = []
    for _ in range(m):
        px = int(data[idx]); idx += 1
        py = int(data[idx]); idx += 1

        inside = False
        on_boundary = False

        x1 = xs[n - 1]
        y1 = ys[n - 1]
        for i in range(n):
            x2 = xs[i]
            y2 = ys[i]

            # --- BOUNDARY: is the point on segment (x1,y1)-(x2,y2)? ---
            # Cross product of the edge vector with (point - first vertex) == 0
            # means the three points are collinear; then check the bounding box.
            cross = (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1)
            if cross == 0:
                if min(x1, x2) <= px <= max(x1, x2) and \
                   min(y1, y2) <= py <= max(y1, y2):
                    on_boundary = True
                    break

            # --- RAY CASTING: shoot a ray toward +x, count crossings ---
            # Half-open trick: use (y1 > py) != (y2 > py) to handle vertices right.
            if (y1 > py) != (y2 > py):
                dy = y2 - y1
                # The edge meets the horizontal line y = py at x-coordinate:
                #   xint = x1 + (x2-x1)*(py-y1)/(y2-y1)
                # We need px < xint (the intersection lies to the right of P).
                # Cross-multiply to avoid floats, watching the sign of dy.
                lhs = (px - x1) * dy               # (px-x1)*(y2-y1)
                rhs = (x2 - x1) * (py - y1)         # (x2-x1)*(py-y1)
                if dy > 0:
                    if lhs < rhs:
                        inside = not inside
                else:  # dy < 0
                    if lhs > rhs:
                        inside = not inside

            x1 = x2
            y1 = y2

        if on_boundary:
            out.append("BOUNDARY")
        elif inside:
            out.append("INSIDE")
        else:
            out.append("OUTSIDE")

    sys.stdout.write("\n".join(out) + "\n")


solve()
