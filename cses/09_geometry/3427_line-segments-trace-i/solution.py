# Line Segments Trace I
# https://cses.fi/problemset/task/3427
#
# Each segment joins (0, y1) - (m, y2) with integer slope, so treat it as a full
# line f(x) = s*x + b, where b = y1 and s = (y2 - y1) // m (exact division).
# We need the max of these lines at x = 0..m => the upper envelope via a monotone
# Convex Hull Trick, using exact integer arithmetic (cross product) and a moving
# pointer for the queries.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # slope -> largest intercept
    best = {}
    for _ in range(n):
        y1 = int(data[idx]); y2 = int(data[idx + 1]); idx += 2
        s = (y2 - y1) // m          # integer slope, exact division
        b = y1                      # value at x = 0
        cur = best.get(s)
        if cur is None or b > cur:
            best[s] = b

    # sort by increasing slope
    lines = sorted(best.items())    # [(slope, intercept), ...]

    # build the upper hull for MAX
    hm = []   # slopes
    hc = []   # intercepts
    for s, c in lines:
        while len(hm) >= 2:
            m1 = hm[-2]; c1 = hc[-2]
            m2 = hm[-1]; c2 = hc[-1]
            # pop l2 if intersection(l2,l3) <= intersection(l1,l2):
            # (c2 - c)*(m2 - m1) <= (c1 - c2)*(s - m2)
            if (c2 - c) * (m2 - m1) <= (c1 - c2) * (s - m2):
                hm.pop(); hc.pop()
            else:
                break
        hm.append(s); hc.append(c)

    # answer queries x = 0..m with a forward-moving pointer
    L = len(hm)
    out = []
    ptr = 0
    for x in range(m + 1):
        while ptr + 1 < L and hm[ptr + 1] * x + hc[ptr + 1] >= hm[ptr] * x + hc[ptr]:
            ptr += 1
        out.append(hm[ptr] * x + hc[ptr])

    sys.stdout.write(' '.join(map(str, out)))
    sys.stdout.write('\n')


main()
