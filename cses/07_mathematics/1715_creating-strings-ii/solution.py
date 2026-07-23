import sys
from collections import Counter


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]
    n = len(s)

    MOD = 1_000_000_007

    # Tính trước giai thừa fact[i] = i! mod MOD.
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % MOD

    # Đáp án = n! / product(cnt[c]!) theo hệ số multinomial.
    # Chia cho giai thừa số lần xuất hiện của từng ký tự bằng modular inverse
    # (Fermat's little theorem).
    ans = fact[n]
    for c in Counter(s).values():
        ans = ans * pow(fact[c], MOD - 2, MOD) % MOD

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
