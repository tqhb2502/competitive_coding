# Maximum Subarray Sum - CSES 1643
# https://cses.fi/problemset/task/1643
# Kadane's algorithm: O(n) time, O(1) extra space.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    # data[1 .. n] are the array values.
    # Subarray must be nonempty, so start from the first element.
    cur = int(data[1])
    ans = cur
    for i in range(2, n + 1):
        x = int(data[i])
        cur = x if cur < 0 else cur + x  # cur = max(x, cur + x)
        if cur > ans:
            ans = cur
    sys.stdout.write(str(ans) + "\n")


main()
