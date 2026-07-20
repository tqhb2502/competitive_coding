# Rectangle Cutting - CSES 1744
# https://cses.fi/problemset/task/1744
#
# dp[i][j] = số lần cắt tối thiểu để chia hình chữ nhật i x j thành các hình vuông.
# base: dp[i][i] = 0 (đã là hình vuông).
# transition:
#   - cắt ngang (horizontal): dp[i][j] = min_{1<=k<j} dp[i][k] + dp[i][j-k] + 1
#   - cắt dọc  (vertical)   : dp[i][j] = min_{1<=k<i} dp[k][j] + dp[i-k][j] + 1
# đáp án: dp[a][b].
#
# Dùng numpy để vectorize chiều vertical + tính horizontal theo từng ô.
# Đối xứng dp[i][j] = dp[j][i] để giảm một nửa công việc.
# Nếu judge không có numpy -> fallback về pure Python (chậm hơn).

import sys


def solve_numpy(a, b, np):
    N = max(a, b)
    dp = np.zeros((N + 1, N + 1), dtype=np.int64)
    for i in range(1, N + 1):
        row = dp[i]
        # best_v[j] = min_{k=1..i-1} dp[k][j] + dp[i-k][j]  (vectorize theo cột)
        best_v = None
        if i >= 2:
            best_v = dp[1] + dp[i - 1]
            for k in range(2, i):
                np.minimum(best_v, dp[k] + dp[i - k], out=best_v)
        # tính cho j > i, rồi mirror sang dp[j][i]
        for j in range(i + 1, N + 1):
            # horizontal: min_{k=1..j-1} row[k] + row[j-k]
            bh = int((row[1:j] + row[j - 1:0:-1]).min())
            if best_v is not None:
                v = int(best_v[j])
                if v < bh:
                    bh = v
            val = bh + 1
            row[j] = val
            dp[j, i] = val
    return int(dp[a, b])


def solve_pure(a, b):
    N = max(a, b)
    W = N + 1
    INF = 1 << 30
    dp = [0] * (W * W)  # dp[i][j] tại chỉ số i*W + j
    for i in range(1, N + 1):
        base = i * W
        half_i = i // 2
        for j in range(i + 1, N + 1):
            best = INF
            # horizontal (chỉ cần k đến j//2 do đối xứng 2 mảnh)
            for k in range(1, j // 2 + 1):
                v = dp[base + k] + dp[base + j - k]
                if v < best:
                    best = v
            # vertical (chỉ cần k đến i//2)
            for k in range(1, half_i + 1):
                v = dp[k * W + j] + dp[(i - k) * W + j]
                if v < best:
                    best = v
            best += 1
            dp[base + j] = best
            dp[j * W + i] = best
    return dp[a * W + b]


def main():
    data = sys.stdin.buffer.read().split()
    a = int(data[0])
    b = int(data[1])
    if a == b:
        sys.stdout.write("0\n")
        return
    try:
        import numpy as np
    except ImportError:
        np = None
    if np is not None:
        ans = solve_numpy(a, b, np)
    else:
        ans = solve_pure(a, b)
    sys.stdout.write(str(ans) + "\n")


main()
