# Missing Number - CSES 1083
# https://cses.fi/problemset/task/1083

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    total = n * (n + 1) // 2
    s = 0
    for i in range(1, n):
        s += int(data[i])
    sys.stdout.write(str(total - s) + "\n")


if __name__ == "__main__":
    main()
