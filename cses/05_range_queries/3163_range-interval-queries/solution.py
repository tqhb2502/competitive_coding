import sys
from bisect import bisect_left, bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    nums = list(map(int, data))
    pos = 0
    n = nums[pos]; q = nums[pos + 1]; pos += 2

    x = nums[pos:pos + n]
    pos += n

    # Nén tọa độ: tập giá trị phân biệt đã sắp xếp
    sorted_vals = sorted(set(x))
    m = len(sorted_vals)

    # Rank (1-indexed) của từng phần tử để point update trên BIT
    ranks = [bisect_left(sorted_vals, v) + 1 for v in x]

    # events[i]: gom sự kiện theo chỉ số i (bucket) để khỏi phải sắp xếp
    events = [[] for _ in range(n + 1)]
    ans = [0] * q

    # Tách mỗi truy vấn [a, b] × [c, d] theo bao hàm - loại trừ:
    #   f(b, d) - f(b, c-1) - f(a-1, d) + f(a-1, c-1)
    for qi in range(q):
        a = nums[pos]; b = nums[pos + 1]; c = nums[pos + 2]; d = nums[pos + 3]
        pos += 4
        rd = bisect_right(sorted_vals, d)       # số giá trị ≤ d, tức prefix cần hỏi
        rc = bisect_right(sorted_vals, c - 1)   # số giá trị ≤ c-1
        # Hai số hạng tại chỉ số b
        eb = events[b]
        if rd:
            eb.append((rd, 1, qi))
        if rc:
            eb.append((rc, -1, qi))
        # Hai số hạng tại chỉ số a-1 (bỏ qua khi a-1 = 0 vì f = 0)
        am1 = a - 1
        if am1 >= 1:
            ea = events[am1]
            if rd:
                ea.append((rd, -1, qi))
            if rc:
                ea.append((rc, 1, qi))

    # Fenwick tree (BIT) trên rank giá trị đã nén
    tree = [0] * (m + 1)

    # Quét chỉ số i tăng dần
    for i in range(1, n + 1):
        # Point update +1 tại rank của x_i
        r = ranks[i - 1]
        while r <= m:
            tree[r] += 1
            r += r & (-r)
        # BIT đã chứa mọi phần tử chỉ số ≤ i nên prefix sum = f(i, rv)
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
