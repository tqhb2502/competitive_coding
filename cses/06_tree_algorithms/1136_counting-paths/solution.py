import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    # Đọc cây: danh sách kề vô hướng gồm n - 1 cạnh
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(next(it)); b = int(next(it))
        adj[a].append(b)
        adj[b].append(a)

    # BFS gốc tại 1: dựng parent, children và thứ tự duyệt (order)
    parent = [0] * (n + 1)
    children = [[] for _ in range(n + 1)]
    order = []
    visited = bytearray(n + 1)
    dq = deque([1])
    visited[1] = 1
    while dq:
        u = dq.popleft()
        order.append(u)
        for v in adj[u]:
            if not visited[v]:
                visited[v] = 1
                parent[v] = u
                children[u].append(v)
                dq.append(v)

    # Đọc các đường đi (truy vấn); qlist[u] = danh sách (đầu kia, chỉ số truy vấn)
    pathA = [0] * m
    pathB = [0] * m
    qlist = [[] for _ in range(n + 1)]
    for qi in range(m):
        a = int(next(it)); b = int(next(it))
        pathA[qi] = a
        pathB[qi] = b
        qlist[a].append((b, qi))
        qlist[b].append((a, qi))

    # ---- Tarjan offline LCA (DSU union theo size + nén đường đi) ----
    dsu = list(range(n + 1))
    size = [1] * (n + 1)
    ancestor = [0] * (n + 1)
    black = bytearray(n + 1)
    lca = [0] * m

    def find(x):
        root = x
        d = dsu
        while d[root] != root:
            root = d[root]
        while d[x] != root:
            nxt = d[x]
            d[x] = root
            x = nxt
        return root

    # DFS lặp: mỗi frame là [node, parent, chỉ số con kế tiếp]
    stack = [[1, 0, 0]]
    ancestor[1] = 1  # make_set(1)
    while stack:
        frame = stack[-1]
        u = frame[0]
        ci = frame[2]
        ch = children[u]
        if ci < len(ch):
            # Còn con chưa duyệt: make_set(v) rồi đi xuống con đó
            v = ch[ci]
            frame[2] = ci + 1
            ancestor[v] = v  # make_set(v)
            stack.append([v, u, 0])
        else:
            # Hoàn tất u: trả lời mọi truy vấn có đầu kia đã hoàn tất (black)
            black[u] = 1
            for w, qi in qlist[u]:
                if black[w]:
                    lca[qi] = ancestor[find(w)]
            stack.pop()
            pu = frame[1]
            if pu:
                # union(pu, u) rồi gán lại ancestor của tập cho cha pu
                ra = find(pu)
                rb = find(u)
                if ra != rb:
                    if size[ra] < size[rb]:
                        ra, rb = rb, ra
                    dsu[rb] = ra
                    size[ra] += size[rb]
                ancestor[find(pu)] = pu

    # ---- Difference on tree: đánh dấu hiệu cho từng đường đi qua LCA ----
    diff = [0] * (n + 1)
    for qi in range(m):
        a = pathA[qi]
        b = pathB[qi]
        l = lca[qi]
        diff[a] += 1
        diff[b] += 1
        diff[l] -= 1
        pl = parent[l]
        if pl:
            diff[pl] -= 1

    # ---- Cộng tổng subtree bằng cách duyệt ngược thứ tự BFS (con dồn lên cha) ----
    val = diff  # tái sử dụng mảng diff
    for i in range(len(order) - 1, -1, -1):
        u = order[i]
        p = parent[u]
        if p:
            val[p] += val[u]

    sys.stdout.buffer.write((' '.join(map(str, val[1:n + 1])) + '\n').encode())


main()
