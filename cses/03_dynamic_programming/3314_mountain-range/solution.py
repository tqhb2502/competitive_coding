# Mountain Range - CSES task 3314
# https://cses.fi/problemset/task/3314
#
# Tu nui i ta co the luon toi moi nui j nam giua L_i va R_i (khong ke i),
# voi L_i / R_i la vi tri gan nhat co chieu cao >= h_i ben trai / ben phai.
# Khi do i la cuc dai duy nhat cua doan [L_i+1, R_i-1] va moi nui j trong doan
# deu thap hon h_i thuc su nen luon toi duoc.
#
# dp[i] = do dai hanh trinh dai nhat bat dau tu nui i = 1 + max(dp[j]) voi j
# reachable tu i. Ta tinh dp theo thu tu chieu cao TANG DAN. Moi nui i, sau khi
# tinh xong dp[i], "day" gia tri nay len hai "cha" cua no la SL[i] va SR[i] (nui
# gan nhat cao hon THUC SU ben trai / phai). Cac cuc dai cua doan reachable cua
# mot nui chinh la nhung nui nhan no lam SL hoac SR, nen childmax[i] tich luy dung
# max(dp[j]) tren toan bo doan reachable cua i. O(n log n) do sort, phan con lai O(n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    if n <= 0:
        sys.stdout.write("0\n")
        return
    h = [int(x) for x in data[1:1 + n]]

    # SL[i]: vi tri gan nhat ben trai co chieu cao > h[i] (strictly), hoac -1
    # SR[i]: vi tri gan nhat ben phai co chieu cao > h[i] (strictly), hoac n
    SL = [-1] * n
    SR = [n] * n
    stack = []
    for i in range(n):
        hi = h[i]
        while stack and h[stack[-1]] <= hi:
            stack.pop()
        if stack:
            SL[i] = stack[-1]
        stack.append(i)
    stack.clear()
    for i in range(n - 1, -1, -1):
        hi = h[i]
        while stack and h[stack[-1]] <= hi:
            stack.pop()
        if stack:
            SR[i] = stack[-1]
        stack.append(i)

    childmax = [0] * n
    order = sorted(range(n), key=h.__getitem__)  # tang dan theo chieu cao

    ans = 0
    for i in order:
        d = childmax[i] + 1
        if d > ans:
            ans = d
        l = SL[i]
        if l >= 0 and d > childmax[l]:
            childmax[l] = d
        r = SR[i]
        if r < n and d > childmax[r]:
            childmax[r] = d

    sys.stdout.write(str(ans) + "\n")


main()
