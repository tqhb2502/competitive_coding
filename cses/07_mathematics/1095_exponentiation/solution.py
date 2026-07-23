import sys


def main():
    # Đọc toàn bộ input một lần để tối ưu I/O khi số truy vấn lớn.
    data = sys.stdin.buffer.read().split()
    MOD = 1_000_000_007
    n = int(data[0])
    out = []
    idx = 1
    for _ in range(n):
        a = int(data[idx])
        b = int(data[idx + 1])
        idx += 2
        # pow(a, b, MOD) dùng binary exponentiation dựng sẵn, chạy O(log b);
        # quy ước 0^0 = 1 và pow(0, 0, MOD) cũng trả về 1 nên khớp yêu cầu.
        out.append(str(pow(a, b, MOD)))
    # Ghi toàn bộ kết quả một lần.
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
