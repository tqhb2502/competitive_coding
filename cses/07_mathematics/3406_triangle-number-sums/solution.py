# Triangle Number Sums - CSES 3406
# https://cses.fi/problemset/task/3406
#
# Số tam giác (triangle number) T_k = k(k+1)/2.
# Theo Gauss's Eureka theorem: mọi số nguyên dương đều là tổng của nhiều nhất
# 3 số tam giác, nên đáp án luôn là 1, 2 hoặc 3.
#   đáp án 1  <=>  n là số tam giác  <=>  8n+1 là số chính phương (perfect square)
#   đáp án 2  <=>  n = T_a + T_b với a,b >= 1
#                  <=>  8n+2 = (2a+1)^2 + (2b+1)^2 là tổng của hai bình phương
#                  <=>  4n+1 là tổng của hai bình phương (sum of two squares)
#                  <=>  mọi ước nguyên tố p == 3 (mod 4) của 4n+1 có số mũ chẵn
#   đáp án 3  các trường hợp còn lại
#
# Phân tích thừa số 4n+1 (<= 4e12) bằng Miller-Rabin + Pollard's rho.

import sys, math, random


def is_prime(n):
    if n < 2:
        return False
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if n % p == 0:
            return n == p
    d = n - 1
    r = 0
    while d % 2 == 0:
        d //= 2
        r += 1
    for a in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        ok = False
        for _ in range(r - 1):
            x = x * x % n
            if x == n - 1:
                ok = True
                break
        if not ok:
            return False
    return True


def pollard(n):
    if n % 2 == 0:
        return 2
    if n % 3 == 0:
        return 3
    while True:
        c = random.randrange(1, n)
        x = y = random.randrange(2, n)
        d = 1
        while d == 1:
            x = (x * x + c) % n
            y = (y * y + c) % n
            y = (y * y + c) % n
            d = math.gcd(abs(x - y), n)
        if d != n:
            return d


def factor(n, res):
    if n == 1:
        return
    if is_prime(n):
        res[n] = res.get(n, 0) + 1
        return
    d = pollard(n)
    factor(d, res)
    factor(n // d, res)


def solve(n):
    # đáp án 1: n là số tam giác  <=>  8n+1 là số chính phương
    s = 8 * n + 1
    r = math.isqrt(s)
    if r * r == s:
        return 1
    # đáp án 2: 4n+1 là tổng của hai bình phương
    m = 4 * n + 1
    f = {}
    factor(m, f)
    for p, e in f.items():
        if p % 4 == 3 and (e & 1):
            return 3
    return 2


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(1, t + 1):
        out.append(str(solve(int(data[i]))))
    sys.stdout.write("\n".join(out) + "\n")


main()
