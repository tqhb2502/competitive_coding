import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    # Dựng danh sách kề cho đồ thị VÔ HƯỚNG (mỗi cạnh thêm hai chiều).
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        adj[b].append(a)

    visited = bytearray(n + 1)
    parent = [0] * (n + 1)   # 0 nghĩa là không có parent (đỉnh đánh số 1..n)
    ptr = [0] * (n + 1)      # con trỏ duyệt danh sách kề cho DFS lặp

    result = None

    # Duyệt từng thành phần liên thông bằng DFS lặp cho tới khi tìm được chu trình.
    for s in range(1, n + 1):
        if visited[s]:
            continue
        visited[s] = 1
        parent[s] = 0
        stack = [s]
        while stack:
            u = stack[-1]
            adj_u = adj[u]
            if ptr[u] < len(adj_u):
                v = adj_u[ptr[u]]
                ptr[u] += 1
                if v == parent[u]:
                    # Bỏ qua đúng một cạnh quay lại parent (không có cạnh bội).
                    continue
                if not visited[v]:
                    # Cạnh cây: đi sâu xuống v.
                    visited[v] = 1
                    parent[v] = u
                    stack.append(v)
                else:
                    # Back edge u -> v: v là tổ tiên của u. Truy ngược theo
                    # parent từ u về v để dựng chu trình [u, ..., v, u].
                    cyc = [u]
                    x = u
                    while x != v:
                        x = parent[x]
                        cyc.append(x)
                    cyc.append(u)
                    result = cyc
                    break
            else:
                # Đã duyệt hết cạnh kề của u -> lấy u ra khỏi stack.
                stack.pop()
        if result is not None:
            break

    # Không tìm thấy chu trình nào -> in "IMPOSSIBLE".
    out = sys.stdout
    if result is None:
        out.write("IMPOSSIBLE\n")
    else:
        out.write(str(len(result)) + "\n")
        out.write(" ".join(map(str, result)) + "\n")


main()
