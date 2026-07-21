# Binomial Coefficients - https://cses.fi/problemset/task/1079
# Tính n hệ số nhị thức C(a, b) mod 1e9+7 bằng factorial + inverse factorial (Fermat).
import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    MOD = 10**9 + 7
    MX = 10**6  # a lớn nhất theo ràng buộc

    # Precompute factorial mod p
    fact = [1] * (MX + 1)
    for i in range(1, MX + 1):
        fact[i] = fact[i - 1] * i % MOD

    # Inverse factorial: chỉ một lần pow (Fermat), sau đó truy hồi
    inv_fact = [1] * (MX + 1)
    inv_fact[MX] = pow(fact[MX], MOD - 2, MOD)
    for i in range(MX, 0, -1):
        inv_fact[i - 1] = inv_fact[i] * i % MOD

    out = []
    for _ in range(n):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        # 0 <= b <= a được bảo đảm bởi đề bài
        res = fact[a] * inv_fact[b] % MOD * inv_fact[a - b] % MOD
        out.append(res)

    sys.stdout.write("\n".join(map(str, out)) + "\n")


main()
