# Apple Division - CSES 1623
# https://cses.fi/problemset/task/1623
#
# Chia n quả táo thành 2 nhóm sao cho hiệu tổng trọng lượng nhỏ nhất.
# Chọn tập con S (nhóm 1) -> hiệu = |total - 2*sum(S)|.
# Dùng meet-in-the-middle: chia mảng làm 2 nửa, sinh mọi subset-sum của từng nửa,
# rồi ghép lại và tìm kết quả tối ưu bằng tìm kiếm nhị phân.

import sys
from bisect import bisect_left


def subset_sums(arr):
    sums = [0]
    for x in arr:
        sums += [s + x for s in sums]
    return sums


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    w = [int(x) for x in data[1:1 + n]]
    total = sum(w)

    half = n // 2
    A = subset_sums(w[:half])
    B = subset_sums(w[half:])
    B.sort()
    m = len(B)

    best = float('inf')
    for a in A:
        key = total - 2 * a          # muốn 2*b gần key nhất
        target = key / 2.0           # tức là b gần key/2 nhất
        pos = bisect_left(B, target)
        for j in (pos - 1, pos):
            if 0 <= j < m:
                diff = key - 2 * B[j]
                if diff < 0:
                    diff = -diff
                if diff < best:
                    best = diff

    sys.stdout.write(str(int(best)) + "\n")


main()
