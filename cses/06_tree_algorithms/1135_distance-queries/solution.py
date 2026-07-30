import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # Đọc trước các cạnh thô rồi mới dựng danh sách kề dạng CSR.
    edges_a = [0] * (n - 1) if n > 1 else []
    edges_b = [0] * (n - 1) if n > 1 else []
    deg = [0] * (n + 1)
    for i in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        edges_a[i] = a
        edges_b[i] = b
        deg[a] += 1
        deg[b] += 1

    # Dựng danh sách kề CSR: start[] là offset, adj[] chứa các đỉnh kề.
    start = [0] * (n + 2)
    for v in range(1, n + 1):
        start[v + 1] = start[v] + deg[v]
    adj = [0] * (2 * (n - 1)) if n > 1 else []
    pos = start[:]  # con trỏ ghi cho từng đỉnh
    for i in range(n - 1):
        a = edges_a[i]; b = edges_b[i]
        adj[pos[a]] = b; pos[a] += 1
        adj[pos[b]] = a; pos[b] += 1

    # BFS lặp từ đỉnh 1 để tính parent[] và depth[], tránh đệ quy.
    depth = [0] * (n + 1)
    parent = [0] * (n + 1)
    visited = bytearray(n + 1)
    order = [0] * n
    order[0] = 1
    visited[1] = 1
    parent[1] = 0
    depth[1] = 0
    qh = 0
    qt = 1
    while qh < qt:
        u = order[qh]; qh += 1
        du = depth[u]
        for e in range(start[u], start[u + 1]):
            w = adj[e]
            if not visited[w]:
                visited[w] = 1
                parent[w] = u
                depth[w] = du + 1
                order[qt] = w; qt += 1

    # Bảng binary lifting: up[k][v] = tổ tiên thứ 2^k của v.
    LOG = 1
    while (1 << LOG) < n:
        LOG += 1
    if LOG < 1:
        LOG = 1

    up = [parent]  # up[0] = parent trực tiếp
    for k in range(1, LOG):
        prev = up[k - 1]
        cur = [0] * (n + 1)
        for v in range(1, n + 1):
            cur[v] = prev[prev[v]]
        up.append(cur)

    out = []
    for _ in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        da = depth[a]; db = depth[b]
        # Đưa đỉnh sâu hơn lên cùng độ sâu với đỉnh còn lại.
        if da < db:
            a, b = b, a
            da, db = db, da
        diff = da - db
        k = 0
        while diff:
            if diff & 1:
                a = up[k][a]
            diff >>= 1
            k += 1
        if a == b:
            # Đã trùng nhau: b chính là LCA.
            l = a
        else:
            # Nhảy đồng thời hai đỉnh lên bước lớn nhất mà chúng vẫn khác nhau.
            for k in range(LOG - 1, -1, -1):
                ua = up[k][a]
                ub = up[k][b]
                if ua != ub:
                    a = ua
                    b = ub
            l = parent[a]  # parent chung là LCA
        # Khoảng cách = depth[a] + depth[b] - 2 * depth[LCA].
        out.append(da + db - 2 * depth[l])

    sys.stdout.buffer.write(("\n".join(map(str, out)) + ("\n" if out else "")).encode())


if __name__ == "__main__":
    main()
