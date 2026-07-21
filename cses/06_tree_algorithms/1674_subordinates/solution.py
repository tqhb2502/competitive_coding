# Subordinates - CSES 1674
# https://cses.fi/problemset/task/1674
# Đếm số cấp dưới (subtree_size - 1) cho mỗi node bằng iterative DFS + DP subtree.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1

    parent = [0] * (n + 1)          # parent[1] = 0 (gốc không có cha)
    children = [[] for _ in range(n + 1)]
    for v in range(2, n + 1):
        p = int(data[pos]); pos += 1
        parent[v] = p
        children[p].append(v)

    # Iterative DFS từ gốc 1 -> thứ tự 'order': cha luôn đứng trước con.
    order = []
    stack = [1]
    while stack:
        u = stack.pop()
        order.append(u)
        for c in children[u]:
            stack.append(c)

    # Xử lý ngược: con trước cha => cộng dồn số cấp dưới lên cha.
    count = [0] * (n + 1)
    for u in reversed(order):
        p = parent[u]
        if p:  # u không phải gốc
            count[p] += count[u] + 1

    sys.stdout.buffer.write(
        (' '.join(map(str, count[1:n + 1])) + '\n').encode()
    )


main()
