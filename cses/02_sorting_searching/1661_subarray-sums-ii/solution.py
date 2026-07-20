# Subarray Sums II - https://cses.fi/problemset/task/1661
# Count subarrays with sum exactly x. Values may be negative, so use
# prefix sum + hash map (two-pointer does not work with negatives).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])

    count = {0: 1}  # prefix sum -> number of occurrences
    prefix = 0
    ans = 0

    idx = 2
    for i in range(n):
        prefix += int(data[idx + i])
        # subarrays ending here with sum x need prior prefix == prefix - x
        ans += count.get(prefix - x, 0)
        count[prefix] = count.get(prefix, 0) + 1

    sys.stdout.write(str(ans) + "\n")


main()
