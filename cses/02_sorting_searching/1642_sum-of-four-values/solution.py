# Sum of Four Values - CSES 1642
# https://cses.fi/problemset/task/1642
#
# Approach: O(n^2) hashmap of pair sums.
# Iterate i as the smaller index of the "high" pair. For each i, first search
# every j > i for need = x - a[i] - a[j] among previously stored pair sums;
# then insert all pairs (k, i) with k < i. Because insertion happens AFTER the
# search, every stored pair (k, l) has both indices < i, and j > i, so the four
# indices satisfy k < l < i < j and are all distinct.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    a = [int(v) for v in data[2:2 + n]]

    seen = {}  # pair sum -> (k, l) with k < l, both indices already fixed
    get = seen.get

    for i in range(n):
        ai = a[i]
        # 1) SEARCH: use i as the smaller index of the high pair.
        for j in range(i + 1, n):
            p = get(x - ai - a[j])
            if p is not None:
                k, l = p
                sys.stdout.write("%d %d %d %d\n" % (k + 1, l + 1, i + 1, j + 1))
                return
        # 2) INSERT: all pairs (k, i) with k < i (keep only first per sum).
        for k in range(i):
            s = a[k] + ai
            if s not in seen:
                seen[s] = (k, i)

    sys.stdout.write("IMPOSSIBLE\n")


main()
