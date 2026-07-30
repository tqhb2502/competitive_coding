import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    coords = data[1:1 + 2 * n]
    xs = list(map(int, coords[0::2]))
    ys = list(map(int, coords[1::2]))
    pts = sorted(zip(xs, ys))  # sắp xếp các điểm theo (x, y)

    if n <= 1:
        out = [str(n)]
        out.extend("%d %d" % (x, y) for x, y in pts)
        sys.stdout.write("\n".join(out) + "\n")
        return

    # Lower hull (duyệt trái -> phải): GIỮ LẠI điểm collinear -> chỉ pop khi
    # cross < 0 (góc quẹo phải thực sự).
    # Tối ưu hằng số: theo dõi độ dài bằng m, unpack đỉnh stack ra biến local.
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
            # cross(a, b, p) = (b - a) x (p - a), số nguyên nên chính xác tuyệt đối
            if (bx - ax) * (py - ay) - (by - ay) * (px - ax) < 0:
                lpop()
                m -= 1
            else:
                break
        lapp(p)
        m += 1

    # Upper hull: duyệt ngược (phải -> trái), cùng điều kiện pop cross < 0.
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

    # Bỏ điểm cuối của mỗi chain (trùng với đỉnh đầu của chain kia) rồi ghép lại.
    hull = lower[:-1] + upper[:-1]

    out = [str(len(hull))]
    out.extend("%d %d" % (x, y) for x, y in hull)
    sys.stdout.write("\n".join(out) + "\n")


main()
