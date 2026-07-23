import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    graph = [[] for _ in range(n + 1)]  # danh sách kề 1-indexed: mỗi phần tử là chỉ số cạnh
    to = []    # to[eid]  = đỉnh đích của cạnh
    cap = []   # cap[eid] = dung lượng còn lại (residual)

    # Thêm cạnh có hướng u -> v; cạnh nghịch (residual) capacity 0 đứng liền kề,
    # nên cạnh đối của eid chính là eid ^ 1.
    def add_edge(u, v, c):
        graph[u].append(len(to))
        to.append(v)
        cap.append(c)
        graph[v].append(len(to))
        to.append(u)
        cap.append(0)

    # Đọc m kết nối có hướng và dựng mạng luồng.
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

    # BFS dựng level graph; trả về True nếu vẫn tới được sink.
    def bfs():
        for i in range(n + 1):
            level[i] = -1
        level[s] = 0
        q = deque([s])
        while q:
            u = q.popleft()
            for eid in graph[u]:
                v = to[eid]
                # Chỉ đi theo cạnh còn dung lượng và tới đỉnh chưa gán level.
                if cap[eid] > 0 and level[v] < 0:
                    level[v] = level[u] + 1
                    q.append(v)
        return level[t] >= 0

    # DFS lặp (iterative) tìm một augmenting path trong level graph,
    # tránh giới hạn đệ quy khi đường đi dài.
    def dfs_send():
        stack = [s]
        # edge_stack lưu chỉ số cạnh đã dùng để đi tới đỉnh hiện tại (để lùi residual).
        edge_stack = []
        while stack:
            u = stack[-1]
            if u == t:
                # Tìm bottleneck dọc theo đường đi rồi cập nhật residual.
                f = min(cap[eid] for eid in edge_stack)
                for eid in edge_stack:
                    cap[eid] -= f
                    cap[eid ^ 1] += f
                return f
            advanced = False
            # it[u] ghi nhớ vị trí đang xét để bỏ qua các cạnh đã bão hòa.
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
                # Ngõ cụt: loại đỉnh này khỏi level graph và lùi lại một bước.
                level[u] = -1
                stack.pop()
                if edge_stack:
                    edge_stack.pop()
        return 0

    # Vòng lặp Dinic: dựng level graph rồi đẩy hết blocking flow.
    flow = 0
    while bfs():
        for i in range(n + 1):
            it[i] = 0
        while True:
            f = dfs_send()
            if f == 0:
                break
            flow += f

    # Tốc độ tải tối đa = luồng cực đại từ máy 1 tới máy n.
    sys.stdout.write(str(flow) + "\n")


main()
