import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])

    MOD = 1_000_000_007

    # Stars and bars: đáp án = C(N, m) với N = n + m - 1.
    N = n + m - 1
    # Vì C(N, m) = C(N, N - m) = C(N, n - 1), chọn chỉ số nhỏ hơn để ít phép nhân nhất.
    k = min(m, N - m)

    # Tính trực tiếp C(N, k) = (N * (N-1) * ... * (N-k+1)) / k! mod MOD.
    # Dùng biến vô hướng (không cấp phát mảng lớn) nên nhanh và ít tốn bộ nhớ.
    num = 1
    for i in range(N - k + 1, N + 1):
        num = num * i % MOD

    den = 1
    for i in range(2, k + 1):
        den = den * i % MOD

    # MOD nguyên tố nên nghịch đảo theo định lý Fermat: inverse(den) = den^(MOD-2).
    ans = num * pow(den, MOD - 2, MOD) % MOD

    sys.stdout.write(str(ans) + "\n")


main()
