# Distinct Routes - https://cses.fi/problemset/task/1711
# Bài toán: tìm số đường đi edge-disjoint tối đa từ phòng 1 đến phòng n,
# mỗi teleporter (cạnh) chỉ được dùng nhiều nhất 1 lần.
# => max-flow với mỗi cạnh có capacity = 1, sau đó decompose thành các đường đi.
import sys
from collections import deque


def main():
    sys.setrecursionlimit(10000)
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Dinic's max flow. Các cạnh lưu theo cặp (e, e^1): forward và reverse.
    to = []
    cap = []
    graph = [[] for _ in range(n + 1)]

    def add_edge(u, v, c):
        graph[u].append(len(to)); to.append(v); cap.append(c)
        graph[v].append(len(to)); to.append(u); cap.append(0)

    # Lưu chỉ số cạnh forward của từng teleporter để tái tạo luồng về sau.
    edge_ab = []  # (a, b, forward_edge_index)
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        eidx = len(to)
        add_edge(a, b, 1)
        edge_ab.append((a, b, eidx))

    s, t = 1, n
    level = [-1] * (n + 1)
    it = [0] * (n + 1)

    def bfs():
        for i in range(n + 1):
            level[i] = -1
        level[s] = 0
        q = deque([s])
        while q:
            u = q.popleft()
            for ei in graph[u]:
                v = to[ei]
                if cap[ei] > 0 and level[v] < 0:
                    level[v] = level[u] + 1
                    q.append(v)
        return level[t] >= 0

    def dfs(u, f):
        if u == t:
            return f
        while it[u] < len(graph[u]):
            ei = graph[u][it[u]]
            v = to[ei]
            if cap[ei] > 0 and level[v] == level[u] + 1:
                d = dfs(v, f if f < cap[ei] else cap[ei])
                if d > 0:
                    cap[ei] -= d
                    cap[ei ^ 1] += d
                    return d
            it[u] += 1
        return 0

    INF = float('inf')
    flow = 0
    while bfs():
        for i in range(n + 1):
            it[i] = 0
        f = dfs(s, INF)
        while f:
            flow += f
            f = dfs(s, INF)

    # Tái tạo các đường đi: xây dựng đồ thị chỉ gồm các cạnh mang luồng (net flow = 1).
    flowadj = [[] for _ in range(n + 1)]
    for a, b, eidx in edge_ab:
        # capacity ban đầu = 1, nếu còn lại 0 nghĩa là có 1 đơn vị luồng chạy qua.
        if cap[eidx] == 0:
            flowadj[a].append(b)

    ptr = [0] * (n + 1)
    out = [str(flow)]
    for _ in range(flow):
        path = [s]
        pos = {s: 0}          # node -> vị trí trong path (để cắt bỏ chu trình)
        node = s
        while node != t:
            nxt = flowadj[node][ptr[node]]
            ptr[node] += 1
            if nxt in pos:
                # Gặp lại node đã có trên path => có chu trình, cắt bỏ phần chu trình.
                i = pos[nxt]
                for nd in path[i + 1:]:
                    del pos[nd]
                del path[i + 1:]
                node = nxt
            else:
                path.append(nxt)
                pos[nxt] = len(path) - 1
                node = nxt
        out.append(str(len(path)))
        out.append(' '.join(map(str, path)))

    sys.stdout.write('\n'.join(out) + '\n')


main()
