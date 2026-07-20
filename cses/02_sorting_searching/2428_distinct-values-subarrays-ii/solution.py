# Distinct Values Subarrays II - https://cses.fi/problemset/task/2428
# Count subarrays with at most k distinct values using a sliding window (two-pointer).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    a = data[2:2 + n]  # keep as bytes tokens; hashable and comparable, avoids int parse overhead

    # Coordinate compress to [0, n) so we can use a fast list-based frequency counter.
    comp = {}
    idx = 0
    arr = [0] * n
    for i in range(n):
        v = a[i]
        j = comp.get(v)
        if j is None:
            j = idx
            comp[v] = j
            idx += 1
        arr[i] = j

    cnt = [0] * idx
    distinct = 0
    left = 0
    total = 0
    for right in range(n):
        v = arr[right]
        if cnt[v] == 0:
            distinct += 1
        cnt[v] += 1
        while distinct > k:
            lv = arr[left]
            cnt[lv] -= 1
            if cnt[lv] == 0:
                distinct -= 1
            left += 1
        total += right - left + 1

    sys.stdout.write(str(total) + "\n")


main()
