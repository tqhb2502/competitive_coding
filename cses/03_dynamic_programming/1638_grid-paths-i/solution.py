# Grid Paths I - https://cses.fi/problemset/task/1638
# DP: dp[j] = số đường đi đến ô (i, j), chỉ đi phải/xuống, tránh bẫy '*'.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    grid = data[1:1 + n]

    MOD = 1000000007

    # dp[j] là số đường đi đến ô (i, j) của hàng hiện tại.
    dp = [0] * n

    for i in range(n):
        row = grid[i]
        for j in range(n):
            # row là bytes; ký tự '*' có mã 42.
            if row[j] == 42:  # '*' -> bẫy
                dp[j] = 0
            elif i == 0 and j == 0:
                dp[j] = 1
            else:
                up = dp[j]                     # giá trị hàng trên tại cột j
                left = dp[j - 1] if j > 0 else 0
                dp[j] = (up + left) % MOD

    sys.stdout.write(str(dp[n - 1] % MOD) + "\n")


main()