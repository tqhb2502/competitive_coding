# Round Trip - CSES 1669
# https://cses.fi/problemset/task/1669
#
# Tìm một chu trình (round trip) trong đồ thị vô hướng: xuất phát và kết thúc
# tại cùng một đỉnh, đi qua ít nhất hai đỉnh khác. Do đồ thị không có cạnh bội
# và không có self-loop nên chu trình ngắn nhất có độ dài 3 (tam giác).
#
# Ý tưởng: DFS lặp (iterative) trên đồ thị vô hướng, lưu mảng parent. Khi từ
# đỉnh u ta gặp một đỉnh v đã thăm (visited) mà v != parent[u], thì u-v là một
# back edge và v chắc chắn là tổ tiên của u trên cây DFS (đồ thị vô hướng không
# có cross edge). Truy ngược theo parent từ u về v để dựng lại chu trình.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        adj[b].append(a)

    visited = bytearray(n + 1)
    parent = [0] * (n + 1)   # 0 nghĩa là không có parent (đỉnh đánh số 1..n)
    ptr = [0] * (n + 1)      # con trỏ duyệt danh sách kề cho DFS lặp

    result = None

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
                    visited[v] = 1
                    parent[v] = u
                    stack.append(v)
                else:
                    # Back edge u -> v, v là tổ tiên của u.
                    cyc = [u]
                    x = u
                    while x != v:
                        x = parent[x]
                        cyc.append(x)
                    cyc.append(u)  # đóng chu trình: [u, ..., v, u]
                    result = cyc
                    break
            else:
                stack.pop()
        if result is not None:
            break

    out = sys.stdout
    if result is None:
        out.write("IMPOSSIBLE\n")
    else:
        out.write(str(len(result)) + "\n")
        out.write(" ".join(map(str, result)) + "\n")


main()
