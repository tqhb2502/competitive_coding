import sys, math, random


# Kiểm tra nguyên tố tất định bằng Miller-Rabin với các base 2..37
# (đúng cho mọi số < 3.2*10^18, thừa sức cho 4n+1 <= 4e12).
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


# Tìm một ước không tầm thường của n bằng Pollard's rho (dò chu kỳ Floyd:
# x đi 1 bước, y đi 2 bước mỗi vòng lặp).
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


# Phân tích thừa số nguyên tố của n, cộng dồn số mũ vào từ điển res.
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
    # Đáp án 1: n là số tam giác  <=>  8n+1 là số chính phương (perfect square).
    s = 8 * n + 1
    r = math.isqrt(s)
    if r * r == s:
        return 1
    # Đáp án 2: 4n+1 là tổng của hai bình phương (sum of two squares)  <=>  mọi
    # ước nguyên tố p == 3 (mod 4) của 4n+1 đều có số mũ chẵn. Nếu vi phạm -> 3.
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
