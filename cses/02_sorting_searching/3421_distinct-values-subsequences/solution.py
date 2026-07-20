# Distinct Values Subsequences - https://cses.fi/problemset/task/3421
# Answer = (product of (1 + cnt[v]) over distinct values v) - 1  (mod 1e9+7)
import sys
from collections import Counter


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    xs = data[1:1 + n]
    MOD = 10 ** 9 + 7
    cnt = Counter(xs)  # count by raw token; distinct tokens == distinct values
    ans = 1
    for c in cnt.values():
        ans = ans * (1 + c) % MOD
    ans = (ans - 1) % MOD
    sys.stdout.write(str(ans) + "\n")


main()
