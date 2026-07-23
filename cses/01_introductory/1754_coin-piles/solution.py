import sys


def main():
    # Đọc toàn bộ đầu vào một lần để I/O nhanh (t có thể tới 1e5)
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    t = int(data[0])
    idx = 1
    out = []
    for _ in range(t):
        # a, b là số xu ở đống trái và đống phải của mỗi truy vấn
        a = int(data[idx])
        b = int(data[idx + 1])
        idx += 2
        # Đưa được về (0, 0) khi (a + b) chia hết 3 và hai đống đủ cân bằng
        if (a + b) % 3 == 0 and 2 * a >= b and 2 * b >= a:
            out.append("YES")
        else:
            out.append("NO")
    # Gom kết quả rồi ghi một lần để tránh chậm I/O
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
