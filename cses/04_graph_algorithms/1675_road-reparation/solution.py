import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    # Đọc số thành phố n và số con đường m.
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Lưu cạnh dưới dạng (c, a, b) để sort tự động theo chi phí tăng dần (Kruskal).
    edges = []
    for _ in range(m):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        c = int(data[idx]); idx += 1
        edges.append((c, a, b))

    edges.sort()

    # DSU với path compression và union by size (mảng, đánh chỉ số từ 1).
    parent = list(range(n + 1))
    size = [1] * (n + 1)

    def find(x):
        # Tìm gốc rồi nén đường về gốc, thực hiện lặp để tránh đệ quy.
        root = x
        while parent[root] != root:
            root = parent[root]
        while parent[x] != root:
            parent[x], x = root, parent[x]
        return root

    # Duyệt cạnh từ rẻ nhất, gộp hai thành phần khác nhau để dựng MST.
    total = 0
    used = 0
    for c, a, b in edges:
        ra = find(a)
        rb = find(b)
        if ra != rb:
            # Gộp theo union by size.
            if size[ra] < size[rb]:
                ra, rb = rb, ra
            parent[rb] = ra
            size[ra] += size[rb]
            total += c
            used += 1
            # Đã đủ n-1 cạnh: cây khung hoàn chỉnh, dừng sớm.
            if used == n - 1:
                break

    # Chọn đủ n-1 cạnh nghĩa là đồ thị liên thông; ngược lại thì không thể nối.
    if used == n - 1:
        sys.stdout.write(str(total) + "\n")
    else:
        sys.stdout.write("IMPOSSIBLE\n")


if __name__ == "__main__":
    main()
