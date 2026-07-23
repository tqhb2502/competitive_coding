import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Xây dựng danh sách kề adj và mảng bậc vào indeg.
    # Ràng buộc "a trước b" tương ứng với cạnh có hướng a -> b.
    adj = [[] for _ in range(n + 1)]
    indeg = [0] * (n + 1)
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        indeg[b] += 1

    # Kahn: khởi tạo hàng đợi với mọi đỉnh không có tiên quyết (indeg = 0).
    q = deque(v for v in range(1, n + 1) if indeg[v] == 0)

    # Lấy dần từng đỉnh ra, giảm bậc vào của các đỉnh kề; đỉnh nào về 0 thì thêm vào.
    order = []
    while q:
        u = q.popleft()
        order.append(u)
        for v in adj[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)

    # Lấy đủ n đỉnh -> in một thứ tự topo hợp lệ; ngược lại còn chu trình.
    if len(order) == n:
        sys.stdout.write(' '.join(map(str, order)) + '\n')
    else:
        sys.stdout.write('IMPOSSIBLE\n')


main()
