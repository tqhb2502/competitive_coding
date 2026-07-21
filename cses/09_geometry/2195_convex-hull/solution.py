# Convex Hull - https://cses.fi/problemset/task/2195
#
# Yêu cầu: in ra TẤT CẢ các điểm nằm trên bao lồi (convex hull), tức là
# cả các đỉnh (vertices) lẫn các điểm nằm TRÊN CẠNH (collinear boundary points).
#
# Thuật toán: Andrew's monotone chain. Để GIỮ LẠI các điểm collinear trên cạnh,
# điều kiện pop chỉ bỏ điểm khi tạo góc quẹo phải THỰC SỰ (strict right turn),
# nghĩa là cross < 0 (thay vì <= 0 như bản convex hull tối thiểu).
# Tất cả số học dùng cross product với số nguyên (exact integer arithmetic),
# không dùng float -> không bao giờ sai số.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    coords = data[1:1 + 2 * n]
    xs = list(map(int, coords[0::2]))
    ys = list(map(int, coords[1::2]))
    pts = sorted(zip(xs, ys))  # sắp xếp theo (x, y)

    if n <= 1:
        out = [str(n)]
        out.extend("%d %d" % (x, y) for x, y in pts)
        sys.stdout.write("\n".join(out) + "\n")
        return

    # Lower hull: giữ lại điểm collinear -> pop khi cross < 0.
    # (Tối ưu hằng số: theo dõi độ dài bằng m, unpack ra biến local cho phép tính.)
    lower = []
    m = 0
    lpop = lower.pop
    lapp = lower.append
    for p in pts:
        px = p[0]
        py = p[1]
        while m >= 2:
            ax, ay = lower[m - 2]
            bx, by = lower[m - 1]
            # cross(a, b, p) = (b-a) x (p-a)
            if (bx - ax) * (py - ay) - (by - ay) * (px - ax) < 0:
                lpop()
                m -= 1
            else:
                break
        lapp(p)
        m += 1

    # Upper hull: duyệt ngược, cùng điều kiện cross < 0.
    upper = []
    m = 0
    upop = upper.pop
    uapp = upper.append
    for p in reversed(pts):
        px = p[0]
        py = p[1]
        while m >= 2:
            ax, ay = upper[m - 2]
            bx, by = upper[m - 1]
            if (bx - ax) * (py - ay) - (by - ay) * (px - ax) < 0:
                upop()
                m -= 1
            else:
                break
        uapp(p)
        m += 1

    # Bỏ điểm cuối của mỗi chain (trùng với đỉnh đầu của chain kia).
    hull = lower[:-1] + upper[:-1]

    out = [str(len(hull))]
    out.extend("%d %d" % (x, y) for x, y in hull)
    sys.stdout.write("\n".join(out) + "\n")


main()
