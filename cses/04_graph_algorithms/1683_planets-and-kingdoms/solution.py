import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    # adj: đồ thị gốc; radj: đồ thị đảo chiều mọi cạnh (dùng cho Kosaraju).
    adj = [[] for _ in range(n + 1)]
    radj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        radj[b].append(a)

    # --- Bước 1: DFS lặp trên đồ thị gốc, ghi thứ tự kết thúc (post-order) vào order ---
    # ptr[node] là con trỏ chỉ số cạnh kế tiếp cần xét của mỗi đỉnh, tránh tràn stack.
    visited = bytearray(n + 1)
    ptr = [0] * (n + 1)
    order = []
    for s in range(1, n + 1):
        if visited[s]:
            continue
        visited[s] = 1
        stack = [s]
        while stack:
            node = stack[-1]
            neigh = adj[node]
            i = ptr[node]
            if i < len(neigh):
                ptr[node] = i + 1
                nxt = neigh[i]
                if not visited[nxt]:
                    visited[nxt] = 1
                    stack.append(nxt)
            else:
                # Đỉnh đã duyệt hết cạnh: chốt finish order rồi lấy ra khỏi stack.
                order.append(node)
                stack.pop()

    # --- Bước 2: DFS trên đồ thị đảo chiều theo thứ tự NGƯỢC của order ---
    # Mỗi lần xuất phát từ đỉnh chưa gán nhãn sẽ phủ trọn đúng một SCC (kingdom).
    comp = [0] * (n + 1)
    label = 0
    for s in reversed(order):
        if comp[s]:
            continue
        label += 1  # Mở một vương quốc (SCC) mới.
        comp[s] = label
        stack = [s]
        while stack:
            node = stack.pop()
            for nxt in radj[node]:
                if not comp[nxt]:
                    comp[nxt] = label
                    stack.append(nxt)

    # In số vương quốc k rồi in nhãn SCC của từng hành tinh 1..n.
    out = [str(label), ' '.join(map(str, comp[1:]))]
    sys.stdout.write('\n'.join(out) + '\n')


main()
