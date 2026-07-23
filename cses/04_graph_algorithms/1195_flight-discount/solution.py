import sys
import heapq


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Danh sách kề của đồ thị có hướng: adj[u] = list các (v, w).
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        adj[a].append((b, c))

    INF = float('inf')
    # Mảng dist phẳng theo trạng thái: state = node * 2 + used
    # (used = 0 nếu chưa dùng phiếu, used = 1 nếu đã dùng phiếu giảm giá).
    dist = [INF] * (2 * (n + 1))
    start = 1 * 2 + 0
    dist[start] = 0
    # Heap chứa (khoảng cách, node, used) cho Dijkstra trên đồ thị trạng thái.
    pq = [(0, 1, 0)]

    while pq:
        d, u, used = heapq.heappop(pq)
        state = u * 2 + used
        # Bỏ qua trạng thái cũ đã lỗi thời (lazy deletion).
        if d > dist[state]:
            continue
        for v, w in adj[u]:
            # Đi bình thường, giữ nguyên trạng thái used.
            nd = d + w
            ns = v * 2 + used
            if nd < dist[ns]:
                dist[ns] = nd
                heapq.heappush(pq, (nd, v, used))
            # Nếu chưa dùng phiếu, thử dùng phiếu trên cạnh này (giá w // 2).
            if used == 0:
                nd2 = d + (w >> 1)
                ns2 = v * 2 + 1
                if nd2 < dist[ns2]:
                    dist[ns2] = nd2
                    heapq.heappush(pq, (nd2, v, 1))

    # Đáp án: chi phí nhỏ nhất tới đỉnh n ở cả hai trạng thái.
    ans = min(dist[n * 2 + 0], dist[n * 2 + 1])
    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
