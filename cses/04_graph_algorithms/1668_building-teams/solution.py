# Building Teams - CSES 1668
# https://cses.fi/problemset/task/1668
# Tô 2 màu (kiểm tra đồ thị hai phía) bằng BFS lặp trên danh sách kề.

import sys
from collections import deque


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    ints = list(map(int, data))
    n = ints[0]
    m = ints[1]

    # Xây dựng danh sách kề cho đồ thị vô hướng.
    adj = [[] for _ in range(n + 1)]
    idx = 2
    for _ in range(m):
        a = ints[idx]
        b = ints[idx + 1]
        idx += 2
        adj[a].append(b)
        adj[b].append(a)

    # 0 = chưa tô màu; 1 và 2 là hai đội.
    color = [0] * (n + 1)
    for s in range(1, n + 1):
        if color[s]:
            continue
        # Mỗi thành phần liên thông: gốc tô màu 1, lan truyền bằng BFS.
        color[s] = 1
        q = deque([s])
        while q:
            u = q.popleft()
            other = 3 - color[u]
            for w in adj[u]:
                c = color[w]
                if c == 0:
                    color[w] = other
                    q.append(w)
                elif c != other:
                    # Cạnh nối hai đỉnh cùng màu -> có chu trình lẻ -> không chia được.
                    sys.stdout.write("IMPOSSIBLE\n")
                    return
        # BFS lặp nên không bị RecursionError dù n tới 1e5.

    sys.stdout.write(" ".join(map(str, color[1:n + 1])))
    sys.stdout.write("\n")


main()
