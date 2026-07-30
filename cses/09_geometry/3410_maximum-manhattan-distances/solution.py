import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Phép quay 45 độ: u = x + y, v = x - y. Duy trì running min/max của u, v
    # (dùng số nguyên chính xác của Python nên không lo tràn số).
    INF = float("inf")
    min_u = INF
    max_u = -INF
    min_v = INF
    max_v = -INF

    out = []
    for _ in range(n):
        x = int(data[idx]); y = int(data[idx + 1]); idx += 2
        # Cập nhật min/max của u và v sau khi thêm điểm mới.
        u = x + y
        v = x - y
        if u < min_u:
            min_u = u
        if u > max_u:
            max_u = u
        if v < min_v:
            min_v = v
        if v > max_v:
            max_v = v
        # Khoảng cách Manhattan lớn nhất = max(maxU - minU, maxV - minV).
        du = max_u - min_u
        dv = max_v - min_v
        out.append(str(du if du > dv else dv))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
