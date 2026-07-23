import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # DSU: parent lưu cha của mỗi đỉnh, size lưu kích thước mỗi tập.
    parent = list(range(n + 1))
    size = [1] * (n + 1)

    components = n              # Số thành phần liên thông hiện tại.
    max_size = 1 if n >= 1 else 0  # Kích thước thành phần lớn nhất.

    def find(x):
        # Tìm root của tập chứa x, nén đường đi kiểu lặp (tránh đệ quy sâu).
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
        # Chỉ gộp khi a và b thuộc hai thành phần khác nhau.
        if ra != rb:
            # union by size: gắn cây nhỏ vào cây lớn.
            if size[ra] < size[rb]:
                ra, rb = rb, ra
            parent[rb] = ra
            size[ra] += size[rb]
            components -= 1
            if size[ra] > max_size:
                max_size = size[ra]
        # Ghi lại kết quả sau mỗi con đường được xây.
        out.append(f"{components} {max_size}")

    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
