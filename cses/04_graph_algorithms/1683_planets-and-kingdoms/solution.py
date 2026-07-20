# Planets and Kingdoms - CSES 1683
# https://cses.fi/problemset/task/1683
#
# Tìm các Strongly Connected Component (SCC) bằng thuật toán Kosaraju.
# Mỗi SCC là một "kingdom"; hai planet cùng SCC được gán cùng nhãn.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1

    adj = [[] for _ in range(n + 1)]   # đồ thị gốc
    radj = [[] for _ in range(n + 1)]  # đồ thị đảo chiều
    for _ in range(m):
        a = int(data[pos]); b = int(data[pos + 1]); pos += 2
        adj[a].append(b)
        radj[b].append(a)

    # --- Bước 1: DFS lặp trên đồ thị gốc, ghi finish order (post-order) ---
    visited = bytearray(n + 1)
    ptr = [0] * (n + 1)   # con trỏ chỉ số cạnh đang xét của mỗi đỉnh
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
                order.append(node)
                stack.pop()

    # --- Bước 2: DFS trên đồ thị đảo chiều theo thứ tự ngược của order ---
    comp = [0] * (n + 1)
    label = 0
    for s in reversed(order):
        if comp[s]:
            continue
        label += 1
        comp[s] = label
        stack = [s]
        while stack:
            node = stack.pop()
            for nxt in radj[node]:
                if not comp[nxt]:
                    comp[nxt] = label
                    stack.append(nxt)

    out = [str(label), ' '.join(map(str, comp[1:]))]
    sys.stdout.write('\n'.join(out) + '\n')


main()
