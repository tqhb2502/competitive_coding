# Subarray Sums I - https://cses.fi/problemset/task/1660
# Two-pointer (sliding window): đếm số subarray có tổng bằng x.
# Áp dụng được vì mọi phần tử đều là số nguyên dương => tổng có tính đơn điệu.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    x = int(data[idx]); idx += 1

    ans = 0
    cur = 0
    left = 0
    # Duyệt right từ 0..n-1, cộng a[right] vào cur.
    for right in range(n):
        cur += int(data[idx + right])
        # Thu hẹp cửa sổ từ bên trái khi tổng vượt quá x.
        while cur > x:
            cur -= int(data[idx + left])
            left += 1
        if cur == x:
            ans += 1

    sys.stdout.write(str(ans) + "\n")


main()
