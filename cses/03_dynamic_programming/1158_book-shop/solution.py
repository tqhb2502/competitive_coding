# Book Shop - CSES 1158
# https://cses.fi/problemset/task/1158
# 0/1 knapsack: toi da hoa so trang voi ngan sach x, moi cuon mua toi da 1 lan.
# Pure stdlib. Toi uu bang cach vectorize buoc cap nhat qua slicing + list
# comprehension (chay o tang C cua CPython) thay cho vong for chi so tung phan tu.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    prices = data[2:2 + n]
    pages = data[2 + n:2 + 2 * n]

    # dp[j] = so trang lon nhat voi tong gia KHONG VUOT QUA j (0 <= j <= x).
    # Khoi tao 0: chua xet cuon nao thi moi ngan sach cho 0 trang.
    dp = [0] * (x + 1)
    top = x + 1

    for i in range(n):
        p = int(prices[i])
        if p > x:
            continue                 # cuon nay khong bao gio mua duoc
        v = int(pages[i])

        # Voi moi j tu p den x: dp[j] = max(dp[j], dp[j - p] + v).
        # seg_new = dp[p:x+1]   -> dp[j] hien tai ("khong mua cuon i").
        # seg_old = dp[0:x+1-p] -> dp[j - p] truoc khi xet cuon i ("mua cuon i").
        # Doc bang slice tao ban sao doc lap truoc khi ghi de nen khong co xung
        # dot thu tu doc/ghi; tuong duong vong lap 0/1 knapsack duyet j giam dan.
        # dp luon khong giam theo j nen ket qua duy tri tinh don dieu.
        seg_new = dp[p:top]
        seg_old = dp[0:top - p]
        dp[p:top] = [a + v if a + v > b else b
                     for a, b in zip(seg_old, seg_new)]

    sys.stdout.write(str(dp[x]) + "\n")


main()
