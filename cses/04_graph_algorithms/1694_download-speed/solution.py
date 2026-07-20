# Download Speed - CSES 1694
# https://cses.fi/problemset/task/1694
# Luồng cực đại từ đỉnh 1 (source) tới đỉnh n (sink) bằng thuật toán Dinic.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    graph = [[] for _ in range(n + 1)]  # 1-indexed adjacency: list of edge ids
    to = []    # to[eid]  = đỉnh đích của cạnh
    cap = []   # cap[eid] = dung lượng còn lại (residual)

    def add_edge(u, v, c):
        graph[u].append(len(to))
        to.append(v)
        cap.append(c)
        graph[v].append(len(to))
        to.append(u)
        cap.append(0)

    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        c = int(data[idx]); idx += 1
        add_edge(a, b, c)

    s, t = 1, n

    # Trường hợp suy biến: source trùng sink.
    if s == t:
        sys.stdout.write("0\n")
        return

    level = [-1] * (n + 1)
    it = [0] * (n + 1)

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

    # DFS lặp (iterative) để tìm một augmenting path trong level graph,
    # tránh giới hạn đệ quy khi đường đi dài.
    def dfs_send():
        # tìm lượng đẩy được từ s tới t (trả về 0 nếu không còn)
        stack = [s]
        # edge_stack lưu chỉ số cạnh đã dùng để đi tới đỉnh hiện tại (để lùi residual)
        edge_stack = []
        while stack:
            u = stack[-1]
            if u == t:
                # tìm bottleneck dọc theo edge_stack
                f = min(cap[eid] for eid in edge_stack)
                for eid in edge_stack:
                    cap[eid] -= f
                    cap[eid ^ 1] += f
                return f
            advanced = False
            while it[u] < len(graph[u]):
                eid = graph[u][it[u]]
                v = to[eid]
                if cap[eid] > 0 and level[v] == level[u] + 1:
                    stack.append(v)
                    edge_stack.append(eid)
                    advanced = True
                    break
                it[u] += 1
            if not advanced:
                # ngõ cụt: bỏ đỉnh này khỏi level graph và lùi lại
                level[u] = -1
                stack.pop()
                if edge_stack:
                    edge_stack.pop()
        return 0

    flow = 0
    while bfs():
        for i in range(n + 1):
            it[i] = 0
        while True:
            f = dfs_send()
            if f == 0:
                break
            flow += f

    sys.stdout.write(str(flow) + "\n")


main()
