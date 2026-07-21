# Finding a Centroid - CSES 2079
# https://cses.fi/problemset/task/2079
#
# Tìm centroid của cây: đỉnh mà khi làm gốc, mọi subtree con có kích thước
# <= floor(n/2). Dùng iterative DFS (không đệ quy) để tính parent và size,
# rồi kiểm tra điều kiện centroid cho từng đỉnh. Độ phức tạp O(n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    if n == 1:
        sys.stdout.buffer.write(b"1\n")
        return

    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # Iterative DFS từ gốc 1 để tính parent và thứ tự preorder.
    parent = [0] * (n + 1)
    order = []
    visited = [False] * (n + 1)
    stack = [1]
    visited[1] = True
    while stack:
        u = stack.pop()
        order.append(u)
        for v in adj[u]:
            if not visited[v]:
                visited[v] = True
                parent[v] = u
                stack.append(v)

    # Tính size[u] bằng cách duyệt ngược thứ tự preorder.
    size = [1] * (n + 1)
    for u in reversed(order):
        p = parent[u]
        if p != 0:
            size[p] += size[u]

    half = n // 2
    centroid = 1
    for u in range(1, n + 1):
        # Thành phần "phía trên" u khi xóa u.
        if n - size[u] > half:
            continue
        ok = True
        for v in adj[u]:
            if parent[v] == u and size[v] > half:
                ok = False
                break
        if ok:
            centroid = u
            break

    sys.stdout.buffer.write(str(centroid).encode() + b"\n")


main()
