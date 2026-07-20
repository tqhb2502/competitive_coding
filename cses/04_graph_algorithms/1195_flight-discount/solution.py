# Flight Discount - CSES 1195
# https://cses.fi/problemset/task/1195
#
# Dijkstra trên đồ thị trạng thái 2 lớp:
#   state = node * 2 + used, với used thuộc {0, 1} (đã dùng phiếu giảm giá hay chưa).
# Mỗi cạnh (u, v, w) sinh ra 3 dịch chuyển:
#   (u,0) -> (v,0) : giá w (chưa dùng phiếu)
#   (u,0) -> (v,1) : giá w // 2 (dùng phiếu ngay trên cạnh này)
#   (u,1) -> (v,1) : giá w (đã dùng phiếu rồi)

import sys
import heapq


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Danh sách kề: adj[u] = list các (v, w)
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        adj[a].append((b, c))

    INF = float('inf')
    # dist theo trạng thái phẳng: state = node * 2 + used
    dist = [INF] * (2 * (n + 1))
    start = 1 * 2 + 0
    dist[start] = 0
    # heap chứa (khoảng cách, node, used)
    pq = [(0, 1, 0)]

    while pq:
        d, u, used = heapq.heappop(pq)
        state = u * 2 + used
        if d > dist[state]:
            continue
        for v, w in adj[u]:
            # đi bình thường, giữ nguyên trạng thái used
            nd = d + w
            ns = v * 2 + used
            if nd < dist[ns]:
                dist[ns] = nd
                heapq.heappush(pq, (nd, v, used))
            # nếu chưa dùng phiếu, thử dùng phiếu trên cạnh này
            if used == 0:
                nd2 = d + (w >> 1)
                ns2 = v * 2 + 1
                if nd2 < dist[ns2]:
                    dist[ns2] = nd2
                    heapq.heappush(pq, (nd2, v, 1))

    ans = min(dist[n * 2 + 0], dist[n * 2 + 1])
    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
