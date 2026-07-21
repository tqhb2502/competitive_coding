# Sum of Divisors - CSES 1082
# https://cses.fi/problemset/task/1082
#
# S = sum_{i=1..n} sigma(i) = sum_{d=1..n} d * floor(n/d)  (mod 1e9+7)
# Dùng kỹ thuật divisor-block (floor division blocks): O(sqrt(n)).

import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 10 ** 9 + 7

    ans = 0
    d = 1
    while d <= n:
        q = n // d              # thương floor(n/d) không đổi trên khối
        r = n // q              # chỉ số lớn nhất giữ nguyên thương q
        block_sum = (d + r) * (r - d + 1) // 2   # tổng cấp số cộng d..r
        ans = (ans + (q % MOD) * (block_sum % MOD)) % MOD
        d = r + 1

    sys.stdout.write(str(ans % MOD))


if __name__ == "__main__":
    main()
