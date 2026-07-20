# Investigation - CSES 1202
# https://cses.fi/problemset/task/1202
#
# Dijkstra kết hợp DP trên shortest-path DAG để tính cùng lúc:
#   - chi phí nhỏ nhất từ 1 đến n
#   - số đường đi rẻ nhất (mod 1e9+7)
#   - số cạnh ít nhất / nhiều nhất trong một đường đi rẻ nhất.

import sys
import heapq


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    MOD = 10 ** 9 + 7

    # Đọc cạnh và đếm bậc ra để dựng danh sách kề dạng CSR.
    ea = [0] * m
    eb = [0] * m
    ec = [0] * m
    deg = [0] * (n + 2)
    for e in range(m):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        ea[e] = a; eb[e] = b; ec[e] = c
        deg[a] += 1

    start = [0] * (n + 2)
    for i in range(1, n + 1):
        start[i + 1] = start[i] + deg[i]

    adj_to = [0] * m
    adj_w = [0] * m
    pos = start[:]  # con trỏ chèn cho từng đỉnh
    for e in range(m):
        a = ea[e]
        p = pos[a]
        adj_to[p] = eb[e]
        adj_w[p] = ec[e]
        pos[a] = p + 1

    INF = float('inf')
    dist = [INF] * (n + 1)
    ways = [0] * (n + 1)
    minE = [0] * (n + 1)
    maxE = [0] * (n + 1)

    dist[1] = 0
    ways[1] = 1
    minE[1] = 0
    maxE[1] = 0

    pq = [(0, 1)]
    heappush = heapq.heappush
    heappop = heapq.heappop

    while pq:
        d, u = heappop(pq)
        if d > dist[u]:
            continue  # bản ghi cũ, bỏ qua
        wu = ways[u]
        miu = minE[u] + 1
        mau = maxE[u] + 1
        for i in range(start[u], start[u + 1]):
            v = adj_to[i]
            nd = d + adj_w[i]
            dv = dist[v]
            if nd < dv:
                dist[v] = nd
                ways[v] = wu
                minE[v] = miu
                maxE[v] = mau
                heappush(pq, (nd, v))
            elif nd == dv:
                ways[v] = (ways[v] + wu) % MOD
                if miu < minE[v]:
                    minE[v] = miu
                if mau > maxE[v]:
                    maxE[v] = mau

    sys.stdout.write(
        "%d %d %d %d\n" % (dist[n], ways[n] % MOD, minE[n], maxE[n])
    )


main()
