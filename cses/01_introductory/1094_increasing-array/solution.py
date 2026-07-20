# Increasing Array - CSES 1094
# https://cses.fi/problemset/task/1094

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    moves = 0
    prev = 0  # giá trị phần tử trước (sau khi chỉnh sửa); mỗi phần tử >= 1
    for i in range(1, n + 1):
        x = int(data[i])
        if x < prev:
            moves += prev - x
        else:
            prev = x
    sys.stdout.write(str(moves) + "\n")


if __name__ == "__main__":
    main()
