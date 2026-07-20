# Array Division - CSES 1085
# https://cses.fi/problemset/task/1085
# Binary search on the answer (minimize the maximum subarray sum) + greedy check.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    arr = [int(v) for v in data[2:2 + n]]

    # feasible(limit): can we split arr into <= k contiguous parts,
    # each with sum <= limit? Greedy: pack as much as possible per part.
    def feasible(limit):
        parts = 1
        cur = 0
        for v in arr:
            if cur + v > limit:
                parts += 1
                cur = v
                if parts > k:
                    return False
            else:
                cur += v
        return parts <= k

    lo = max(arr)          # each single element must fit into a part
    hi = sum(arr)          # k == 1 -> whole array is one part
    while lo < hi:
        mid = (lo + hi) // 2
        if feasible(mid):
            hi = mid
        else:
            lo = mid + 1

    sys.stdout.write(str(lo) + "\n")


main()
