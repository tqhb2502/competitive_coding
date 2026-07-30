import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Danh sách kề của cây (n đỉnh, n - 1 cạnh).
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # Duyệt lặp bằng stack từ gốc 1 để lấy thứ tự duyệt và mảng parent.
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
    # dp0[u]: u KHÔNG được ghép; dp1[u]: u ĐƯỢC ghép với đúng một con.
    dp0 = [0] * (n + 1)
    dp1 = [NEG] * (n + 1)

    # Xử lý theo thứ tự đảo ngược => con tính trước cha (post-order).
    for u in reversed(order):
        p = parent[u]
        best = 0            # tổng max(dp0[c], dp1[c]) trên mọi con c
        extra = NEG         # max(1 + dp0[c] - max(dp0[c], dp1[c]))
        for w in adj[u]:
            if w == p:
                continue
            m = dp0[w] if dp0[w] > dp1[w] else dp1[w]
            best += m
            # Lợi ích khi đổi con w sang trạng thái ghép cạnh u-w.
            cand = 1 + dp0[w] - m
            if cand > extra:
                extra = cand
        dp0[u] = best
        # Nếu u có con thì extra > NEG, dp1 hợp lệ; nếu là lá thì giữ NEG.
        dp1[u] = best + extra if extra > NEG else NEG

    # Đáp án là trạng thái tốt hơn tại gốc.
    ans = dp0[1] if dp0[1] > dp1[1] else dp1[1]
    sys.stdout.buffer.write((str(ans) + "\n").encode())


main()
