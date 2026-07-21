# Graph Paths I - https://cses.fi/problemset/task/1723
# Đếm số walk (đường đi có thể lặp cạnh) độ dài đúng k từ đỉnh 1 đến đỉnh n,
# kết quả modulo 1e9+7. Dùng matrix exponentiation của ma trận kề.
# Tối ưu bằng kỹ thuật "bignum packing": mỗi hàng ma trận được đóng gói thành
# một số nguyên lớn, mỗi phần tử chiếm SHIFT bit -> nhân ma trận nhanh hơn.

import sys


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    k = int(data[idx]); idx += 1

    MOD = 1_000_000_007

    # SHIFT = 67: mỗi ô (slot) rộng 67 bit.
    # Giá trị lớn nhất của một ô = n*(MOD-1)^2 <= 100*(1e9+6)^2 ~ 1.0e20 < 2^67.
    SHIFT = 67
    MASK = (1 << SHIFT) - 1

    # Ma trận kề: A[i][j] = số cạnh từ i sang j.
    A = [[0] * n for _ in range(n)]
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        A[a - 1][b - 1] += 1

    def mat_mult(X, Y):
        # Tính X*Y (mod MOD). Đóng gói các hàng của Y thành số nguyên lớn.
        Yenc = []
        for row in Y:
            e = 0
            for j in range(n - 1, -1, -1):
                e = (e << SHIFT) | row[j]
            Yenc.append(e)
        Z = []
        for i in range(n):
            Xi = X[i]
            acc = 0
            for kk in range(n):
                a = Xi[kk]
                if a:
                    acc += a * Yenc[kk]
            zi = [0] * n
            for j in range(n):
                zi[j] = (acc >> (j * SHIFT)) & MASK
                if zi[j] >= MOD:
                    zi[j] %= MOD
            Z.append(zi)
        return Z

    # Binary matrix exponentiation: result = A^k.
    # result khởi tạo = ma trận đơn vị.
    result = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    base = A
    e = k
    while e:
        if e & 1:
            result = mat_mult(result, base)
        e >>= 1
        if e:
            base = mat_mult(base, base)

    ans = result[0][n - 1] % MOD
    sys.stdout.write(str(ans) + "\n")


solve()
