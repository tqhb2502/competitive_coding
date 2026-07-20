# Longest Common Subsequence - CSES 3403
# https://cses.fi/problemset/task/3403
#
# Classic LCS with reconstruction.
# dp[i][j] = length of LCS of a[0..i-1] and b[0..j-1].
# Values are compared as byte-strings (no int conversion needed).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    a = data[idx:idx + n]; idx += n
    b = data[idx:idx + m]; idx += m

    # Full DP table for reconstruction: (n+1) x (m+1)
    prev = [0] * (m + 1)
    dp = [prev]
    for i in range(n):
        ai = a[i]
        cur = [0] * (m + 1)
        for j in range(m):
            if ai == b[j]:
                cur[j + 1] = prev[j] + 1
            else:
                left = cur[j]
                up = prev[j + 1]
                cur[j + 1] = left if left >= up else up
        dp.append(cur)
        prev = cur

    length = dp[n][m]

    # Backtrack one valid LCS (any valid sequence is accepted).
    res = []
    i, j = n, m
    while i > 0 and j > 0:
        if a[i - 1] == b[j - 1]:
            res.append(a[i - 1])
            i -= 1
            j -= 1
        elif dp[i - 1][j] >= dp[i][j - 1]:
            i -= 1
        else:
            j -= 1
    res.reverse()

    out = sys.stdout
    out.write(str(length))
    out.write('\n')
    out.write(b' '.join(res).decode())
    out.write('\n')


main()
