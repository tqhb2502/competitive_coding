# Distinct Values Subarrays - CSES 3420
# https://cses.fi/problemset/task/3420
# Two-pointer (sliding window): đếm số subarray có tất cả phần tử phân biệt.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    a = data[1:1 + n]

    last = {}          # giá trị -> vị trí xuất hiện gần nhất
    left = 0           # con trỏ trái nhỏ nhất cho cửa sổ phân biệt
    total = 0
    for right in range(n):
        v = a[right]
        p = last.get(v)
        if p is not None and p >= left:
            left = p + 1
        total += right - left + 1
        last[v] = right

    sys.stdout.write(str(total) + "\n")


main()
