# Tree Diameter - CSES 1131
# https://cses.fi/problemset/task/1131
# Thuật toán: double BFS (two-sweep). BFS lần 1 từ node bất kỳ để tìm một
# đầu mút của đường kính, BFS lần 2 từ đầu mút đó để đo độ dài đường kính.
# Toàn bộ iterative (không đệ quy) để an toàn với cây tới 2*10^5 node.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        print(0)
        return

    idx = 0
    n = int(data[idx]); idx += 1

    # Adjacency list dạng CSR để duyệt nhanh và tiết kiệm bộ nhớ.
    # Bước 1: đếm bậc (degree) của mỗi node.
    deg = [0] * (n + 1)
    ea = [0] * (n - 1) if n > 1 else []
    eb = [0] * (n - 1) if n > 1 else []
    for i in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        ea[i] = a
        eb[i] = b
        deg[a] += 1
        deg[b] += 1

    # Bước 2: tính mảng offset (start) cho CSR.
    start = [0] * (n + 2)
    for v in range(1, n + 1):
        start[v + 1] = start[v] + deg[v]

    # Bước 3: điền các cạnh vào mảng adj.
    adj = [0] * (2 * (n - 1)) if n > 1 else []
    pos = start[:]  # con trỏ chèn hiện tại cho mỗi node
    for i in range(n - 1):
        a = ea[i]; b = eb[i]
        adj[pos[a]] = b; pos[a] += 1
        adj[pos[b]] = a; pos[b] += 1

    def bfs(src):
        dist = [-1] * (n + 1)
        dist[src] = 0
        q = deque([src])
        far_node = src
        far_dist = 0
        while q:
            u = q.popleft()
            du = dist[u]
            if du > far_dist:
                far_dist = du
                far_node = u
            for k in range(start[u], start[u + 1]):
                w = adj[k]
                if dist[w] == -1:
                    dist[w] = du + 1
                    q.append(w)
        return far_node, far_dist

    if n == 1:
        sys.stdout.buffer.write(b"0\n")
        return

    u, _ = bfs(1)
    _, diameter = bfs(u)

    sys.stdout.buffer.write((str(diameter) + "\n").encode())


if __name__ == "__main__":
    main()
