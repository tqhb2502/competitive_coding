# Sum of Four Squares - https://cses.fi/problemset/task/3355
#
# Theo định lý Lagrange (Lagrange's four-square theorem), mọi số nguyên
# không âm n đều biểu diễn được thành tổng của 4 số chính phương:
#     n = a^2 + b^2 + c^2 + d^2.
# Ta dùng định lý ba số chính phương (Legendre's three-square theorem):
# số m biểu diễn được thành tổng 3 số chính phương khi và chỉ khi m KHÔNG
# có dạng 4^k * (8*t + 7).
#
# Chiến lược:
#  1) Khử hết các thừa số 4: n = 4^k * t với t không chia hết cho 4.
#     Nếu t = a^2+b^2+c^2+d^2 thì n = (2^k a)^2 + ... nên chỉ cần giải cho t
#     rồi nhân kết quả với p = 2^k.
#  2) Nếu t % 8 == 7 (dạng cấm duy nhất khi t không chia hết cho 4) thì
#     đặt d = 1, và t-1 luôn biểu diễn được thành 3 số chính phương.
#     Ngược lại d = 0 và t biểu diễn được thành 3 số chính phương.
#  3) three_squares(m): duyệt c từ isqrt(m) giảm dần, kiểm tra m - c^2 có là
#     tổng 2 số chính phương không (two-pointer O(sqrt)). Mật độ số là tổng
#     2 số chính phương (~1/sqrt(ln m)) nên chỉ cần vài lần thử là trúng.

import sys
from math import isqrt


def two_squares(m):
    # Tìm a <= b không âm với a*a + b*b == m, hoặc None. Two-pointer O(sqrt(m)).
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
    # m được giả thiết biểu diễn được thành tổng 3 số chính phương.
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
    # Khử thừa số 4.
    k = 0
    t = n
    while t % 4 == 0:
        t //= 4
        k += 1
    p = 1 << k  # 2^k
    if t % 8 == 7:
        a, b, c = three_squares(t - 1)
        d = 1
    else:
        a, b, c = three_squares(t)
        d = 0
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
