import sys
from bisect import bisect_left


def subset_sums(arr):
    # Sinh tất cả tổng của mọi tập con: bắt đầu {0}, mỗi phần tử làm số tổng gấp đôi.
    sums = [0]
    for x in arr:
        sums += [s + x for s in sums]
    return sums


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    # Đọc n quả táo và tính tổng trọng lượng total.
    n = int(data[0])
    w = [int(x) for x in data[1:1 + n]]
    total = sum(w)

    # Meet in the middle: chia mảng làm 2 nửa, sinh subset-sum của từng nửa.
    half = n // 2
    A = subset_sums(w[:half])
    B = subset_sums(w[half:])
    B.sort()  # sắp xếp nửa phải để tìm kiếm nhị phân
    m = len(B)

    # Với mỗi tổng a của nửa trái, chọn b ở nửa phải sao cho
    # |total - 2*(a+b)| nhỏ nhất, tức b gần key/2 nhất.
    best = float('inf')
    for a in A:
        key = total - 2 * a          # muốn 2*b gần key nhất
        target = key / 2.0           # tức là b gần key/2 nhất
        pos = bisect_left(B, target)
        # Chỉ cần thử 2 ứng viên liền kề quanh vị trí key/2.
        for j in (pos - 1, pos):
            if 0 <= j < m:
                diff = key - 2 * B[j]
                if diff < 0:
                    diff = -diff
                if diff < best:
                    best = diff

    sys.stdout.write(str(int(best)) + "\n")


main()
