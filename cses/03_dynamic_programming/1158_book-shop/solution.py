# Book Shop - CSES 1158
# https://cses.fi/problemset/task/1158
# 0/1 knapsack: tối đa hóa số trang với ngân sách x, mỗi cuốn mua tối đa 1 lần.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    x = int(data[idx]); idx += 1
    prices = data[idx:idx + n]; idx += n
    pages = data[idx:idx + n]; idx += n

    try:
        import numpy as np
        # Fast path: vectorized 0/1 knapsack.
        dp = np.zeros(x + 1, dtype=np.int64)
        for i in range(n):
            p = int(prices[i])
            v = int(pages[i])
            if p > x:
                continue
            # dp[j] = max(dp[j], dp[j - p] + v). Đọc dp[:x+1-p] là giá trị cũ.
            dp[p:] = np.maximum(dp[p:], dp[:x + 1 - p] + v)
        sys.stdout.write(str(int(dp[x])) + "\n")
    except ImportError:
        # Fallback: pure Python 1D knapsack, duyệt j giảm dần.
        dp = [0] * (x + 1)
        for i in range(n):
            p = int(prices[i])
            v = int(pages[i])
            if p > x:
                continue
            row = dp
            for j in range(x, p - 1, -1):
                cand = row[j - p] + v
                if cand > row[j]:
                    row[j] = cand
        sys.stdout.write(str(dp[x]) + "\n")


main()
