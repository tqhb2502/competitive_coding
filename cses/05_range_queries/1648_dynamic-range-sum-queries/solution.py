import sys


def main():
    # Đọc toàn bộ input một lần để tăng tốc.
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    # cur[i] = giá trị hiện tại của vị trí i (đánh chỉ số từ 1); tree là Fenwick tree (BIT).
    cur = [0] * (n + 1)
    tree = [0] * (n + 1)

    # Dựng BIT từ các giá trị ban đầu trong O(n) bằng cách đẩy lên node cha.
    for i in range(1, n + 1):
        v = int(data[pos]); pos += 1
        cur[i] = v
        tree[i] += v
        j = i + (i & (-i))
        if j <= n:
            tree[j] += tree[i]

    out = []
    for _ in range(q):
        t = data[pos]; pos += 1
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        if t == b"1":
            # Truy vấn "1 a b" là phép GÁN: cập nhật BIT theo chênh lệch delta, đi lên theo i += i & (-i).
            delta = b - cur[a]
            cur[a] = b
            i = a
            while i <= n:
                tree[i] += delta
                i += i & (-i)
        else:
            # Truy vấn "2 a b": tổng đoạn [a, b] = prefix(b) - prefix(a-1), đi xuống theo i -= i & (-i).
            s = 0
            i = b
            while i > 0:
                s += tree[i]
                i -= i & (-i)
            i = a - 1
            while i > 0:
                s -= tree[i]
                i -= i & (-i)
            out.append(s)

    # Ghi toàn bộ output một lần.
    sys.stdout.buffer.write(b"\n".join(str(x).encode() for x in out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
