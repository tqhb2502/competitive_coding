# Line Segment Intersection - https://cses.fi/problemset/task/2190
# Xác định hai đoạn thẳng có giao nhau hay không (có chung ít nhất một điểm).
# Dùng cross product (orientation) với số nguyên chính xác tuyệt đối.

import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    t = data[0]
    i = 1
    out = []
    ap = out.append

    for _ in range(t):
        x1 = data[i]; y1 = data[i + 1]
        x2 = data[i + 2]; y2 = data[i + 3]
        x3 = data[i + 4]; y3 = data[i + 5]
        x4 = data[i + 6]; y4 = data[i + 7]
        i += 8

        # Vector chỉ phương của mỗi đoạn (tính trước để tái sử dụng).
        ax = x4 - x3; ay = y4 - y3   # p3 -> p4
        bx = x2 - x1; by = y2 - y1   # p1 -> p2

        # cross((p2-p1) x (q-p1)) — orientation of q relative to a directed line.
        # d1, d2: vị trí của p1, p2 so với đường thẳng p3p4.
        # d3, d4: vị trí của p3, p4 so với đường thẳng p1p2.
        d1 = ax * (y1 - y3) - ay * (x1 - x3)  # p1 vs line p3p4
        d2 = ax * (y2 - y3) - ay * (x2 - x3)  # p2 vs line p3p4
        d3 = bx * (y3 - y1) - by * (x3 - x1)  # p3 vs line p1p2
        d4 = bx * (y4 - y1) - by * (x4 - x1)  # p4 vs line p1p2

        # Trường hợp giao nhau "thật sự" (proper): p1, p2 nằm khác phía đối với
        # đường thẳng p3p4 VÀ p3, p4 nằm khác phía đối với đường thẳng p1p2.
        if ((d1 > 0) != (d2 > 0)) and d1 != 0 and d2 != 0 and \
           ((d3 > 0) != (d4 > 0)) and d3 != 0 and d4 != 0:
            ap("YES")
            continue

        # Trường hợp collinear / chạm đầu mút: một điểm thẳng hàng (d == 0)
        # và nằm trong bounding box của đoạn kia (kiểm tra on-segment).
        if d1 == 0 and min(x3, x4) <= x1 <= max(x3, x4) and min(y3, y4) <= y1 <= max(y3, y4):
            ap("YES")
        elif d2 == 0 and min(x3, x4) <= x2 <= max(x3, x4) and min(y3, y4) <= y2 <= max(y3, y4):
            ap("YES")
        elif d3 == 0 and min(x1, x2) <= x3 <= max(x1, x2) and min(y1, y2) <= y3 <= max(y1, y2):
            ap("YES")
        elif d4 == 0 and min(x1, x2) <= x4 <= max(x1, x2) and min(y1, y2) <= y4 <= max(y1, y2):
            ap("YES")
        else:
            ap("NO")

    sys.stdout.write("\n".join(out) + "\n")


main()
