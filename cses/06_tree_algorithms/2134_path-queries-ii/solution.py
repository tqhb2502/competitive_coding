import sys


def main():
    data = sys.stdin.buffer.read().split()
    ip = 0
    n = int(data[ip]); ip += 1
    q = int(data[ip]); ip += 1

    val = [0] * (n + 1)
    for i in range(1, n + 1):
        val[i] = int(data[ip]); ip += 1

    # Danh sách kề của cây vô hướng.
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[ip]); b = int(data[ip + 1]); ip += 2
        adj[a].append(b)
        adj[b].append(a)

    # --- DFS ITERATIVE từ gốc: tính parent, depth và thứ tự pre-order ---
    parent = [0] * (n + 1)
    depth = [0] * (n + 1)
    order = []
    visited = bytearray(n + 1)
    root = 1
    stack = [root]
    visited[root] = 1
    while stack:
        u = stack.pop()
        order.append(u)
        du = depth[u]
        for v in adj[u]:
            if not visited[v]:
                visited[v] = 1
                parent[v] = u
                depth[v] = du + 1
                stack.append(v)

    # --- Tính subtree size và chọn heavy child (duyệt ngược thứ tự pre-order) ---
    size = [1] * (n + 1)
    size[0] = 0
    heavy = [0] * (n + 1)
    for u in reversed(order):
        p = parent[u]
        if p:
            size[p] += size[u]
            if size[u] > size[heavy[p]]:
                heavy[p] = u

    # --- Decomposition: gán head (đầu chain) và pos (vị trí trên segment tree) ---
    head = [0] * (n + 1)
    posn = [0] * (n + 1)
    timer = 0
    stack = [(root, root)]
    while stack:
        u, h = stack.pop()
        while u:
            head[u] = h
            posn[u] = timer
            timer += 1
            hv = heavy[u]
            pu = parent[u]
            for v in adj[u]:
                if v != pu and v != hv:
                    stack.append((v, v))  # light child mở đầu một chain mới
            u = hv  # đi theo heavy child để các vị trí trong chain liên tiếp

    # --- Iterative max segment tree trên mảng sắp theo pos ---
    S = 1
    while S < n:
        S <<= 1
    tree = [0] * (2 * S)  # giá trị >= 1 nên 0 là phần tử trung hòa cho max
    for u in range(1, n + 1):
        tree[S + posn[u]] = val[u]
    for i in range(S - 1, 0, -1):
        a1 = tree[2 * i]; a2 = tree[2 * i + 1]
        tree[i] = a1 if a1 > a2 else a2

    out = []
    for _ in range(q):
        t = data[ip]; ip += 1
        if t == b'1':
            # Loại 1: point update đỉnh s thành x, cập nhật max dọc lên gốc.
            s = int(data[ip]); x = int(data[ip + 1]); ip += 2
            p = posn[s] + S
            tree[p] = x
            p >>= 1
            while p:
                a1 = tree[2 * p]; a2 = tree[2 * p + 1]
                tree[p] = a1 if a1 > a2 else a2
                p >>= 1
        else:
            # Loại 2: max trên đường đi (a, b) bằng cách leo qua các chain.
            a = int(data[ip]); b = int(data[ip + 1]); ip += 2
            res = 0
            ha = head[a]; hb = head[b]
            while ha != hb:
                # Luôn nhảy đỉnh có head sâu hơn để tiến về LCA.
                if depth[ha] < depth[hb]:
                    a, b = b, a
                    ha, hb = hb, ha
                # Truy vấn max trên đoạn [pos[ha], pos[a]] (inline segment tree).
                l = posn[ha] + S
                r = posn[a] + S + 1
                while l < r:
                    if l & 1:
                        if tree[l] > res:
                            res = tree[l]
                        l += 1
                    if r & 1:
                        r -= 1
                        if tree[r] > res:
                            res = tree[r]
                    l >>= 1
                    r >>= 1
                a = parent[ha]
                ha = head[a]
            # Hai đỉnh đã cùng chain: lấy max trên đoạn giữa chúng.
            if depth[a] > depth[b]:
                a, b = b, a
            l = posn[a] + S
            r = posn[b] + S + 1
            while l < r:
                if l & 1:
                    if tree[l] > res:
                        res = tree[l]
                    l += 1
                if r & 1:
                    r -= 1
                    if tree[r] > res:
                        res = tree[r]
                l >>= 1
                r >>= 1
            out.append(res)

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + ('\n' if out else '')).encode())


main()
