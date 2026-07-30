import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    if n == 1:
        sys.stdout.buffer.write(b"0\n")
        return

    # Xây dựng adjacency dạng CSR bằng các mảng phẳng để chạy nhanh:
    # deg[v] = bậc của node v; us/vs lưu tạm các cạnh đã đọc.
    deg = [0] * (n + 1)
    us = [0] * (n - 1)
    vs = [0] * (n - 1)
    for i in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        us[i] = a
        vs[i] = b
        deg[a] += 1
        deg[b] += 1

    # start[] là mảng offset của CSR; adj[] chứa danh sách kề nối tiếp nhau.
    start = [0] * (n + 2)
    for v in range(1, n + 1):
        start[v + 1] = start[v] + deg[v]
    adj = [0] * (2 * (n - 1))
    pos = start[:]  # vị trí ghi hiện tại cho từng node
    for i in range(n - 1):
        a = us[i]; b = vs[i]
        adj[pos[a]] = b; pos[a] += 1
        adj[pos[b]] = a; pos[b] += 1

    # BFS iterative (queue là mảng + con trỏ head/tail) tránh đệ quy sâu.
    # Trả về (mảng khoảng cách từ src, node xa nhất tính từ src).
    def bfs(src):
        dist = [-1] * (n + 1)
        queue = [0] * n
        head = 0
        tail = 0
        dist[src] = 0
        queue[tail] = src; tail += 1
        far = src
        while head < tail:
            u = queue[head]; head += 1
            du = dist[u]
            for j in range(start[u], start[u + 1]):
                w = adj[j]
                if dist[w] == -1:
                    dist[w] = du + 1
                    if dist[w] > dist[far]:
                        far = w
                    queue[tail] = w; tail += 1
        return dist, far

    # Bước 1: BFS từ node bất kỳ (node 1) -> A là một đầu mút của diameter.
    _, A = bfs(1)
    # Bước 2: BFS từ A -> distA và đầu mút còn lại B của diameter.
    distA, B = bfs(A)
    # Bước 3: BFS từ B -> distB.
    distB, _ = bfs(B)

    # Đáp án của mỗi node v = max(distA[v], distB[v]).
    out = [0] * n
    for v in range(1, n + 1):
        da = distA[v]
        db = distB[v]
        out[v - 1] = da if da > db else db

    sys.stdout.buffer.write((" ".join(map(str, out)) + "\n").encode())


main()
