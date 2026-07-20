# Ferris Wheel - https://cses.fi/problemset/task/1090
# Greedy + two-pointer: sort weights, pair lightest with heaviest when possible.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    p = [int(v) for v in data[2:2 + n]]
    p.sort()

    left = 0
    right = n - 1
    ans = 0
    while left <= right:
        if p[left] + p[right] <= x:
            left += 1
        right -= 1
        ans += 1

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
