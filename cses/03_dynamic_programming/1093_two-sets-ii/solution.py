# Two Sets II - CSES 1093
# https://cses.fi/problemset/task/1093
#
# Đếm số cách chia 1..n thành hai tập có tổng bằng nhau (mod 1e9+7).
# total = n*(n+1)/2. Nếu lẻ -> 0. Ngược lại target = total/2 và ta đếm số subset
# của {1,...,n-1} có tổng = target (bỏ qua n để mỗi cách chia chỉ đếm 1 lần).
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 10 ** 9 + 7

    total = n * (n + 1) // 2
    if total % 2 != 0:
        # Tổng lẻ -> không thể chia đều
        sys.stdout.write("0\n")
        return
    target = total // 2

    # DP 0/1 knapsack: dp[j] = số subset của {1,...,n-1} có tổng = j.
    try:
        import numpy as np
        dp = np.zeros(target + 1, dtype=np.int64)
        dp[0] = 1
        for k in range(1, n):
            # dp[j] += dp[j-k] cho j = k..target (vế phải tạo mảng mới từ dp cũ,
            # nên không bị lỗi ghi đè khi cập nhật).
            dp[k:] = (dp[k:] + dp[:target - k + 1]) % MOD
        ans = int(dp[target]) % MOD
    except ImportError:
        # Fallback thuần CPython (chậm hơn, có thể TLE trên CSES).
        dp = [0] * (target + 1)
        dp[0] = 1
        for k in range(1, n):
            for j in range(target, k - 1, -1):
                v = dp[j] + dp[j - k]
                if v >= MOD:
                    v -= MOD
                dp[j] = v
        ans = dp[target] % MOD

    sys.stdout.write(str(ans) + "\n")


main()
