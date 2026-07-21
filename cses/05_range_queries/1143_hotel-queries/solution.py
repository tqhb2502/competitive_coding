# Hotel Queries - CSES 1143
# https://cses.fi/problemset/task/1143
#
# Max segment tree trên mảng số phòng trống. Với mỗi nhóm khách ta đi xuống
# (descent) tìm khách sạn trái nhất có số phòng trống >= r, rồi trừ r vào lá
# đó và cập nhật lại các tổ tiên. Mỗi truy vấn O(log n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])

    # size = lũy thừa 2 nhỏ nhất >= n
    size = 1
    while size < n:
        size <<= 1

    tree = [0] * (2 * size)

    # nạp giá trị phòng trống vào các lá
    base = 2
    for i in range(n):
        tree[size + i] = int(data[base + i])

    # dựng tree từ dưới lên
    for i in range(size - 1, 0, -1):
        l = tree[2 * i]
        rr = tree[2 * i + 1]
        tree[i] = l if l >= rr else rr

    out = []
    idx = base + n
    tree_local = tree  # bind cục bộ cho nhanh

    for j in range(m):
        r = int(data[idx + j])
        if tree_local[1] < r:
            out.append(b'0')
            continue
        # descent tìm lá trái nhất có giá trị >= r
        pos = 1
        while pos < size:
            left = 2 * pos
            if tree_local[left] >= r:
                pos = left
            else:
                pos = left + 1
        # pos là chỉ số lá; khách sạn (1-indexed)
        out.append(str(pos - size + 1).encode())
        # point update: trừ r rồi cập nhật tổ tiên
        tree_local[pos] -= r
        pos >>= 1
        while pos >= 1:
            a = tree_local[2 * pos]
            b = tree_local[2 * pos + 1]
            tree_local[pos] = a if a >= b else b
            pos >>= 1

    sys.stdout.buffer.write(b' '.join(out) + b'\n')


main()
