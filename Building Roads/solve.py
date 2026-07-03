import sys
from collections import deque


def solve():
    input = sys.stdin.readline

    n, m = map(int, input().split())

    graph = [[] for _ in range(n + 1)]

    for _ in range(m):
        a, b = map(int, input().split())
        graph[a].append(b)
        graph[b].append(a)

    visited = [False] * (n + 1)
    representatives = []

    for city in range(1, n + 1):
        if not visited[city]:
            representatives.append(city)

            queue = deque([city])
            visited[city] = True

            while queue:
                u = queue.popleft()

                for v in graph[u]:
                    if not visited[v]:
                        visited[v] = True
                        queue.append(v)

    k = len(representatives) - 1
    print(k)

    for i in range(k):
        print(representatives[i], representatives[i + 1])


if __name__ == "__main__":
    solve()
