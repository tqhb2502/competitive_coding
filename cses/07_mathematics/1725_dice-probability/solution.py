# Dice Probability - https://cses.fi/problemset/task/1725
# DP trên phân phối xác suất của tổng n con xúc xắc.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    a = int(data[1])
    b = int(data[2])

    # dp[s] = xác suất tổng hiện tại bằng s
    dp = [0.0] * (6 * n + 1)
    dp[0] = 1.0
    p = 1.0 / 6.0
    cur_max = 0
    for _ in range(n):
        ndp = [0.0] * (6 * n + 1)
        for s in range(cur_max + 1):
            v = dp[s]
            if v == 0.0:
                continue
            v *= p
            ndp[s + 1] += v
            ndp[s + 2] += v
            ndp[s + 3] += v
            ndp[s + 4] += v
            ndp[s + 5] += v
            ndp[s + 6] += v
        cur_max += 6
        dp = ndp

    ans = sum(dp[a:b + 1])
    sys.stdout.write("%.6f\n" % ans)


main()
