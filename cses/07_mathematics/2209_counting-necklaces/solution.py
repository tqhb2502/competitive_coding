# Counting Necklaces - https://cses.fi/problemset/task/2209
# Bổ đề Burnside trên nhóm cyclic C_n:
#   answer = (1/n) * sum_{d | n} phi(d) * m^(n/d)  (mod 1e9+7)
import sys


def euler_phi(x):
    # Tính Euler totient phi(x) bằng phân tích thừa số nguyên tố.
    result = x
    temp = x
    p = 2
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    MOD = 10 ** 9 + 7

    # Tìm tất cả ước d của n trong O(sqrt(n)).
    divisors = []
    i = 1
    while i * i <= n:
        if n % i == 0:
            divisors.append(i)
            if i != n // i:
                divisors.append(n // i)
        i += 1

    total = 0
    for d in divisors:
        total = (total + euler_phi(d) * pow(m, n // d, MOD)) % MOD

    # Chia cho n bằng modular inverse (Fermat, MOD nguyên tố).
    ans = total * pow(n, MOD - 2, MOD) % MOD
    sys.stdout.write(str(ans))


main()
