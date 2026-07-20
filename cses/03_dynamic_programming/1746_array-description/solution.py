# Array Description - CSES 1746
# https://cses.fi/problemset/task/1746
# DP: dp[v] = số cách điền tiền tố sao cho phần tử cuối mang giá trị v (1..m).
# Chuyển trạng thái: dp_new[v] = dp[v-1] + dp[v] + dp[v+1] (mod 1e9+7).
# Vector hoá bằng numpy trên trục v; hoãn mod (defer) để giảm số phép tính.

import sys
import numpy as np


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    MOD = 1000000007

    # dp có độ dài m+2, chỉ số 0 và m+1 là padding (luôn = 0) để tránh kiểm tra biên.
    dp = np.zeros(m + 2, dtype=np.int64)
    x0 = int(data[2])
    if x0 == 0:
        dp[1:m + 1] = 1
    else:
        dp[x0] = 1

    # Hoãn mod: cộng dồn không mod trong tối đa 16 bước rồi mới lấy mod một lần.
    # Giá trị tăng tối đa x3 mỗi bước, sau 16 bước < 3^16 * 1e9 ~ 4.3e16, an toàn int64.
    count = 0
    for i in range(1, n):
        xi = int(data[2 + i])
        if xi == 0:
            core = dp[0:m] + dp[1:m + 1] + dp[2:m + 2]
            count += 1
            if count == 16:
                core %= MOD
                count = 0
            dp[1:m + 1] = core
        else:
            val = (int(dp[xi - 1]) + int(dp[xi]) + int(dp[xi + 1])) % MOD
            dp[:] = 0
            dp[xi] = val
            count = 0

    print(int(dp.sum() % MOD))


main()
