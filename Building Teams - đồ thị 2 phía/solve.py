from collections import deque
import sys

input = sys.stdin.readline

n, m = map(int, input().split())

adj = [[] for _ in range(n + 1)]

for _ in range(m):
    a, b = map(int, input().split())
    adj[a].append(b)
    adj[b].append(a)

team = [0] * (n + 1)

for start in range(1, n + 1):
    if team[start] != 0:
        continue

    team[start] = 1
    q = deque([start])

    while q:
        u = q.popleft()

        for v in adj[u]:
            if team[v] == 0:
                team[v] = 3 - team[u]
                q.append(v)
            elif team[v] == team[u]:
                print("IMPOSSIBLE")
                sys.exit()

print(*team[1:])
