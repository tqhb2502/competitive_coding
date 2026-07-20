# Counting Towers - CSES 2413
# https://cses.fi/problemset/task/2413
#
# DP theo cạnh trên cùng của tháp cao i:
#   A[i]: hàng trên cùng là một khối rộng 2 (connected)
#   B[i]: hàng trên cùng bị split (có vách ngăn dọc)
# Truy hồi:
#   A[i+1] = 2*A[i] + B[i]
#   B[i+1] = A[i] + 4*B[i]
# Base: A[1] = B[1] = 1 ; answer(n) = (A[n] + B[n]) % MOD
#
# Dạng ma trận: [A[i+1], B[i+1]]^T = M * [A[i], B[i]]^T với M = [[2,1],[1,4]].
# => [A[n], B[n]]^T = M^(n-1) * [1, 1]^T.
# Dùng lũy thừa ma trận O(log n) mỗi truy vấn nên tổng độ phức tạp O(t*log n),
# tránh vòng lặp O(10^6) của Python thuần (tránh nguy cơ TLE với giới hạn 1.00s).

import sys

MOD = 1000000007


def mat_mult(X, Y):
    a, b, c, d = X
    e, f, g, h = Y
    return ((a * e + b * g) % MOD, (a * f + b * h) % MOD,
            (c * e + d * g) % MOD, (c * f + d * h) % MOD)


def mat_pow(M, p):
    R = (1, 0, 0, 1)  # ma trận đơn vị
    while p:
        if p & 1:
            R = mat_mult(R, M)
        M = mat_mult(M, M)
        p >>= 1
    return R


def solve(n):
    a, b, c, d = mat_pow((2, 1, 1, 4), n - 1)
    # [A, B]^T = M^(n-1) * [1, 1]^T
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
