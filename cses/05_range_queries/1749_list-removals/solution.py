# List Removals - CSES task 1749
# https://cses.fi/problemset/task/1749
# Fenwick tree (BIT) với kỹ thuật "find k-th" bằng binary lifting:
#   - tree[i] = 1 nếu phần tử ở vị trí i còn tồn tại, 0 nếu đã bị xóa.
#   - Với mỗi truy vấn p, tìm vị trí của phần tử còn tồn tại thứ p
#     (tức là vị trí nhỏ nhất có prefix sum == p) trong O(log n),
#     rồi cập nhật vị trí đó về 0 (point update) trong O(log n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Giá trị gốc của danh sách (1-indexed).
    vals = [0] * (n + 1)
    tree = [0] * (n + 1)

    # Build BIT: mỗi vị trí bắt đầu với giá trị 1 (còn tồn tại) trong O(n).
    for i in range(1, n + 1):
        vals[i] = int(data[idx]); idx += 1
        tree[i] += 1
        j = i + (i & (-i))
        if j <= n:
            tree[j] += tree[i]

    # LOG = số mũ cao nhất sao cho 1 << LOG <= n.
    LOG = n.bit_length() - 1

    out = []
    for _ in range(n):
        k = int(data[idx]); idx += 1

        # find k-th: tìm vị trí còn tồn tại thứ k bằng binary lifting.
        pos = 0
        r = LOG
        while r >= 0:
            nxt = pos + (1 << r)
            if nxt <= n and tree[nxt] < k:
                pos = nxt
                k -= tree[pos]
            r -= 1
        pos += 1  # vị trí cần xóa

        out.append(vals[pos])

        # Xóa phần tử tại pos: point update -1.
        i = pos
        while i <= n:
            tree[i] -= 1
            i += i & (-i)

    sys.stdout.buffer.write(b" ".join(str(x).encode() for x in out))
    sys.stdout.buffer.write(b"\n")


main()
