import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    m = int(data[1])

    # Danh sách kề: mỗi chuyến bay a -> b là một cạnh có hướng.
    adj = [[] for _ in range(n + 1)]
    pos = 2
    for _ in range(m):
        a = int(data[pos])
        b = int(data[pos + 1])
        pos += 2
        adj[a].append(b)

    # Tô màu 3 trạng thái: 0 white (chưa thăm), 1 gray (đang trên stack), 2 black (xong)
    color = [0] * (n + 1)
    parent = [0] * (n + 1)
    it = [0] * (n + 1)      # con trỏ chỉ số vào danh sách kề của mỗi đỉnh

    cycle = None

    # Chạy DFS từ mọi đỉnh chưa thăm để bao phủ cả đồ thị không liên thông.
    for start in range(1, n + 1):
        if color[start] != 0:
            continue
        color[start] = 1
        parent[start] = 0
        # DFS iterative với stack tường minh (tránh RecursionError khi n lớn).
        stack = [start]
        while stack:
            node = stack[-1]
            adj_node = adj[node]
            if it[node] < len(adj_node):
                nxt = adj_node[it[node]]
                it[node] += 1
                c = color[nxt]
                if c == 0:
                    # Đỉnh white: đi tiếp, ghi cha và push vào stack.
                    color[nxt] = 1
                    parent[nxt] = node
                    stack.append(nxt)
                elif c == 1:
                    # Back edge node -> nxt: tìm được chu trình nxt -> ... -> node -> nxt.
                    # Truy vết ngược theo parent từ node lên tới nxt rồi đảo lại.
                    cyc = [nxt]
                    cur = node
                    while cur != nxt:
                        cyc.append(cur)
                        cur = parent[cur]
                    cyc.append(nxt)
                    cyc.reverse()   # [nxt, ..., node, nxt]
                    cycle = cyc
                    break
                # c == 2 (black): bỏ qua, cạnh này không tạo chu trình mới.
            else:
                # Đã duyệt hết cạnh của node: tô black rồi pop khỏi stack.
                color[node] = 2
                stack.pop()
        if cycle is not None:
            break

    if cycle is None:
        sys.stdout.write("IMPOSSIBLE\n")
    else:
        # In số đỉnh (đỉnh đầu và cuối trùng nhau) rồi in dãy đỉnh của chu trình.
        out = str(len(cycle)) + "\n" + " ".join(map(str, cycle)) + "\n"
        sys.stdout.write(out)


main()
