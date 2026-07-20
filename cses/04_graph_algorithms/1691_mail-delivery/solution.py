# Mail Delivery - CSES 1691
# https://cses.fi/problemset/task/1691
# Tìm một chu trình Euler (Eulerian circuit) trên đồ thị vô hướng,
# bắt đầu và kết thúc tại đỉnh 1, đi qua mỗi cạnh đúng một lần.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # adj[v] chứa các cặp (đỉnh kề, edge id)
    adj = [[] for _ in range(n + 1)]
    deg = [0] * (n + 1)

    for e in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        adj[a].append((b, e))
        adj[b].append((a, e))
        deg[a] += 1
        deg[b] += 1

    out = sys.stdout

    # Điều kiện cần: mọi đỉnh phải có bậc chẵn.
    for v in range(1, n + 1):
        if deg[v] & 1:
            out.write("IMPOSSIBLE\n")
            return

    # Nếu đỉnh 1 không có cạnh nào mà tồn tại cạnh khác -> không thể bắt đầu.
    # Trường hợp này sẽ bị bắt bởi kiểm tra số cạnh sử dụng bên dưới.

    used = bytearray(m)          # đánh dấu cạnh đã đi qua
    ptr = [0] * (n + 1)          # con trỏ duyệt adjacency của mỗi đỉnh

    # Thuật toán Hierholzer (iterative để tránh tràn đệ quy).
    stack = [1]
    circuit = []
    while stack:
        v = stack[-1]
        av = adj[v]
        p = ptr[v]
        lv = len(av)
        # bỏ qua các cạnh đã dùng
        while p < lv and used[av[p][1]]:
            p += 1
        ptr[v] = p
        if p == lv:
            circuit.append(v)
            stack.pop()
        else:
            to, eid = av[p]
            used[eid] = 1
            ptr[v] = p + 1
            stack.append(to)

    # Chu trình Euler hợp lệ phải đi qua tất cả m cạnh -> có m+1 đỉnh.
    # Nếu ít hơn nghĩa là đồ thị không liên thông (có cạnh ở thành phần khác).
    if len(circuit) != m + 1:
        out.write("IMPOSSIBLE\n")
        return

    circuit.reverse()
    out.write(" ".join(map(str, circuit)))
    out.write("\n")


main()
