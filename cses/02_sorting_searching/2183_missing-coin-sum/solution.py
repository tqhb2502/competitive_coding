# Missing Coin Sum - CSES 2183
# https://cses.fi/problemset/task/2183
#
# Greedy sau khi sắp xếp: duy trì reach = tổng nhỏ nhất chưa tạo được (bắt đầu = 1).
# Với mỗi đồng xu x theo thứ tự tăng dần: nếu x <= reach thì reach += x,
# ngược lại dừng và in ra reach.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    coins = list(map(int, data[1:1 + n]))
    coins.sort()

    reach = 1  # smallest sum we currently cannot form
    for x in coins:
        if x <= reach:
            reach += x
        else:
            break

    sys.stdout.write(str(reach) + "\n")


if __name__ == "__main__":
    main()
