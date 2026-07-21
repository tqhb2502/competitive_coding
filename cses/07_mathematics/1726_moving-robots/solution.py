# Moving Robots - https://cses.fi/problemset/task/1726
# Bàn cờ 8x8, mỗi ô ban đầu có 1 robot (tổng 64 robot). Mỗi robot di chuyển k bước,
# mỗi bước chọn đều một hướng hợp lệ (không ra ngoài bàn). Tính kỳ vọng số ô rỗng sau k bước.
#
# Linearity of expectation: E[số ô rỗng] = sum_{s} P(ô s rỗng).
# Các robot di chuyển độc lập, nên P(ô s rỗng) = prod_{i} (1 - p_k(i -> s)),
# với p_k(i -> s) = xác suất robot xuất phát tại ô i kết thúc tại ô s sau k bước.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    k = int(data[0])

    N = 8
    S = N * N

    # Danh sách láng giềng hợp lệ cho mỗi ô (bậc của ô = số hướng di chuyển được).
    neigh = [[] for _ in range(S)]
    for r in range(N):
        for c in range(N):
            i = r * N + c
            for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nr, nc = r + dr, c + dc
                if 0 <= nr < N and 0 <= nc < N:
                    neigh[i].append(nr * N + nc)

    # dist[i] = phân bố xác suất vị trí của robot xuất phát tại ô i sau (đang làm) bước.
    dist = [[0.0] * S for _ in range(S)]
    for i in range(S):
        dist[i][i] = 1.0

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

    # P(ô s rỗng) = tích (1 - dist[i][s]) trên mỗi robot xuất phát i.
    ans = 0.0
    for s in range(S):
        prod = 1.0
        for i in range(S):
            prod *= (1.0 - dist[i][s])
        ans += prod

    sys.stdout.write("%.6f\n" % ans)


main()
