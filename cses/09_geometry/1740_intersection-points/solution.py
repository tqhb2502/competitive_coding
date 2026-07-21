# Intersection Points - CSES task 1740
# https://cses.fi/problemset/task/1740
#
# Các đoạn thẳng chỉ là ngang (horizontal) hoặc dọc (vertical).
# Không có hai đoạn song song cắt nhau, và không có endpoint nào là
# giao điểm. Vì vậy mỗi giao điểm là giao của đúng một đoạn ngang với
# một đoạn dọc, cắt nhau tại điểm trong (strict interior) của cả hai.
#
# Sweep line theo x từ trái sang phải + Binary Indexed Tree (BIT) đếm
# số đoạn ngang đang "hoạt động" có tọa độ y nằm trong [y1, y2] của
# đoạn dọc hiện tại. Tất cả dùng số nguyên (exact integer arithmetic).

import sys
from bisect import bisect_left, bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1

    horizontals = []  # (x_left, x_right, y)
    verticals = []    # (x, y_low, y_high)
    ys = []           # tất cả y của đoạn ngang (để nén tọa độ)

    for _ in range(n):
        x1 = int(data[pos]); y1 = int(data[pos + 1])
        x2 = int(data[pos + 2]); y2 = int(data[pos + 3])
        pos += 4
        if y1 == y2:
            # đoạn ngang: theo ràng buộc x1 <= x2
            horizontals.append((x1, x2, y1))
            ys.append(y1)
        else:
            # đoạn dọc: x1 == x2, theo ràng buộc y1 <= y2
            verticals.append((x1, y1, y2))

    # Nén tọa độ y của các đoạn ngang.
    ys = sorted(set(ys))
    m = len(ys)

    # Tạo events. type: add=0 < query=1 < remove=2 (thứ tự khi cùng x).
    # Nhờ đảm bảo không có endpoint là giao điểm nên thứ tự này an toàn.
    events = []
    ap = events.append

    for xl, xr, y in horizontals:
        r = bisect_left(ys, y) + 1  # rank 1-indexed trong BIT (1-based)
        ap((xl, 0, r, 0))
        ap((xr, 2, r, 0))

    for x, ylo, yhi in verticals:
        lo = bisect_left(ys, ylo)   # số phần tử < ylo
        hi = bisect_right(ys, yhi)  # số phần tử <= yhi
        if hi > lo:
            # range sum trên rank (lo+1 .. hi) = prefix(hi) - prefix(lo)
            ap((x, 1, lo, hi))

    events.sort()

    sz = m + 1
    tree = [0] * sz
    ans = 0

    for _x, typ, p, q in events:
        if typ == 1:
            s = 0
            i = q
            while i > 0:
                s += tree[i]
                i -= i & (-i)
            i = p
            while i > 0:
                s -= tree[i]
                i -= i & (-i)
            ans += s
        elif typ == 0:
            i = p
            while i < sz:
                tree[i] += 1
                i += i & (-i)
        else:  # typ == 2
            i = p
            while i < sz:
                tree[i] -= 1
                i += i & (-i)

    sys.stdout.write(str(ans) + "\n")


main()
