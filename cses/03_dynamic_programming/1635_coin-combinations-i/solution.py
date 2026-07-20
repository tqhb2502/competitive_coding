# Coin Combinations I - CSES 1635
# https://cses.fi/problemset/task/1635
#
# dp[s] = so cach CO THU TU (ordered) tao tong s.
# dp[0] = 1; dp[s] = sum(dp[s - c] for c in coins if c <= s); ket qua dp[x] % MOD.
#
# Thuan stdlib. Tach lam 2 vung:
#   - Vung bien s < cmax: khong phai moi dong xu deu dung duoc (can kiem tra c <= s).
#   - Vung nong s >= cmax: moi dong xu deu hop le nen sinh san bieu thuc cong
#     "duoi cuon" (unrolled) qua exec de bo vong lap ben trong -> nhanh hon o CPython.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])
    coins = [int(v) for v in data[2:2 + n]]
    MOD = 1000000007

    dp = [0] * (x + 1)
    dp[0] = 1

    coins.sort()
    cmax = coins[-1]

    # Vung bien: 1 <= s < cmax. Coins da sap xep tang dan nen break som khi c > s.
    bhi = cmax if cmax <= x else x + 1
    for s in range(1, bhi):
        t = 0
        for c in coins:
            if c > s:
                break
            t += dp[s - c]
        dp[s] = t % MOD

    # Vung nong: cmax <= s <= x. Moi dong xu deu hop le (s - c >= 0).
    if x >= cmax:
        terms = " + ".join("d[s-%d]" % c for c in coins)
        src = (
            "def hot(d, lo, hi, MOD):\n"
            " for s in range(lo, hi):\n"
            "  d[s] = (" + terms + ") % MOD\n"
        )
        namespace = {}
        exec(src, namespace)
        namespace["hot"](dp, cmax, x + 1, MOD)

    sys.stdout.write(str(dp[x] % MOD) + "\n")


main()
