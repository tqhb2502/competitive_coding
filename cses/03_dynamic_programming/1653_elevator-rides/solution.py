# Elevator Rides - CSES 1653
# https://cses.fi/problemset/task/1653
#
# Bitmask DP: dp[mask] = (rides, weight_của_chuyến_cuối) được mã hoá thành
# một số nguyên  state = rides * BIG + weight  với BIG = x + 1.
# So sánh min trên số nguyên này chính là so sánh lexicographic (rides, weight).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    w = [int(data[2 + i]) for i in range(n)]

    BIG = x + 1                 # weight luôn <= x nên BIG đảm bảo không tràn bậc
    size = 1 << n
    full = size - 1

    dp = [0] * size
    dp[0] = BIG                 # rides = 1, weight = 0  ->  1 * BIG + 0

    for mask in range(1, size):
        best = 1 << 62
        m = mask
        while m:
            low = m & (-m)
            p = low.bit_length() - 1
            prev = dp[mask ^ low]
            wp = w[p]
            wt = prev % BIG                 # weight của chuyến cuối ở state prev
            if wt + wp <= x:                # nhét được vào chuyến đang mở
                cand = prev + wp
            else:                           # phải mở chuyến mới
                cand = prev - wt + BIG + wp
            if cand < best:
                best = cand
            m ^= low
        dp[mask] = best

    sys.stdout.write(str(dp[full] // BIG) + "\n")


main()
