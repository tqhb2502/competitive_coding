# Two Sets II - CSES 1093
# https://cses.fi/problemset/task/1093
#
# Dem so cach chia 1..n thanh hai tap co tong bang nhau (mod 1e9+7).
# total = n*(n+1)/2. Neu le -> 0. Nguoc lai target = total/2 va ta dem so subset
# cua {1,...,n-1} co tong = target (co dinh n o tap thu hai de moi cach chia chi
# duoc dem dung 1 lan).
#
# Thuan Python standard library, khong dung numpy hay bat ky thu vien ben ngoai nao.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 1000000007

    total = n * (n + 1) // 2
    if total & 1:
        # Tong le -> khong the chia deu.
        sys.stdout.write("0\n")
        return
    target = total // 2

    # DP 0/1 knapsack: dp[j] = so subset cua {1,...,k} co tong = j (mod MOD).
    # Chi dem cac phan tu 1..n-1 (bo qua n) de moi cach chia duoc dem dung 1 lan.
    m = n - 1
    s_full = m * (m + 1) // 2          # tong cua tat ca cac item 1..m
    gap = s_full - target              # tong toi da co the "bo di" khoi target

    dp = [0] * (target + 1)
    dp[0] = 1

    s_k = 0                            # tong cac item da xet 1..k
    for k in range(1, m + 1):
        s_k += k
        # Cua so [lo, hi] cac chi so j thuc su can cap nhat:
        #   hi = min(target, s_k)      -> tong lon nhat dat duoc voi item 1..k
        #   lo = max(k, s_k - gap)     -> duoi nguong nay khong the cham target
        hi = target if s_k > target else s_k
        lo = s_k - gap
        if lo < k:
            lo = k
        # Duyet j giam dan de moi item chi dung mot lan trong mot subset.
        for j in range(hi, lo - 1, -1):
            v = dp[j] + dp[j - k]
            if v >= MOD:
                v -= MOD
            dp[j] = v

    sys.stdout.write(str(dp[target]) + "\n")


main()
