import sys


def main():
    # Đọc toàn bộ input một lần để tránh chậm do I/O trên CPython.
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Xây adjacency list cho đồ thị có hướng và đếm bậc vào/ra mỗi đỉnh.
    adj = [[] for _ in range(n + 1)]
    outdeg = [0] * (n + 1)
    indeg = [0] * (n + 1)

    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append(b)
        outdeg[a] += 1
        indeg[b] += 1

    # Điều kiện bậc cho đường đi Euler có hướng từ đỉnh 1 tới đỉnh n:
    #   đỉnh 1 dư đúng một cạnh ra (+1), đỉnh n dư đúng một cạnh vào (-1),
    #   mọi đỉnh khác cân bằng bậc vào/ra (0). (n >= 2 nên 1 != n.)
    ok = True
    for v in range(1, n + 1):
        d = outdeg[v] - indeg[v]
        if v == 1:
            if d != 1:
                ok = False
                break
        elif v == n:
            if d != -1:
                ok = False
                break
        else:
            if d != 0:
                ok = False
                break

    if not ok:
        sys.stdout.write("IMPOSSIBLE\n")
        return

    # Hierholzer kiểu iterative xuất phát từ đỉnh 1; ptr[v] là con trỏ tới cạnh
    # chưa dùng kế tiếp của v nên mỗi cạnh chỉ được xét đúng một lần.
    ptr = [0] * (n + 1)
    stack = [1]
    path = []
    while stack:
        v = stack[-1]
        p = ptr[v]
        if p < len(adj[v]):
            # Còn cạnh chưa dùng: đi theo cạnh đó, đẩy đỉnh kề vào stack.
            ptr[v] = p + 1
            stack.append(adj[v][p])
        else:
            # Hết cạnh: pop đỉnh và thêm vào path (thứ tự ngược).
            path.append(v)
            stack.pop()

    # Phải dùng hết mọi cạnh => path có m + 1 đỉnh; đây cũng bao trọn điều kiện
    # liên thông. Nếu không đủ thì IMPOSSIBLE.
    if len(path) != m + 1:
        sys.stdout.write("IMPOSSIBLE\n")
        return

    # Đảo ngược để có thứ tự đi thực sự rồi ghi output một lần.
    path.reverse()
    sys.stdout.write(' '.join(map(str, path)))
    sys.stdout.write('\n')


main()
