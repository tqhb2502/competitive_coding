# Christmas Party - https://cses.fi/problemset/task/1717
# Count derangements D(n) mod 1e9+7 using the recurrence
# D(1)=0, D(2)=1, D(n) = (n-1) * (D(n-1) + D(n-2)).
import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 1_000_000_007

    if n == 1:
        sys.stdout.write("0\n")
        return

    # d_prev2 = D(k-1), d_prev1 = D(k); start with D(1)=0, D(2)=1
    d_prev2 = 0  # D(1)
    d_prev1 = 1  # D(2)
    if n == 2:
        sys.stdout.write("1\n")
        return

    for k in range(3, n + 1):
        d_cur = (k - 1) * (d_prev1 + d_prev2) % MOD
        d_prev2 = d_prev1
        d_prev1 = d_cur

    sys.stdout.write(str(d_prev1) + "\n")


main()
