# Flight Routes Check - https://cses.fi/problemset/task/1682
# Kiểm tra đồ thị có hướng có strongly connected (liên thông mạnh) hay không.
# Ý tưởng: chọn đỉnh 1. Đồ thị liên thông mạnh <=> từ đỉnh 1 đi tới được mọi
# đỉnh trong đồ thị gốc VÀ từ đỉnh 1 đi tới được mọi đỉnh trong đồ thị đảo chiều.
import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    adj = [[] for _ in range(n + 1)]   # đồ thị gốc
    radj = [[] for _ in range(n + 1)]  # đồ thị đảo chiều
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        radj[b].append(a)

    def bfs(graph, start):
        visited = bytearray(n + 1)
        visited[start] = 1
        count = 1
        q = deque([start])
        while q:
            u = q.popleft()
            for v in graph[u]:
                if not visited[v]:
                    visited[v] = 1
                    count += 1
                    q.append(v)
        return visited, count

    out = []

    # (1) Từ đỉnh 1 có tới được mọi đỉnh trong đồ thị gốc?
    vis1, cnt1 = bfs(adj, 1)
    if cnt1 < n:
        for u in range(1, n + 1):
            if not vis1[u]:
                out.append("NO")
                out.append("%d %d" % (1, u))  # không đi được từ 1 tới u
                sys.stdout.write("\n".join(out) + "\n")
                return

    # (2) Từ đỉnh 1 có tới được mọi đỉnh trong đồ thị đảo chiều?
    # Tương đương: mọi đỉnh đều tới được đỉnh 1 trong đồ thị gốc.
    vis2, cnt2 = bfs(radj, 1)
    if cnt2 < n:
        for u in range(1, n + 1):
            if not vis2[u]:
                out.append("NO")
                out.append("%d %d" % (u, 1))  # u không tới được đỉnh 1
                sys.stdout.write("\n".join(out) + "\n")
                return

    sys.stdout.write("YES\n")


main()
