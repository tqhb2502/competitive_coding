# Increasing Subsequence - CSES 1145
# https://cses.fi/problemset/task/1145
# Longest strictly increasing subsequence (LIS) in O(n log n)
# dùng patience sorting + binary search (bisect_left cho tăng nghiêm ngặt)

import sys
from bisect import bisect_left


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        print(0)
        return
    n = int(data[0])
    # tails[i] = smallest possible tail of an increasing subsequence of length i+1
    tails = []
    for i in range(1, n + 1):
        x = int(data[i])
        pos = bisect_left(tails, x)
        if pos == len(tails):
            tails.append(x)
        else:
            tails[pos] = x
    sys.stdout.write(str(len(tails)) + "\n")


main()
