import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    arr = data[idx:idx + n]
    idx += n

    # Đọc truy vấn dưới dạng (b, a, k) rồi SẮP XẾP theo đầu mút phải b tăng dần.
    queries = []
    for k in range(q):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        queries.append((b, a, k))
    queries.sort()

    # Fenwick tree (BIT) 1-indexed trên các vị trí 1..n.
    tree = [0] * (n + 1)

    def update(i, delta):
        while i <= n:
            tree[i] += delta
            i += i & (-i)

    def prefix(i):
        s = 0
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        return s

    ans = [0] * q
    last = {}          # giá trị (dạng bytes) -> vị trí xuất hiện gần nhất
    pos = 1            # vị trí tiếp theo cần thêm vào BIT (1-indexed)

    for b, a, k in queries:
        # Mở rộng tiền tố tới b, chỉ giữ dấu 1 tại lần xuất hiện cuối của mỗi giá trị.
        while pos <= b:
            v = arr[pos - 1]
            prev = last.get(v)
            if prev is not None:
                update(prev, -1)   # gỡ đánh dấu cũ
            update(pos, 1)         # đánh dấu vị trí mới
            last[v] = pos
            pos += 1
        # Số giá trị phân biệt trong [a, b] = tổng đoạn BIT.
        ans[k] = prefix(b) - prefix(a - 1)

    out = b"\n".join(str(x).encode() for x in ans)
    sys.stdout.buffer.write(out + b"\n")


main()
