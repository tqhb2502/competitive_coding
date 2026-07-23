import sys
from collections import deque


def main():
    # Đọc toàn bộ dữ liệu một lần cho nhanh (tránh nguy cơ TLE)
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    MOD = 1000000007

    # Xây danh sách kề (adjacency list) và đếm in-degree cho mỗi đỉnh
    adj = [[] for _ in range(n + 1)]
    indeg = [0] * (n + 1)

    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        indeg[b] += 1

    # dp[v]: số đường đi từ đỉnh 1 đến đỉnh v, khởi tạo dp[1] = 1
    dp = [0] * (n + 1)
    dp[1] = 1

    # Kahn's algorithm: nạp mọi đỉnh có in-degree = 0 vào hàng đợi
    dq = deque()
    for v in range(1, n + 1):
        if indeg[v] == 0:
            dq.append(v)

    # Duyệt theo thứ tự topo, cộng dồn số đường đi sang các đỉnh kế tiếp
    while dq:
        u = dq.popleft()
        du = dp[u]
        for v in adj[u]:
            dp[v] = (dp[v] + du) % MOD
            # Giảm in-degree; đỉnh về 0 nghĩa là đã sẵn sàng để xử lý
            indeg[v] -= 1
            if indeg[v] == 0:
                dq.append(v)

    # Đáp án: số đường đi từ màn 1 đến màn n
    sys.stdout.write(str(dp[n] % MOD) + "\n")


main()
