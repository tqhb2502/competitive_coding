# Tree Matching - CSES 1130
# https://cses.fi/problemset/task/1130
# Maximum matching trên cây bằng tree DP (post-order), duyệt lặp (không đệ quy).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # BFS/DFS lặp từ gốc 1 để lấy thứ tự duyệt và parent.
    parent = [0] * (n + 1)
    order = []
    visited = bytearray(n + 1)
    stack = [1]
    visited[1] = 1
    while stack:
        u = stack.pop()
        order.append(u)
        for w in adj[u]:
            if not visited[w]:
                visited[w] = 1
                parent[w] = u
                stack.append(w)

    NEG = -(1 << 30)
    # dp0[u]: u không được ghép; dp1[u]: u được ghép với một con.
    dp0 = [0] * (n + 1)
    dp1 = [NEG] * (n + 1)

    # Xử lý theo thứ tự đảo ngược => con trước cha (post-order).
    for u in reversed(order):
        p = parent[u]
        best = 0            # tổng max(dp0[c], dp1[c])
        extra = NEG         # max (1 + dp0[c] - max(dp0[c], dp1[c]))
        for w in adj[u]:
            if w == p:
                continue
            m = dp0[w] if dp0[w] > dp1[w] else dp1[w]
            best += m
            cand = 1 + dp0[w] - m
            if cand > extra:
                extra = cand
        dp0[u] = best
        # Nếu u có con thì extra > NEG, dp1 hợp lệ; nếu là lá thì giữ NEG.
        dp1[u] = best + extra if extra > NEG else NEG

    ans = dp0[1] if dp0[1] > dp1[1] else dp1[1]
    sys.stdout.buffer.write((str(ans) + "\n").encode())


main()
