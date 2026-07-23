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

    # Bước 1: xáo trộn ngẫu nhiên để phá mọi trường hợp xấu (kỳ vọng O(n)).
    random.shuffle(pts)

    # Bước 2: best = bình phương khoảng cách của hai điểm đầu tiên.
    ax, ay = pts[0]
    bx, by = pts[1]
    best = (ax - bx) ** 2 + (ay - by) ** 2
    # Bước 3: kích thước ô lưới s = isqrt(best) (số nguyên, s >= 1 vì best >= 1).
    s = isqrt(best) or 1

    def build(upto):
        # Dựng lại grid từ các điểm pts[0..upto] với kích thước ô s hiện tại.
        # Grid là dict lồng nhau: cột cx -> (cy -> danh sách điểm).
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

    # Bước 4: chèn từng điểm còn lại theo kiểu incremental.
    for i in range(2, n):
        x, y = pts[i]
        cx = x // s
        cy = y // s
        improved = False
        # Chỉ cần duyệt 3x3 ô quanh ô của điểm (xem chứng minh trong idea.txt).
        for gx in (cx - 1, cx, cx + 1):
            col = gget(gx)
            if col:
                cg = col.get
                for gy in (cy - 1, cy, cy + 1):
                    bucket = cg(gy)
                    if bucket:
                        # So sánh bằng số học nguyên chính xác dx*dx + dy*dy.
                        for qx, qy in bucket:
                            ddx = x - qx
                            ddy = y - qy
                            d2 = ddx * ddx + ddy * ddy
                            if d2 < best:
                                best = d2
                                improved = True
        if improved:
            # best giảm: tính lại kích thước ô và rebuild toàn bộ grid.
            s = isqrt(best) or 1
            grid = build(i)
            gget = grid.get
        else:
            # Không cải thiện: chỉ chèn điểm vào ô của nó.
            col = gget(cx)
            if col is None:
                grid[cx] = {cy: [(x, y)]}
            else:
                col.setdefault(cy, []).append((x, y))

    sys.stdout.write(str(best) + "\n")


main()
