import sys


def main():
    # Đọc toàn bộ input một lần rồi chuyển sang int (một lần convert ở tầng C).
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    ints = list(map(int, data))
    n = ints[0]
    k = ints[1]

    # Dựng danh sách kề (cây vô hướng).
    adj = [[] for _ in range(n + 1)]
    pos = 2
    for _ in range(n - 1):
        a = ints[pos]; b = ints[pos + 1]; pos += 2
        adj[a].append(b)
        adj[b].append(a)

    if n == 1:
        sys.stdout.buffer.write(b"0\n")
        return

    # DFS ITERATIVE từ root 1 (dùng stack tường minh, tránh RecursionError):
    # tính parent[] và danh sách preorder order[].
    parent = [0] * (n + 1)
    order = []
    order_append = order.append
    visited = bytearray(n + 1)
    st = [1]
    visited[1] = 1
    while st:
        u = st.pop()
        order_append(u)
        for v in adj[u]:
            if not visited[v]:
                visited[v] = 1
                parent[v] = u
                st.append(v)

    # height[v] = số cạnh của đường đi xuống dài nhất từ v.
    # heavy[v]  = con đạt được height đó (0 nếu v là lá).
    # Duyệt đảo preorder nên con luôn được xử lý trước cha.
    height = [0] * (n + 1)
    heavy = [0] * (n + 1)
    for u in reversed(order):
        p = parent[u]
        if p:
            h = height[u] + 1
            if h > height[p]:
                height[p] = h
                heavy[p] = u

    # Dựng danh sách light child (mọi con trừ heavy child) và memory pool.
    # Mỗi heavy chain (đỉnh đầu chuỗi .. lá sâu nhất) chiếm một đoạn liên tiếp
    # kích thước height[đầu]+1. Đỉnh v tại offset j có Dptr[v] = base + j, nên
    # cnt[v][i] = D[Dptr[v] + i] và Dptr[v] = Dptr[heavy[v]] - 1 (thừa kế O(1)).
    light = [None] * (n + 1)
    Dptr = [0] * (n + 1)
    pool = 0
    for v in order:
        p = parent[v]
        if v == 1 or heavy[p] != v:    # v là đỉnh đầu một heavy chain (light child)
            if v != 1:
                lp = light[p]
                if lp is None:
                    light[p] = [v]
                else:
                    lp.append(v)
            base = pool
            pool += height[v] + 1
            cur = v
            off = base
            while cur:
                Dptr[cur] = off
                off += 1
                cur = heavy[cur]
    D = [0] * (pool + 1)

    ans = 0
    base_idx = k - 1
    # Duyệt bottom-up (đảo preorder): mọi con của u đã xong trước u, và mảng cnt
    # của heavy child đã sẵn nằm chung vùng nhớ với u.
    for u in reversed(order):
        du = Dptr[u]
        hu = height[u]
        D[du] = 1                      # cnt[u][0] = u; đã thừa kế cnt của heavy child
        lu = light[u]
        if lu is None:
            if k <= hu:
                ans += D[du + k]
            continue
        for c in lu:                   # chỉ light child
            dc = Dptr[c]
            ch = height[c]
            # Pairing: đỉnh cách c là h thì cách u là h+1; ghép với một đỉnh đã
            # gộp trước cách u đúng idx = k-(h+1). Điều kiện idx >= 1 loại bỏ
            # chính u và giữ hai đầu mút đều nằm thực sự dưới u.
            for h in range(ch + 1):
                idx = base_idx - h
                if idx < 1:
                    break              # idx chỉ giảm khi h tăng
                if idx <= hu:
                    ans += D[dc + h] * D[du + idx]
            # Merge cnt[c] vào cnt[u] (dịch lên một khoảng cách).
            dd = du + 1
            for h in range(ch + 1):
                D[dd + h] += D[dc + h]
        # Đường đi thẳng đứng nhận u làm đầu mút, dài đúng k.
        if k <= hu:
            ans += D[du + k]

    sys.stdout.buffer.write((str(ans) + "\n").encode())


main()
