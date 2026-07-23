import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Trường hợp cây một đỉnh: khoảng cách bằng 0.
    if n == 1:
        sys.stdout.buffer.write(b"0\n")
        return

    # Cây vô hướng biểu diễn bằng adjacency list.
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # Duyệt iterative BFS từ gốc 1 để lấy parent, depth và BFS order (tránh đệ quy).
    parent = [0] * (n + 1)
    depth = [0] * (n + 1)
    order = [0] * n
    visited = bytearray(n + 1)

    root = 1
    visited[root] = 1
    order[0] = root
    head = 0
    tail = 1
    while head < tail:
        u = order[head]
        head += 1
        du = depth[u]
        for w in adj[u]:
            if not visited[w]:
                visited[w] = 1
                parent[w] = u
                depth[w] = du + 1
                order[tail] = w
                tail += 1

    # size[v] = số đỉnh trong subtree của v; xử lý ngược order (con trước cha).
    size = [1] * (n + 1)
    for i in range(n - 1, 0, -1):
        v = order[i]
        size[parent[v]] += size[v]

    # ans[root] = tổng khoảng cách từ gốc tới mọi đỉnh = tổng depth.
    ans = [0] * (n + 1)
    s = 0
    for i in range(n):
        s += depth[order[i]]
    ans[root] = s

    # Rerooting: chuyển gốc cha -> con theo BFS order, ans[c] = ans[p] + n - 2*size[c].
    for i in range(1, n):
        v = order[i]
        p = parent[v]
        ans[v] = ans[p] + n - 2 * size[v]

    out = " ".join(str(ans[i]) for i in range(1, n + 1))
    sys.stdout.buffer.write(out.encode() + b"\n")


main()
