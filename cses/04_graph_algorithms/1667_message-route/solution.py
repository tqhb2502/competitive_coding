import sys
from collections import deque


def main():
    # Đọc toàn bộ input một lần cho nhanh
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Danh sách kề của đồ thị vô hướng, không trọng số
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        adj[b].append(a)

    # parent[v] = đỉnh liền trước v trên đường đi ngắn nhất; 0 nghĩa là chưa thăm
    parent = [0] * (n + 1)
    parent[1] = 1  # đánh dấu đỉnh nguồn đã thăm (trỏ chính nó)

    # BFS từ đỉnh 1: lần đầu chạm tới một đỉnh chính là qua đường đi ngắn nhất
    q = deque([1])
    while q:
        u = q.popleft()
        if u == n:
            break  # đã tới đích, dừng sớm
        for v in adj[u]:
            if parent[v] == 0:
                parent[v] = u
                q.append(v)

    # Nếu đỉnh n không được thăm thì không tồn tại đường đi
    if parent[n] == 0:
        sys.stdout.write("IMPOSSIBLE\n")
        return

    # Truy vết ngược từ n về 1 qua mảng parent, rồi đảo lại để in theo thứ tự 1 -> n
    path = []
    cur = n
    while cur != 1:
        path.append(cur)
        cur = parent[cur]
    path.append(1)
    path.reverse()

    # In số máy tính trên đường đi và trình tự các máy
    out = [str(len(path)), " ".join(map(str, path))]
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
