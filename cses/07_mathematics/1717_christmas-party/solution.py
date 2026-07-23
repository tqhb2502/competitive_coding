import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 1_000_000_007

    # Trường hợp nhỏ: một người luôn phải nhận quà của chính mình -> D(1) = 0.
    if n == 1:
        sys.stdout.write("0\n")
        return

    # Khởi tạo hai giá trị liền trước: D(1) = 0 và D(2) = 1.
    d_prev2 = 0  # D(1)
    d_prev1 = 1  # D(2)
    if n == 2:
        sys.stdout.write("1\n")
        return

    # Tính lặp công thức truy hồi D(k) = (k-1) * (D(k-1) + D(k-2)) mod 10^9+7.
    for k in range(3, n + 1):
        d_cur = (k - 1) * (d_prev1 + d_prev2) % MOD
        d_prev2 = d_prev1
        d_prev1 = d_cur

    sys.stdout.write(str(d_prev1) + "\n")


main()
