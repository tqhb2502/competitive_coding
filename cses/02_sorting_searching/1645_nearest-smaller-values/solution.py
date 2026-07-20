# Nearest Smaller Values - CSES 1645
# https://cses.fi/problemset/task/1645
# Monotonic stack: with each position find the nearest position to its left
# having a strictly smaller value (1-indexed), or 0 if none exists.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    a = list(map(int, data[1:1 + n]))

    res = [0] * n          # res[i] = 1-indexed answer for position i
    stack = []             # stack of indices (0-based), values strictly increasing

    for i in range(n):
        x = a[i]
        # Pop indices whose value is >= current value; they can never answer
        # for i or for anything to the right of i.
        while stack and a[stack[-1]] >= x:
            stack.pop()
        if stack:
            res[i] = stack[-1] + 1   # convert to 1-indexed
        # else res[i] stays 0
        stack.append(i)

    sys.stdout.write(' '.join(map(str, res)))
    sys.stdout.write('\n')


main()
