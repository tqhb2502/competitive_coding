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

    # Danh sách kề (cây vô hướng, sẽ được định hướng từ gốc 1 khi DFS).
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        adj[b].append(a)

    # DFS lặp (không đệ quy): tính parent[] và order[] (cha luôn đứng trước con).
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

    # small-to-large: xử lý order theo thứ tự đảo nên con luôn được duyệt trước cha.
    sets = [None] * (n + 1)
    ans = [0] * (n + 1)

    for u in reversed(order):
        # Thêm màu của u vào set (đã gộp sẵn màu của mọi subtree con).
        s = sets[u]
        if s is None:
            s = {c[u]}
        else:
            s.add(c[u])
        # Kích thước set = số màu phân biệt trong subtree của u.
        ans[u] = len(s)

        # Gộp set của u vào set của cha theo quy tắc small-to-large (nhỏ vào lớn).
        p = parent[u]
        if p:
            sp = sets[p]
            if sp is None:
                sets[p] = s
            elif len(s) > len(sp):
                # set của u lớn hơn: gộp set cha (sp) vào s rồi gán lại cho cha
                s.update(sp)
                sets[p] = s
            else:
                sp.update(s)
        sets[u] = None  # giải phóng bộ nhớ set của u sau khi đã gộp

    sys.stdout.buffer.write((' '.join(map(str, ans[1:n + 1]))).encode())


if __name__ == "__main__":
    main()
