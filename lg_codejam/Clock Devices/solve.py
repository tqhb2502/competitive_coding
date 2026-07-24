import sys

def solve():
    input = sys.stdin.readline

    N, C = map(int, input().split())
    D = list(map(int, input().split()))
    W = list(map(int, input().split()))

    adj = [[] for _ in range(N)]

    for _ in range(N - 1):
        u, v = map(int, input().split())
        u -= 1
        v -= 1
        adj[u].append(v)
        adj[v].append(u)

    need = [(12 - di) for di in D]

    # DFS
    parent = [-2] * N
    parent[0] = -1
    order = []
    stack = [0]
    while stack:
        u = stack.pop()
        order.append(u)

        for v in adj[u]:
            if v != parent[u]:
                parent[v] = u
                stack.append(v)

    dp = [None] * N

    for u in reversed(order):
        # Nếu đỉnh u chọn nhãn need[u] thì nó sẽ đc W[u]
        # Chọn nhãn khác thì k đc gì
        cur = [0] * 12
        cur[need[u]] = W[u]

        for v in adj[u]:
            if parent[v] == u:
                # Cắt cạnh u-v
                best_cut = max(dp[v]) - C
                # Giữ cạnh u-v
                for label in range(12):
                    cur[label] += max(dp[v][label], best_cut)

        dp[u] = cur

    print(max(dp[0]))


if __name__ == "__main__":
    solve()
