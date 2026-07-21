# Creating Strings II - https://cses.fi/problemset/task/1715
# Answer = n! / (product of cnt[c]!) mod 1e9+7 (multinomial coefficient).
# Use precomputed factorials and Fermat's little theorem for modular inverse.

import sys
from collections import Counter


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]
    n = len(s)

    MOD = 1_000_000_007

    # Precompute factorials mod MOD up to n.
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % MOD

    ans = fact[n]
    # Chia cho giai thừa số lần xuất hiện của từng ký tự (multinomial).
    for c in Counter(s).values():
        ans = ans * pow(fact[c], MOD - 2, MOD) % MOD

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
