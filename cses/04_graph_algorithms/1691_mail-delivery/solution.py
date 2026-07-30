import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Đồ thị vô hướng: adj[v] chứa các cặp (đỉnh kề, edge id).
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

    # Điều kiện cần của chu trình Euler: mọi đỉnh phải có bậc chẵn.
    for v in range(1, n + 1):
        if deg[v] & 1:
            out.write("IMPOSSIBLE\n")
            return

    used = bytearray(m)          # đánh dấu cạnh đã đi qua
    ptr = [0] * (n + 1)          # con trỏ duyệt adjacency của mỗi đỉnh

    # Thuật toán Hierholzer (iterative để tránh tràn đệ quy): đi theo cạnh chưa dùng
    # cho tới khi kẹt, rồi lùi lại và ghi đỉnh vào circuit.
    stack = [1]
    circuit = []
    while stack:
        v = stack[-1]
        av = adj[v]
        p = ptr[v]
        lv = len(av)
        # Bỏ qua các cạnh đã dùng nhờ con trỏ ptr (mỗi cạnh chỉ bị bỏ qua O(1) lần).
        while p < lv and used[av[p][1]]:
            p += 1
        ptr[v] = p
        if p == lv:
            # Không còn cạnh: chốt đỉnh này vào circuit và lùi lại.
            circuit.append(v)
            stack.pop()
        else:
            # Đi tiếp qua một cạnh chưa dùng, đánh dấu cạnh và tiến sang đỉnh kề.
            to, eid = av[p]
            used[eid] = 1
            ptr[v] = p + 1
            stack.append(to)

    # Chu trình hợp lệ phải đi qua tất cả m cạnh -> có m + 1 đỉnh. Nếu ít hơn nghĩa là
    # đồ thị không liên thông (còn cạnh ở thành phần khác) -> in "IMPOSSIBLE".
    if len(circuit) != m + 1:
        out.write("IMPOSSIBLE\n")
        return

    # circuit đang theo thứ tự ngược, đảo lại để có lộ trình đi đúng chiều.
    circuit.reverse()
    out.write(" ".join(map(str, circuit)))
    out.write("\n")


main()
