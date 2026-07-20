# Array Description - CSES 1746
# https://cses.fi/problemset/task/1746
# DP: dp[v] = so cach dien tien to sao cho phan tu cuoi mang gia tri v (1..m).
# Chuyen trang thai: dp_new[v] = dp[v-1] + dp[v] + dp[v+1] (mod 1e9+7).
# Thuan stdlib: cong ba lat cat (slice) cua list cung luc bang zip.
# Hoan mod (defer): tich luy khong mod trong toi da 16 buoc roi moi mod mot lan.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    MOD = 1000000007

    # dp co do dai m+2, chi so 0 va m+1 la padding (luon = 0) de tranh kiem tra bien.
    dp = [0] * (m + 2)
    x0 = int(data[2])
    if x0 == 0:
        for v in range(1, m + 1):
            dp[v] = 1
    else:
        dp[x0] = 1

    cnt = 0
    for i in range(1, n):
        xi = int(data[2 + i])
        if xi == 0:
            # core[k] = dp[k] + dp[k+1] + dp[k+2], k = 0..m-1
            # => dp_new[v] = core[v-1] = dp[v-1] + dp[v] + dp[v+1]
            core = [a + b + c for a, b, c in zip(dp, dp[1:], dp[2:])]
            cnt += 1
            if cnt == 16:
                # Sau 16 buoc, gia tri < 3^16 * 1e9 ~ 4.3e16, Python int chinh xac tuyet doi.
                core = [v % MOD for v in core]
                cnt = 0
            dp = [0]
            dp += core
            dp.append(0)
        else:
            val = (dp[xi - 1] + dp[xi] + dp[xi + 1]) % MOD
            dp = [0] * (m + 2)
            dp[xi] = val
            cnt = 0

    sys.stdout.write(str(sum(dp) % MOD) + "\n")


main()
