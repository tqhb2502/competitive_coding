import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Danh sách kề và indegree phục vụ thuật toán Kahn (topo sort).
    adj = [[] for _ in range(n + 1)]
    indeg = [0] * (n + 1)
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        indeg[b] += 1

    # dist[v] = số thành phố nhiều nhất trên đường đi từ 1 tới v (-1 = chưa tới được).
    # parent[v] = đỉnh liền trước trên đường đi tối ưu, dùng để truy vết.
    dist = [-1] * (n + 1)
    parent = [0] * (n + 1)
    dist[1] = 1

    # Khởi tạo hàng đợi bằng các đỉnh có indegree 0, rồi duyệt theo thứ tự topo.
    dq = deque(v for v in range(1, n + 1) if indeg[v] == 0)
    while dq:
        u = dq.popleft()
        du = dist[u]
        for v in adj[u]:
            # Relax cạnh u -> v khi u tới được từ 1 và cho đường đi dài hơn.
            if du != -1 and du + 1 > dist[v]:
                dist[v] = du + 1
                parent[v] = u
            # Giảm indegree cho Kahn, về 0 thì đẩy v vào hàng đợi.
            indeg[v] -= 1
            if indeg[v] == 0:
                dq.append(v)

    out = sys.stdout
    # Không tới được n từ 1 nên không tồn tại đường đi.
    if dist[n] == -1:
        out.write("IMPOSSIBLE\n")
        return

    # Truy vết đường đi: đi ngược theo parent từ n về 1 rồi đảo lại.
    path = []
    cur = n
    while cur != 0:
        path.append(cur)
        if cur == 1:
            break
        cur = parent[cur]
    path.reverse()

    out.write(str(dist[n]))
    out.write("\n")
    out.write(" ".join(map(str, path)))
    out.write("\n")


if __name__ == "__main__":
    main()
