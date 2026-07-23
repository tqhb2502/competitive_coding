import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # DSU: parent lưu gốc đại diện, size dùng cho union by size.
    parent = list(range(n + 1))
    size = [1] * (n + 1)

    # Tìm gốc đại diện, kèm path compression (lặp, tránh đệ quy sâu).
    def find(x):
        root = x
        while parent[root] != root:
            root = parent[root]
        while parent[x] != root:
            parent[x], x = root, parent[x]
        return root

    # Gộp các đỉnh nối bởi những con đường đã có sẵn.
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        ra, rb = find(a), find(b)
        if ra != rb:
            if size[ra] < size[rb]:
                ra, rb = rb, ra
            parent[rb] = ra
            size[ra] += size[rb]

    # Lấy một đỉnh đại diện cho mỗi thành phần liên thông.
    reps = []
    for v in range(1, n + 1):
        if find(v) == v:
            reps.append(v)

    # Cần (số thành phần - 1) con đường; nối đại diện đầu tiên với các đại diện còn lại.
    out = []
    out.append(str(len(reps) - 1))
    base = reps[0]
    for i in range(1, len(reps)):
        out.append(str(base) + " " + str(reps[i]))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
