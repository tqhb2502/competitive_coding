# Coin Combinations I - CSES 1635
# https://cses.fi/problemset/task/1635
#
# dp[s] = số cách CÓ THỨ TỰ (ordered) tạo tổng s.
# dp[0] = 1; dp[s] = sum(dp[s - c] for c in coins if c <= s); kết quả dp[x] % MOD.
#
# Tối ưu: block size B = min(coins). Trong block [L, L+B) mọi chỉ số nguồn
# s - c < L (đã tính xong) vì c >= B, nên cộng cả block bằng vector numpy.

import sys
import numpy as np


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    coins = [int(v) for v in data[2:2 + n]]
    MOD = 1000000007

    dp = np.zeros(x + 1, dtype=np.int64)
    dp[0] = 1

    if x >= 1:
        B = min(coins)  # đảm bảo mọi coin c >= B -> nguồn nằm trước block hiện tại
        for L in range(1, x + 1, B):
            R = min(L + B, x + 1)
            acc = np.zeros(R - L, dtype=np.int64)
            for c in coins:
                t0 = L if L > c else c  # max(L, c): chỉ cộng khi s - c >= 0
                if t0 >= R:
                    continue
                acc[t0 - L:R - L] += dp[t0 - c:R - c]
            dp[L:R] = acc % MOD

    sys.stdout.write(str(int(dp[x])) + "\n")


main()
