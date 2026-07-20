# Rectangle Cutting - CSES 1744
# https://cses.fi/problemset/task/1744
#
# dp[i][j] = so lan cat toi thieu de chia hinh chu nhat i x j thanh cac hinh vuong.
# base: dp[i][i] = 0 (da la hinh vuong).
# transition:
#   - cat theo canh j (giu i): dp[i][j] = min_{1<=k<j} dp[i][k] + dp[i][j-k] + 1
#   - cat theo canh i (giu j): dp[i][j] = min_{1<=k<i} dp[k][j] + dp[i-k][j] + 1
# dap an: dp[a][b].
#
# Doi xung dp[i][j] = dp[j][i] cho phep viet CA HAI huong cat duoi dang
# "row[k] + row[len-k]" tren mot hang, nho do gop min bang map(add,...) chay C.
# Thuan stdlib: khong numpy, khong module ben thu ba.

import sys
from operator import add


def main():
    data = sys.stdin.buffer.read().split()
    a = int(data[0])
    b = int(data[1])
    if a == b:
        sys.stdout.write("0\n")
        return

    N = a if a > b else b
    # dp la list-of-list (N+1) x (N+1), khoi tao 0 (dp[i][i] = 0 san).
    dp = [[0] * (N + 1) for _ in range(N + 1)]

    _min = min
    _map = map

    for i in range(1, N + 1):
        row_i = dp[i]
        hi = i >> 1  # i // 2
        for j in range(i + 1, N + 1):
            # Cat theo canh j (giu i): dp[i][k] + dp[i][j-k], k = 1..j//2
            jh = j >> 1
            best = _min(_map(add, row_i[1:jh + 1], row_i[j - 1:j - 1 - jh:-1]))
            # Cat theo canh i (giu j): dp[k][j] + dp[i-k][j], k = 1..i//2.
            # dp[k][j] = dp[j][k] = row_j[k] (doi xung), voi k, i-k < i < j nen
            # cac o nay deu da duoc dien (o hang j da mirror). i >= 2 moi co huong nay.
            if hi:
                row_j = dp[j]
                v = _min(_map(add, row_j[1:hi + 1], row_j[i - 1:i - 1 - hi:-1]))
                if v < best:
                    best = v
            best += 1
            row_i[j] = best
            dp[j][i] = best  # mirror

    sys.stdout.write(str(dp[a][b]) + "\n")


main()
