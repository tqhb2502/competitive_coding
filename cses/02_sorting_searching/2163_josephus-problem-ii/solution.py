# Josephus Problem II - CSES 2163
# https://cses.fi/problemset/task/2163
#
# Mô phỏng: các đứa trẻ còn lại luôn giữ đúng thứ tự tăng dần (đúng thứ tự vòng tròn).
# Dùng một order-statistics segment tree để vừa tìm phần tử thứ i còn tồn tại vừa xóa nó,
# tất cả trong một lần đi xuống O(log n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])

    # segment tree đếm số phần tử còn tồn tại trên [1..n]
    size = 1
    while size < n:
        size <<= 1
    tree = [0] * (2 * size)
    for i in range(n):
        tree[size + i] = 1
    for i in range(size - 1, 0, -1):
        tree[i] = tree[2 * i] + tree[2 * i + 1]

    res = [0] * n
    pos = 0
    remaining = n
    for out in range(n):
        idx = (pos + k) % remaining      # chỉ số 0-based của đứa trẻ bị loại
        target = idx + 1                 # tìm phần tử thứ target (1-based) còn tồn tại
        node = 1
        tree[1] -= 1                     # xóa luôn trên đường đi xuống
        while node < size:
            left = node + node
            lc = tree[left]
            if lc >= target:
                node = left
            else:
                target -= lc
                node = left + 1
            tree[node] -= 1
        res[out] = node - size + 1       # số hiệu đứa trẻ (1-based)
        pos = idx
        remaining -= 1

    sys.stdout.write(' '.join(map(str, res)) + '\n')


main()
