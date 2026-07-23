import sys


def main():
    # Doc toan bo du lieu mot lan de tang toc I/O.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])
    coins = [int(v) for v in data[2:2 + n]]
    MOD = 1000000007

    # dp[s] = so cach CO THU TU tao ra tong dung bang s; co so dp[0] = 1.
    dp = [0] * (x + 1)
    dp[0] = 1

    # Sap xep tang dan de vong trong break som khi c > s.
    coins.sort()
    cmax = coins[-1]

    # Vung bien 1 <= s < cmax: chua chac moi dong xu deu dung duoc nen phai kiem
    # tra c <= s; coins da sap xep tang dan nen gap c > s la break som.
    bhi = cmax if cmax <= x else x + 1
    for s in range(1, bhi):
        t = 0
        for c in coins:
            if c > s:
                break
            t += dp[s - c]
        dp[s] = t % MOD

    # Vung nong cmax <= s <= x: moi dong xu deu hop le (s - c >= 0). Sinh san mot
    # ham cong "duoi cuon" (unrolled) bang exec, nhung thang cac menh gia vao bieu
    # thuc de bo vong lap ben trong -> CPython chay nhanh hon dang ke.
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

    # Dap an: dp[x] mod MOD.
    sys.stdout.write(str(dp[x] % MOD) + "\n")


main()
