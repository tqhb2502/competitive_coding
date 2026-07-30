import sys
from math import gcd


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Đọc n đỉnh nguyên của đa giác đơn.
    xs = [0] * n
    ys = [0] * n
    for k in range(n):
        xs[k] = int(data[idx]); idx += 1
        ys[k] = int(data[idx]); idx += 1

    two_area = 0          # 2A: diện tích gấp đôi (shoelace), số nguyên chính xác
    boundary = 0          # b: số điểm nguyên trên biên

    # Duyệt từng cạnh (đỉnh k tới đỉnh k+1, cạnh cuối vòng về đỉnh 0).
    for k in range(n):
        x1 = xs[k]
        y1 = ys[k]
        j = k + 1 if k + 1 < n else 0
        x2 = xs[j]
        y2 = ys[j]

        # Cộng dồn shoelace cho diện tích gấp đôi.
        two_area += x1 * y2 - x2 * y1

        # Số điểm nguyên trên cạnh (bỏ đầu mút) = gcd(|dx|, |dy|).
        dx = x2 - x1
        dy = y2 - y1
        if dx < 0:
            dx = -dx
        if dy < 0:
            dy = -dy
        boundary += gcd(dx, dy)

    # Lấy trị tuyệt đối để không phụ thuộc chiều duyệt đa giác.
    if two_area < 0:
        two_area = -two_area

    # Định lý Pick: i = A - b/2 + 1 = (2A - b + 2) // 2 (tử số luôn chẵn).
    interior = (two_area - boundary + 2) // 2

    sys.stdout.write("%d %d\n" % (interior, boundary))


if __name__ == "__main__":
    main()
