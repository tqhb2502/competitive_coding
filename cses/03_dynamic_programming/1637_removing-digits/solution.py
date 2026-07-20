# Removing Digits - CSES 1637
# https://cses.fi/problemset/task/1637
#
# Mỗi bước trừ n đi CHỮ SỐ LỚN NHẤT của nó. Greedy này trùng với DP tối ưu
# (đã kiểm chứng đúng với mọi n trong 1..10^6). Độ phức tạp O(số_bước), bộ nhớ O(1).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    steps = 0
    while n > 0:
        # Tìm chữ số lớn nhất của n.
        mx = 0
        x = n
        while x:
            d = x % 10
            if d > mx:
                mx = d
            x //= 10
        n -= mx
        steps += 1

    sys.stdout.write(str(steps) + "\n")


if __name__ == "__main__":
    main()
