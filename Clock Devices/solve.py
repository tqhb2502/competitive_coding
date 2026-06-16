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

    

if __name__ == "__main__":
    solve()