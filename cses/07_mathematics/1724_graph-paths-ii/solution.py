import sys
from operator import add


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1
    k = int(data[pos]); pos += 1

    INF = 1 << 62  # > giá trị thật lớn nhất = K * max_c <= 1e9 * 1e9 = 1e18

    # M[i][j] = trọng số cạnh nhỏ nhất từ i -> j (INF nếu không có cạnh)
    M = [[INF] * n for _ in range(n)]
    for _ in range(m):
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        c = int(data[pos]); pos += 1
        a -= 1; b -= 1
        if c < M[a][b]:
            M[a][b] = c

    def matmul(A, B):
        # min-plus product. Chuyển vị B để mỗi cell = min(map(add, hàng, cột))
        # chạy ở tốc độ C (không có vòng lặp Python ở lõi trong).
        BT = list(zip(*B))
        return [[min(map(add, Ai, col)) for col in BT] for Ai in A]

    def vecmul(v, B):
        # r[j] = min_i ( v[i] + B[i][j] ), O(n^2). Bỏ qua nguồn v[i] = INF.
        r = [INF] * n
        for i in range(n):
            a = v[i]
            if a < INF:
                Bi = B[i]
                r = [c if c < (s := a + x) else s for c, x in zip(r, Bi)]
        return r

    # v = e_0 (bắt đầu tại node 0). Sau khi nhân v * M^k trong (min, +):
    # v[j] = trọng số nhỏ nhất của walk 0->j dùng đúng k cạnh.
    v = [INF] * n
    v[0] = 0
    base = M
    e = k
    while e:
        if e & 1:
            v = vecmul(v, base)
        e >>= 1
        if e:
            base = matmul(base, base)

    ans = v[n - 1]
    sys.stdout.write("-1\n" if ans >= INF else str(ans) + "\n")


main()
