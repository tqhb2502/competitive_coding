# Range Interval Queries - https://cses.fi/problemset/task/3163
# Đếm 2D offline: số chỉ số i thỏa a<=i<=b và c<=x_i<=d.
# Fenwick tree (BIT) + nén tọa độ (coordinate compression) + sweep line theo chỉ số.
# answer = f(b,d) - f(b,c-1) - f(a-1,d) + f(a-1,c-1),
#   với f(i,v) = số j<=i có x_j<=v.

import sys
from bisect import bisect_left, bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    nums = list(map(int, data))
    pos = 0
    n = nums[pos]; q = nums[pos + 1]; pos += 2

    x = nums[pos:pos + n]
    pos += n

    sorted_vals = sorted(set(x))
    m = len(sorted_vals)

    # rank (1-indexed) của từng phần tử để point-update trên BIT
    ranks = [bisect_left(sorted_vals, v) + 1 for v in x]

    # gom sự kiện theo chỉ số i (bucket) để khỏi phải sort
    events = [[] for _ in range(n + 1)]
    ans = [0] * q

    for qi in range(q):
        a = nums[pos]; b = nums[pos + 1]; c = nums[pos + 2]; d = nums[pos + 3]
        pos += 4
        rd = bisect_right(sorted_vals, d)       # số giá trị <= d
        rc = bisect_right(sorted_vals, c - 1)   # số giá trị <= c-1
        eb = events[b]
        if rd:
            eb.append((rd, 1, qi))
        if rc:
            eb.append((rc, -1, qi))
        am1 = a - 1
        if am1 >= 1:
            ea = events[am1]
            if rd:
                ea.append((rd, -1, qi))
            if rc:
                ea.append((rc, 1, qi))

    # Fenwick tree (BIT)
    tree = [0] * (m + 1)

    for i in range(1, n + 1):
        # point update +1 tại rank của x_i
        r = ranks[i - 1]
        while r <= m:
            tree[r] += 1
            r += r & (-r)
        # xử lý các sự kiện tại chỉ số i
        ev = events[i]
        if ev:
            for rv, sign, qi in ev:
                s = 0
                rr = rv
                while rr > 0:
                    s += tree[rr]
                    rr -= rr & (-rr)
                ans[qi] += sign * s

    out = b'\n'.join(str(v).encode() for v in ans)
    sys.stdout.buffer.write(out)
    sys.stdout.buffer.write(b'\n')


main()
