# Distinct Subsequences - https://cses.fi/problemset/task/1149
# Đếm số subsequence phân biệt khác rỗng, modulo 1e9+7.
# DP kinh điển: dp[i] = 2*dp[i-1] - dp[last-1], đáp án = dp[n] - 1.

import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    s = data[0]

    MOD = 1_000_000_007
    dp = 1               # dp[0]: chỉ có chuỗi rỗng
    last = [0] * 26      # last[c] = dp[j-1] của lần xuất hiện gần nhất của c
    seen = [False] * 26

    for byte in s:
        c = byte - 97    # 'a' == 97
        new = (2 * dp) % MOD
        if seen[c]:
            new = (new - last[c]) % MOD
        last[c] = dp
        seen[c] = True
        dp = new

    ans = (dp - 1) % MOD
    sys.stdout.write(str(ans) + "\n")


main()
