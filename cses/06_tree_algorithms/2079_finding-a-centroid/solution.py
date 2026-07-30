import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Trường hợp đặc biệt: cây một đỉnh, centroid là đỉnh 1.
    if n == 1:
        sys.stdout.buffer.write(b"1\n")
        return

    # Đọc n-1 cạnh vô hướng và lưu cây bằng adjacency list.
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # Iterative DFS từ gốc 1 (không đệ quy) để tính parent và thứ tự preorder.
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

    # Tính size[u] = số đỉnh trong subtree gốc u bằng cách duyệt ngược preorder.
    size = [1] * (n + 1)
    for u in reversed(order):
        p = parent[u]
        if p != 0:
            size[p] += size[u]

    # Centroid là đỉnh đầu tiên mà mọi thành phần khi xóa nó đều <= floor(n/2):
    # phần "phía trên" (n - size[u]) và từng subtree con (size[v]).
    half = n // 2
    centroid = 1
    for u in range(1, n + 1):
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
