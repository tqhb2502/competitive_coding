# Bracket Sequences I - CSES 2064
# https://cses.fi/problemset/task/2064
# Đếm số dãy ngoặc hợp lệ độ dài n = số Catalan thứ n/2 (mod 1e9+7).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    MOD = 1_000_000_007

    # Nếu n lẻ thì không thể có dãy ngoặc hợp lệ.
    if n % 2 == 1:
        sys.stdout.write("0\n")
        return

    m = n // 2  # số cặp ngoặc

    # Precompute factorials 0..n mod MOD.
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % MOD

    # Inverse factorials via Fermat's little theorem + backward recurrence.
    inv_fact = [1] * (n + 1)
    inv_fact[n] = pow(fact[n], MOD - 2, MOD)
    for i in range(n, 0, -1):
        inv_fact[i - 1] = inv_fact[i] * i % MOD

    # Catalan(m) = (2m)! / (m! * (m+1)!) ; lưu ý 2m = n.
    ans = fact[n] * inv_fact[m] % MOD * inv_fact[m + 1] % MOD

    sys.stdout.write(str(ans) + "\n")


main()
