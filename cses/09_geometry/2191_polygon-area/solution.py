import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Đọc toàn bộ tọa độ các đỉnh (số nguyên, không dùng float để tránh sai số).
    xs = [0] * n
    ys = [0] * n
    idx = 1
    for i in range(n):
        xs[i] = int(data[idx])
        ys[i] = int(data[idx + 1])
        idx += 2

    # Công thức shoelace: cộng dồn cross product (x_i*y_{i+1} - x_{i+1}*y_i),
    # chỉ số lấy mod n. Python dùng big integer nên không bao giờ tràn số.
    total = 0
    for i in range(n):
        j = i + 1 if i + 1 < n else 0
        total += xs[i] * ys[j] - xs[j] * ys[i]

    # In 2a = trị tuyệt đối của tổng, luôn là số nguyên không âm.
    sys.stdout.write(str(abs(total)))


if __name__ == "__main__":
    main()
