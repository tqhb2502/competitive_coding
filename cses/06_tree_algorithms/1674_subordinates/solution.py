import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    pos = 0
    n = int(data[pos]); pos += 1

    # Đọc mảng parent và dựng danh sách con children[] cho từng node.
    parent = [0] * (n + 1)          # parent[1] = 0 (gốc không có cha)
    children = [[] for _ in range(n + 1)]
    for v in range(2, n + 1):
        p = int(data[pos]); pos += 1
        parent[v] = p
        children[p].append(v)

    # Iterative DFS từ gốc 1 (explicit stack, tránh đệ quy sâu):
    # thu được thứ tự 'order' mà cha luôn đứng trước con.
    order = []
    stack = [1]
    while stack:
        u = stack.pop()
        order.append(u)
        for c in children[u]:
            stack.append(c)

    # Duyệt ngược (con trước cha): DP subtree, cộng dồn count[u] + 1 lên cha
    # để có số cấp dưới của mỗi node.
    count = [0] * (n + 1)
    for u in reversed(order):
        p = parent[u]
        if p:  # u không phải gốc
            count[p] += count[u] + 1

    # In số cấp dưới của các nhân viên 1..n, cách nhau bởi dấu cách.
    sys.stdout.buffer.write(
        (' '.join(map(str, count[1:n + 1])) + '\n').encode()
    )


main()
