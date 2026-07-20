# Factory Machines - CSES 1620
# https://cses.fi/problemset/task/1620
# Binary search on the answer: find smallest T with sum(T // k_i) >= t.
# The per-step sum is vectorized with numpy so the ~60 binary-search
# iterations over n = 2*10^5 machines stay well within the time limit.

import sys

import numpy as np


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    t = int(data[1])
    k = np.array(list(map(int, data[2:2 + n])), dtype=np.int64)

    # Upper bound: fastest machine alone finishes t products in min(k) * t seconds.
    lo = 1
    hi = int(k.min()) * t

    while lo < hi:
        mid = (lo + hi) // 2
        # Total products all machines can make in 'mid' seconds.
        # Each term mid // k_i <= mid // min(k) <= t <= 10^9, so the int64
        # sum (<= n * t = 2*10^14) never overflows.
        if int((mid // k).sum()) >= t:
            hi = mid
        else:
            lo = mid + 1

    sys.stdout.write(str(lo) + "\n")


main()
