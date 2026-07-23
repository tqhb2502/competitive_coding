import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh.
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # Kích thước cây: lũy thừa của 2 nhỏ nhất mà >= n.
    size = 1
    while size < n:
        size <<= 1

    INF = 1 << 62
    tree = [INF] * (2 * size)

    # Đặt các lá: lá thứ i nằm tại tree[size + i].
    base = size
    for i in range(n):
        tree[base + i] = int(data[idx]); idx += 1

    # Xây dựng bottom-up: mỗi node cha = min của hai con.
    for p in range(size - 1, 0, -1):
        l = tree[2 * p]
        r = tree[2 * p + 1]
        tree[p] = l if l < r else r

    out = []
    out_append = out.append

    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'1':
            k = int(data[idx]); idx += 1
            u = int(data[idx]); idx += 1
            # Point update: gán lại lá tại vị trí k (1-indexed) rồi đi từ lá
            # lên gốc, cập nhật lại các node cha bằng min hai con.
            pos = size + k - 1
            tree[pos] = u
            pos >>= 1
            while pos:
                l = tree[2 * pos]
                r = tree[2 * pos + 1]
                tree[pos] = l if l < r else r
                pos >>= 1
        else:
            a = int(data[idx]); idx += 1
            b = int(data[idx]); idx += 1
            # Range-min trên đoạn đóng [a, b] (1-indexed) -> nửa mở [a-1, b).
            lo = a - 1 + size
            hi = b + size
            res = INF
            while lo < hi:
                # Biên trái lẻ: gộp node rồi tiến lên.
                if lo & 1:
                    v = tree[lo]
                    if v < res:
                        res = v
                    lo += 1
                # Biên phải lẻ: lùi lại rồi gộp node trong đoạn.
                if hi & 1:
                    hi -= 1
                    v = tree[hi]
                    if v < res:
                        res = v
                lo >>= 1
                hi >>= 1
            out_append(res)

    # Ghi kết quả một lần, dùng LF để xuống dòng.
    sys.stdout.buffer.write(b'\n'.join(str(x).encode() for x in out))
    if out:
        sys.stdout.buffer.write(b'\n')


main()
