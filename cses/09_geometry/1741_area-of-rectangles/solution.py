# Area of Rectangles - CSES 1741
# https://cses.fi/problemset/task/1741
#
# Union area of n axis-aligned rectangles via sweep line (x) + segment tree
# over compressed y coordinates that maintains the total covered length.
# All arithmetic is exact integer (coordinates up to 1e6, answer fits in int).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    nums = list(map(int, data))
    pos = 0
    n = nums[pos]; pos += 1

    X1 = [0] * n
    Y1 = [0] * n
    X2 = [0] * n
    Y2 = [0] * n
    ys_set = set()
    for i in range(n):
        x1 = nums[pos]; y1 = nums[pos + 1]; x2 = nums[pos + 2]; y2 = nums[pos + 3]
        pos += 4
        X1[i] = x1; Y1[i] = y1; X2[i] = x2; Y2[i] = y2
        ys_set.add(y1)
        ys_set.add(y2)

    # Coordinate compression of y.
    ys = sorted(ys_set)
    yidx = {v: i for i, v in enumerate(ys)}
    M = len(ys) - 1  # number of elementary y-intervals (>=1 since y1<y2 always)

    # Segment tree size (power of two >= M).
    sz = 1
    while sz < M:
        sz <<= 1

    # span[node] = physical y-length covered by the node's elementary intervals.
    span = [0] * (2 * sz)
    for i in range(M):
        span[sz + i] = ys[i + 1] - ys[i]
    for i in range(sz - 1, 0, -1):
        span[i] = span[2 * i] + span[2 * i + 1]

    # cnt[node] = how many active rectangles fully cover this node's range
    #            (lazy count, never pushed down).
    # cover[node] = measure of the covered part inside this node's range,
    #            considering cnt at this node and below (NOT ancestors).
    cnt = [0] * (2 * sz)
    cover = [0] * (2 * sz)

    # Build vertical events: at x1 add +1 on [y1,y2), at x2 add -1 on [y1,y2).
    events = []
    ap = events.append
    for i in range(n):
        li = yidx[Y1[i]]
        ri = yidx[Y2[i]]
        ap((X1[i], 1, li, ri))
        ap((X2[i], -1, li, ri))
    events.sort()

    area = 0
    ne = len(events)
    prev_x = events[0][0]
    j = 0
    while j < ne:
        x = events[j][0]
        # Strip [prev_x, x] is covered by the current union length cover[1].
        area += (x - prev_x) * cover[1]
        # Apply every event that shares this x.
        while j < ne and events[j][0] == x:
            _, val, l, r = events[j]
            j += 1
            # Range update on elementary-interval indices [l, r) by +val/-val,
            # maintaining cover with the local recurrence
            #   cover = span            if cnt > 0
            #   cover = 0               if leaf and cnt == 0
            #   cover = child0 + child1 otherwise
            l += sz
            r += sz
            ll = l
            rr = r
            while l < r:
                if l & 1:
                    c = cnt[l] + val
                    cnt[l] = c
                    if c > 0:
                        cover[l] = span[l]
                    elif l >= sz:
                        cover[l] = 0
                    else:
                        cover[l] = cover[2 * l] + cover[2 * l + 1]
                    l += 1
                if r & 1:
                    r -= 1
                    c = cnt[r] + val
                    cnt[r] = c
                    if c > 0:
                        cover[r] = span[r]
                    elif r >= sz:
                        cover[r] = 0
                    else:
                        cover[r] = cover[2 * r] + cover[2 * r + 1]
                l >>= 1
                r >>= 1
            # Recompute cover for all ancestors of the two boundaries up to root.
            k = ll >> 1
            while k >= 1:
                if cnt[k] > 0:
                    cover[k] = span[k]
                else:
                    cover[k] = cover[2 * k] + cover[2 * k + 1]
                k >>= 1
            k = (rr - 1) >> 1
            while k >= 1:
                if cnt[k] > 0:
                    cover[k] = span[k]
                else:
                    cover[k] = cover[2 * k] + cover[2 * k + 1]
                k >>= 1
        prev_x = x

    sys.stdout.write(str(area) + "\n")


main()
