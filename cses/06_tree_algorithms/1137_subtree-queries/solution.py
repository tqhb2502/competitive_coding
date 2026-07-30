import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    val = [0] * (n + 1)
    for i in range(1, n + 1):
        val[i] = int(data[idx]); idx += 1

    # Danh sách kề của cây.
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        adj[b].append(a)

    # DFS lặp từ gốc 1 để tính Euler tour: gán tin khi "vào" node, gán tout khi
    # "ra" (đã duyệt hết con). Subtree của node u ứng với đoạn [tin[u], tout[u]].
    tin = [0] * (n + 1)
    tout = [0] * (n + 1)
    parent = [0] * (n + 1)
    child_ptr = [0] * (n + 1)  # con trỏ tới con tiếp theo cần duyệt của mỗi node
    visited = [False] * (n + 1)

    order_stack = [1]
    visited[1] = True
    timer = 1
    tin[1] = timer
    while order_stack:
        u = order_stack[-1]
        if child_ptr[u] < len(adj[u]):
            v = adj[u][child_ptr[u]]
            child_ptr[u] += 1
            if not visited[v]:
                visited[v] = True
                parent[v] = u
                timer += 1
                tin[v] = timer  # gán tin cho con khi vừa đi vào
                order_stack.append(v)
        else:
            tout[u] = timer  # gán tout khi mọi con đã được thăm
            order_stack.pop()

    # BIT (Fenwick tree) 1-indexed trên miền [1, n] theo tin.
    bit = [0] * (n + 1)

    def update(i, delta):
        while i <= n:
            bit[i] += delta
            i += i & (-i)

    def prefix(i):
        s = 0
        while i > 0:
            s += bit[i]
            i -= i & (-i)
        return s

    # Khởi tạo BIT: đặt giá trị của mỗi node tại vị trí tin.
    for i in range(1, n + 1):
        update(tin[i], val[i])

    out = []
    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'1':
            # Đổi giá trị node s: cập nhật điểm với delta = giá trị mới - giá trị cũ.
            s = int(data[idx]); idx += 1
            x = int(data[idx]); idx += 1
            update(tin[s], x - val[s])
            val[s] = x
        else:
            # Tổng subtree của s = tổng đoạn [tin[s], tout[s]].
            s = int(data[idx]); idx += 1
            out.append(prefix(tout[s]) - prefix(tin[s] - 1))

    sys.stdout.buffer.write(("\n".join(map(str, out)) + ("\n" if out else "")).encode())


main()
