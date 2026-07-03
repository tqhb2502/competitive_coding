import sys

input = sys.stdin.readline


def solve():
    n, m = map(int, input().split())

    adj = [[] for _ in range(n + 1)]

    for _ in range(m):
        a, b = map(int, input().split())
        adj[a].append(b)
        adj[b].append(a)

    # color:
    # 0 = chưa thăm
    # 1 = đang nằm trong stack DFS hiện tại
    # 2 = đã xử lý xong
    color = [0] * (n + 1)
    parent = [0] * (n + 1)

    for start in range(1, n + 1):
        if color[start] != 0:
            continue

        color[start] = 1
        parent[start] = 0

        # stack lưu (đỉnh hiện tại, chỉ số cạnh tiếp theo cần duyệt)
        stack = [(start, 0)]

        while stack:
            u, idx = stack[-1]

            if idx == len(adj[u]):
                color[u] = 2
                stack.pop()
                continue

            v = adj[u][idx]
            stack[-1] = (u, idx + 1)

            if v == parent[u]:
                continue

            if color[v] == 0:
                parent[v] = u
                color[v] = 1
                stack.append((v, 0))

            elif color[v] == 1:
                # Tìm được chu trình từ v -> ... -> u -> v
                cycle = [v]
                x = u

                while x != v:
                    cycle.append(x)
                    x = parent[x]

                cycle.append(v)

                print(len(cycle))
                print(*cycle)
                return

    print("IMPOSSIBLE")


if __name__ == "__main__":
    solve()
