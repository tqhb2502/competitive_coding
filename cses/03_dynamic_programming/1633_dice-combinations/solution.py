# Dice Combinations - https://cses.fi/problemset/task/1633
# dp[i] = số cách tạo tổng i, dp[0] = 1, dp[i] = sum(dp[i-j], j=1..6) mod 1e9+7
import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])

    MOD = 1000000007

    dp = [0] * (n + 1)
    dp[0] = 1
    # window = dp[i-1] + ... + dp[i-6] duy trì bằng sliding window O(1)/bước
    window = 0
    for i in range(1, n + 1):
        window += dp[i - 1]
        if i - 7 >= 0:
            window -= dp[i - 7]
        window %= MOD
        dp[i] = window

    sys.stdout.write(str(dp[n]) + "\n")


main()
