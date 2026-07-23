import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Giá trị gốc của danh sách (đánh số 1..n).
    vals = [0] * (n + 1)
    # Fenwick tree (BIT) lưu prefix sum của mảng đánh dấu "còn sống".
    tree = [0] * (n + 1)

    # Build BIT trong O(n): mỗi vị trí bắt đầu với giá trị 1 (phần tử còn sống).
    for i in range(1, n + 1):
        vals[i] = int(data[idx]); idx += 1
        tree[i] += 1
        j = i + (i & (-i))
        if j <= n:
            tree[j] += tree[i]

    # LOG = số mũ cao nhất sao cho 1 << LOG <= n (dùng cho binary lifting).
    LOG = n.bit_length() - 1

    out = []
    for _ in range(n):
        # k = vị trí trong danh sách hiện tại.
        k = int(data[idx]); idx += 1

        # find k-th: tìm phần tử còn sống thứ k bằng binary lifting trên Fenwick.
        pos = 0
        r = LOG
        while r >= 0:
            nxt = pos + (1 << r)
            if nxt <= n and tree[nxt] < k:
                pos = nxt
                k -= tree[pos]
            r -= 1
        pos += 1  # vị trí nhỏ nhất có prefix sum bằng k -> phần tử cần xóa

        out.append(vals[pos])

        # Xóa phần tử tại pos: point update -1 dọc theo BIT.
        i = pos
        while i <= n:
            tree[i] -= 1
            i += i & (-i)

    sys.stdout.buffer.write(b" ".join(str(x).encode() for x in out))
    sys.stdout.buffer.write(b"\n")


main()
