# Minimum Euclidean Distance - CSES 2194
# https://cses.fi/problemset/task/2194
#
# Bài toán closest pair: in ra d^2 (bình phương khoảng cách Euclid nhỏ nhất)
# giữa hai điểm phân biệt bất kỳ.
#
# Thuật toán: randomized incremental với spatial hash grid (kỳ vọng O(n)).
# - Xáo trộn ngẫu nhiên thứ tự các điểm để phá vỡ các trường hợp xấu.
# - Duy trì "best" = bình phương khoảng cách nhỏ nhất hiện tại.
# - Kích thước ô lưới s = isqrt(best) là SỐ NGUYÊN, mỗi điểm được băm vào ô
#   (x // s, y // s). Grid là dict lồng nhau: cột cx -> (cy -> danh sách điểm)
#   để tra cứu bằng khóa số nguyên (nhanh hơn khóa tuple) và bỏ qua nhanh các
#   cột rỗng.
# - Khi thêm điểm mới p, chỉ cần duyệt 3x3 ô quanh ô của p. Nếu tìm được
#   khoảng cách nhỏ hơn -> cập nhật best, tính lại s, rebuild grid.
# - Mỗi phép so sánh dùng SỐ HỌC NGUYÊN CHÍNH XÁC (dx*dx + dy*dy) nên kết quả
#   tuyệt đối chính xác; grid chỉ là chỉ mục không gian để tỉa bớt.
#
# Tính đúng: vì s <= sqrt(best) và s là số nguyên, bất kỳ điểm q có
# dist(p,q) < best đều có |dx| < sqrt(best) < s+1 => |dx| <= s => chỉ số ô sai
# khác nhau nhiều nhất 1 => q nằm trong 3x3 ô => không bao giờ bỏ sót.

import sys
from math import isqrt
import random


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    pts = []
    ap = pts.append
    j = 1
    for _ in range(n):
        ap((int(data[j]), int(data[j + 1])))
        j += 2

    if n <= 1:
        sys.stdout.write("0\n")
        return

    random.shuffle(pts)

    ax, ay = pts[0]
    bx, by = pts[1]
    best = (ax - bx) ** 2 + (ay - by) ** 2
    s = isqrt(best) or 1  # s >= 1 (các điểm phân biệt nên best >= 1)

    def build(upto):
        # Dựng lại grid từ các điểm pts[0..upto] với kích thước ô s hiện tại.
        g = {}
        for t in range(upto + 1):
            px, py = pts[t]
            cx = px // s
            col = g.get(cx)
            if col is None:
                g[cx] = {py // s: [(px, py)]}
            else:
                col.setdefault(py // s, []).append((px, py))
        return g

    grid = build(1)
    gget = grid.get

    for i in range(2, n):
        x, y = pts[i]
        cx = x // s
        cy = y // s
        improved = False
        for gx in (cx - 1, cx, cx + 1):
            col = gget(gx)
            if col:
                cg = col.get
                for gy in (cy - 1, cy, cy + 1):
                    bucket = cg(gy)
                    if bucket:
                        for qx, qy in bucket:
                            ddx = x - qx
                            ddy = y - qy
                            d2 = ddx * ddx + ddy * ddy
                            if d2 < best:
                                best = d2
                                improved = True
        if improved:
            s = isqrt(best) or 1
            grid = build(i)
            gget = grid.get
        else:
            col = gget(cx)
            if col is None:
                grid[cx] = {cy: [(x, y)]}
            else:
                col.setdefault(cy, []).append((x, y))

    sys.stdout.write(str(best) + "\n")


main()
