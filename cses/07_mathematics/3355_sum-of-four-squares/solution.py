import sys
from math import isqrt


def two_squares(m):
    # Tìm a <= b không âm với a*a + b*b == m, hoặc None. Kỹ thuật two-pointer
    # O(sqrt(m)): tăng lo nếu tổng nhỏ hơn m, giảm hi nếu lớn hơn.
    lo = 0
    hi = isqrt(m)
    while lo <= hi:
        s = lo * lo + hi * hi
        if s == m:
            return lo, hi
        if s < m:
            lo += 1
        else:
            hi -= 1
    return None


def three_squares(m):
    # m được giả thiết biểu diễn được thành tổng 3 số chính phương (không thuộc
    # dạng cấm của Legendre). Duyệt c giảm dần, kiểm tra m - c^2 có tách được
    # thành 2 số chính phương không.
    c = isqrt(m)
    while c >= 0:
        r = two_squares(m - c * c)
        if r is not None:
            a, b = r
            return a, b, c
        c -= 1
    return 0, 0, 0


def four_squares(n):
    if n == 0:
        return 0, 0, 0, 0
    # Bước 1 - khử hết thừa số 4: n = 4^k * t, hệ số nhân lại p = 2^k.
    k = 0
    t = n
    while t % 4 == 0:
        t //= 4
        k += 1
    p = 1 << k  # 2^k
    # Bước 2 - dạng cấm duy nhất còn lại khi t không chia hết cho 4 là t % 8 == 7:
    # tách d = 1 để phần dư t-1 luôn là tổng 3 số chính phương; ngược lại d = 0.
    if t % 8 == 7:
        a, b, c = three_squares(t - 1)
        d = 1
    else:
        a, b, c = three_squares(t)
        d = 0
    # Bước 3 - nhân lại nghiệm của t với p để được nghiệm của n.
    return a * p, b * p, c * p, d * p


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    t = int(data[0])
    out = []
    for i in range(1, t + 1):
        n = int(data[i])
        a, b, c, d = four_squares(n)
        out.append("%d %d %d %d" % (a, b, c, d))
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
