# Subarray Divisibility - https://cses.fi/problemset/task/1662
# Count subarrays whose sum is divisible by n using prefix sums modulo n.
# Two prefixes with the same residue mod n bound a subarray divisible by n.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    # cnt[r] = number of prefixes seen so far with residue r (mod n).
    cnt = [0] * n
    cnt[0] = 1  # empty prefix P[0] = 0
    prefix = 0
    ans = 0
    for i in range(1, n + 1):
        prefix += int(data[i])
        r = prefix % n  # Python's % already yields a non-negative result for positive n
        ans += cnt[r]
        cnt[r] += 1
    sys.stdout.write(str(ans) + "\n")


main()
