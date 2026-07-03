from collections import deque
import sys

input = sys.stdin.readline

n, m = map(int, input().split())

graph = [[] for _ in range(n + 1)]

for _ in range(m):
    a, b = map(int, input().split())
    graph[a].append(b)
    graph[b].append(a)

parent = [-1] * (n + 1)
parent[1] = 0

q = deque([1])

while q:
    u = q.popleft()

    if u == n:
        break

    for v in graph[u]:
        if parent[v] == -1:
            parent[v] = u
            q.append(v)

if parent[n] == -1:
    print("IMPOSSIBLE")
else:
    path = []
    cur = n

    while cur != 0:
        path.append(cur)
        cur = parent[cur]

    path.reverse()

    print(len(path))
    print(*path)
