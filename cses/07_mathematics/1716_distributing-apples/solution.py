# Distributing Apples - https://cses.fi/problemset/task/1716
# Stars and bars: so cach chia m qua tao cho n dua tre = C(n + m - 1, m) mod 1e9+7.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])

    MOD = 1_000_000_007

    # Can C(N, m) = C(N, N - m) voi N = n + m - 1 (luu y N - m = n - 1).
    N = n + m - 1
    # Chon chi so nho hon de it phep nhan nhat: k = min(m, n - 1).
    k = min(m, N - m)

    # Tinh truc tiep C(N, k) = (N * (N-1) * ... * (N-k+1)) / k! mod MOD.
    # Dung bien vo huong (khong cap phat mang lon) -> nhanh va it bo nho.
    num = 1
    for i in range(N - k + 1, N + 1):
        num = num * i % MOD

    den = 1
    for i in range(2, k + 1):
        den = den * i % MOD

    # MOD nguyen to => nghich dao theo Fermat: inverse(den) = den^(MOD-2).
    ans = num * pow(den, MOD - 2, MOD) % MOD

    sys.stdout.write(str(ans) + "\n")


main()
