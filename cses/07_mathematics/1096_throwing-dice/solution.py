import sys

MOD = 1_000_000_007


# Nhân hai ma trận, mọi phần tử lấy modulo MOD.
# Bỏ qua khi phần tử bằng 0 để tiết kiệm phép nhân.
def mat_mul(A, B):
    n = len(A)
    m = len(B[0])
    k = len(B)
    C = [[0] * m for _ in range(n)]
    for i in range(n):
        Ai = A[i]
        Ci = C[i]
        for t in range(k):
            a = Ai[t]
            if a:
                Bt = B[t]
                for j in range(m):
                    Ci[j] = (Ci[j] + a * Bt[j]) % MOD
    return C


# Lũy thừa ma trận bằng fast exponentiation (bình phương liên tiếp).
def mat_pow(M, e):
    n = len(M)
    # Khởi tạo ma trận đơn vị.
    R = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while e > 0:
        if e & 1:
            R = mat_mul(R, M)
        M = mat_mul(M, M)
        e >>= 1
    return R


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Trường hợp nhỏ (n <= 4): trả lời trực tiếp f(0..4).
    if n <= 4:
        print([0, 1, 2, 4, 8][n])
        return

    # Ma trận chuyển 6x6 của recurrence f(n) = f(n-1) + ... + f(n-6).
    M = [
        [1, 1, 1, 1, 1, 1],
        [1, 0, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 1, 0],
    ]
    # Vector trạng thái xuất phát v_5 = [f(5), f(4), f(3), f(2), f(1), f(0)].
    v5 = [[16], [8], [4], [2], [1], [1]]

    # v_n = M^(n-5) * v_5; đáp số là phần tử đầu tiên, tức f(n).
    P = mat_pow(M, n - 5)
    vn = mat_mul(P, v5)
    sys.stdout.write(str(vn[0][0] % MOD) + "\n")


main()
