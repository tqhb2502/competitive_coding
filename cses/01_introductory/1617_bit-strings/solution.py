import sys


def main():
    # Đọc số nguyên n từ đầu vào
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    MOD = 10 ** 9 + 7

    # Số chuỗi bit độ dài n bằng 2^n mod (10^9 + 7);
    # pow(2, n, MOD) dùng lũy thừa nhanh modulo có sẵn, chạy O(log n)
    sys.stdout.write(str(pow(2, n, MOD)))


if __name__ == "__main__":
    main()
