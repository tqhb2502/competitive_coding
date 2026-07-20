# Road Construction - https://cses.fi/problemset/task/1676
# DSU (Union-Find) with path compression + union by size.
# Sau mỗi con đường, in số thành phần liên thông và kích thước thành phần lớn nhất.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    parent = list(range(n + 1))
    size = [1] * (n + 1)

    components = n          # số thành phần liên thông hiện tại
    max_size = 1 if n >= 1 else 0

    def find(x):
        # path compression lặp (iterative) để tránh đệ quy sâu
        root = x
        while parent[root] != root:
            root = parent[root]
        while parent[x] != root:
            parent[x], x = root, parent[x]
        return root

    out = []
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        ra = find(a)
        rb = find(b)
        if ra != rb:
            # union by size: gắn cây nhỏ vào cây lớn
            if size[ra] < size[rb]:
                ra, rb = rb, ra
            parent[rb] = ra
            size[ra] += size[rb]
            components -= 1
            if size[ra] > max_size:
                max_size = size[ra]
        out.append(f"{components} {max_size}")

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
