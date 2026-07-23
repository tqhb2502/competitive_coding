import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    # Danh sách cung cho mạng luồng.
    to = []
    cap = []
    graph = [[] for _ in range(n + 1)]
    edges = []  # các con đường vô hướng gốc (a, b)

    def add_edge(u, v, c):
        # Cạnh vô hướng: cả hai chiều đều có capacity c, mỗi chiều là reverse
        # (residual) của chiều kia. Chỉ số liên tiếp nên eid ^ 1 là cung ngược.
        graph[u].append(len(to)); to.append(v); cap.append(c)
        graph[v].append(len(to)); to.append(u); cap.append(c)

    # Đọc các con đường và dựng mạng luồng với capacity 1 cho mỗi cạnh.
    for _ in range(m):
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        edges.append((a, b))
        add_edge(a, b, 1)

    s, t = 1, n

    level = [-1] * (n + 1)
    it = [0] * (n + 1)

    # BFS phân tầng: gán level theo khoảng cách từ s trong residual graph.
    def bfs():
        for i in range(n + 1):
            level[i] = -1
        level[s] = 0
        q = deque([s])
        while q:
            u = q.popleft()
            for eid in graph[u]:
                v = to[eid]
                if cap[eid] > 0 and level[v] < 0:
                    level[v] = level[u] + 1
                    q.append(v)
        return level[t] >= 0

    # DFS đẩy luồng theo blocking flow trên đồ thị phân tầng.
    def dfs(u, f):
        if u == t:
            return f
        gu = graph[u]
        while it[u] < len(gu):
            eid = gu[it[u]]
            v = to[eid]
            if cap[eid] > 0 and level[v] == level[u] + 1:
                d = dfs(v, f if f < cap[eid] else cap[eid])
                if d > 0:
                    # Cập nhật residual: giảm cung xuôi, tăng cung ngược.
                    cap[eid] -= d
                    cap[eid ^ 1] += d
                    return d
            it[u] += 1
        return 0

    sys.setrecursionlimit(10000)

    # Chạy Dinic: lặp BFS phân tầng rồi đẩy blocking flow tới khi hết luồng.
    INF = float('inf')
    while bfs():
        for i in range(n + 1):
            it[i] = 0
        while dfs(s, INF) > 0:
            pass

    # Tập đỉnh còn tới được từ s trong residual graph -> một phía của min cut.
    visited = [False] * (n + 1)
    visited[s] = True
    q = deque([s])
    while q:
        u = q.popleft()
        for eid in graph[u]:
            v = to[eid]
            if cap[eid] > 0 and not visited[v]:
                visited[v] = True
                q.append(v)

    # Con đường cần đóng: cạnh có đúng một đầu thuộc tập S (nằm trên lát cắt).
    cut = [(a, b) for (a, b) in edges if visited[a] != visited[b]]

    out = [str(len(cut))]
    out.extend("%d %d" % (a, b) for (a, b) in cut)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
