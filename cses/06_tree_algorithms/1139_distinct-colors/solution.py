# Distinct Colors (CSES 1139)
# https://cses.fi/problemset/task/1139
# Số màu phân biệt trong subtree của mỗi đỉnh.
# Kỹ thuật: small-to-large merging (DSU on tree), duyệt cây không đệ quy.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1

    c = [0] * (n + 1)
    for i in range(1, n + 1):
        c[i] = int(data[pos]); pos += 1

    # Danh sách kề (cây vô hướng, sẽ định hướng từ gốc 1 khi BFS/DFS).
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        adj[b].append(a)

    # DFS lặp (iterative): tính parent[] và order[] (cha luôn đứng trước con).
    parent = [0] * (n + 1)
    order = []
    visited = bytearray(n + 1)
    stack = [1]
    visited[1] = 1
    while stack:
        u = stack.pop()
        order.append(u)
        for v in adj[u]:
            if not visited[v]:
                visited[v] = 1
                parent[v] = u
                stack.append(v)

    # small-to-large: xử lý order theo thứ tự đảo (con trước cha).
    sets = [None] * (n + 1)
    ans = [0] * (n + 1)

    for u in reversed(order):
        s = sets[u]
        if s is None:
            s = {c[u]}
        else:
            s.add(c[u])
        ans[u] = len(s)

        p = parent[u]
        if p:
            sp = sets[p]
            if sp is None:
                sets[p] = s
            elif len(s) > len(sp):
                # gộp set nhỏ (sp) vào set lớn (s)
                s.update(sp)
                sets[p] = s
            else:
                sp.update(s)
        sets[u] = None  # giải phóng bộ nhớ

    sys.stdout.buffer.write((' '.join(map(str, ans[1:n + 1]))).encode())


if __name__ == "__main__":
    main()
