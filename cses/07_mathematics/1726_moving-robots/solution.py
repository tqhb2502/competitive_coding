import sys


def main():
    data = sys.stdin.buffer.read().split()
    k = int(data[0])

    N = 8
    S = N * N

    # Danh sách láng giềng hợp lệ cho mỗi ô; bậc của ô = số hướng di chuyển được
    # (ô góc 2 hướng, ô cạnh 3 hướng, ô trong 4 hướng).
    neigh = [[] for _ in range(S)]
    for r in range(N):
        for c in range(N):
            i = r * N + c
            for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nr, nc = r + dr, c + dc
                if 0 <= nr < N and 0 <= nc < N:
                    neigh[i].append(nr * N + nc)

    # dist[i] = phân bố xác suất vị trí của robot xuất phát tại ô i; khởi tạo đặt toàn
    # bộ xác suất tại chính ô xuất phát.
    dist = [[0.0] * S for _ in range(S)]
    for i in range(S):
        dist[i][i] = 1.0

    # Mô phỏng random walk k bước: mỗi bước chia đều xác suất p tại ô u sang các láng
    # giềng (p/deg(u) cho mỗi ô kề) -> sau k bước dist[i][s] = p_k(i -> s).
    for _ in range(k):
        new = [[0.0] * S for _ in range(S)]
        for i in range(S):
            di = dist[i]
            ni = new[i]
            for u in range(S):
                p = di[u]
                if p:
                    nb = neigh[u]
                    share = p / len(nb)
                    for v in nb:
                        ni[v] += share
        dist = new

    # Linearity of expectation: đáp số = tổng P(ô s rỗng) trên mọi ô s; các robot độc
    # lập nên P(ô s rỗng) = tích (1 - dist[i][s]) trên mỗi robot xuất phát i.
    ans = 0.0
    for s in range(S):
        prod = 1.0
        for i in range(S):
            prod *= (1.0 - dist[i][s])
        ans += prod

    sys.stdout.write("%.6f\n" % ans)


main()
