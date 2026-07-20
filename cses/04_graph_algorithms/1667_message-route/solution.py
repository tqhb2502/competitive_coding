# Message Route - https://cses.fi/problemset/task/1667
# BFS tìm đường đi ngắn nhất (ít máy tính nhất) từ đỉnh 1 đến đỉnh n
# trên đồ thị không trọng số, hai chiều; truy vết parent để dựng lại đường đi.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Adjacency list
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        adj[b].append(a)

    parent = [0] * (n + 1)   # 0 = chưa thăm
    parent[1] = 1            # đánh dấu đỉnh nguồn đã thăm (trỏ chính nó)

    q = deque([1])
    while q:
        u = q.popleft()
        if u == n:
            break
        for v in adj[u]:
            if parent[v] == 0:
                parent[v] = u
                q.append(v)

    if parent[n] == 0:
        sys.stdout.write("IMPOSSIBLE\n")
        return

    # Truy vết ngược từ n về 1
    path = []
    cur = n
    while cur != 1:
        path.append(cur)
        cur = parent[cur]
    path.append(1)
    path.reverse()

    out = [str(len(path)), " ".join(map(str, path))]
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
