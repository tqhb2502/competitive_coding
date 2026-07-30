import sys

MOD = 1000000007


# Nhân hai ma trận 2x2 (biểu diễn phẳng a, b, c, d) theo modulo.
def mat_mult(X, Y):
    a, b, c, d = X
    e, f, g, h = Y
    return ((a * e + b * g) % MOD, (a * f + b * h) % MOD,
            (c * e + d * g) % MOD, (c * f + d * h) % MOD)


# Lũy thừa ma trận bằng bình phương liên tiếp trong O(log p).
def mat_pow(M, p):
    R = (1, 0, 0, 1)  # Ma trận đơn vị.
    while p:
        if p & 1:
            R = mat_mult(R, M)
        M = mat_mult(M, M)
        p >>= 1
    return R


# answer(n) = (A[n] + B[n]) mod p, với [A[n], B[n]]^T = M^(n-1) * [1, 1]^T
# và M = [[2, 1], [1, 4]] (nhân với [1, 1]^T là cộng hai cột lại).
def solve(n):
    a, b, c, d = mat_pow((2, 1, 1, 4), n - 1)
    A = (a + b) % MOD
    B = (c + d) % MOD
    return (A + B) % MOD


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    t = int(data[0])
    ns = data[1:1 + t]
    if not ns:
        return

    # Bộ nhớ đệm theo chiều cao để không tính lại các truy vấn trùng nhau.
    cache = {}
    out = []
    for x in ns:
        n = int(x)
        res = cache.get(n)
        if res is None:
            res = str(solve(n))
            cache[n] = res
        out.append(res)

    sys.stdout.write("\n".join(out) + "\n")


main()
