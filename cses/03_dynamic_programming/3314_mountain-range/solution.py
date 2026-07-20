# Mountain Range - CSES task 3314
# https://cses.fi/problemset/task/3314
#
# Từ núi i ta có thể lượn tới mọi núi j nằm giữa L_i và R_i (không kể i),
# với L_i / R_i là vị trí gần nhất có chiều cao >= h_i bên trái / bên phải.
# Khi đó i là cực đại duy nhất của đoạn [L_i+1, R_i-1] và mọi núi j trong đoạn
# đều thấp hơn h_i thực sự nên luôn lượn tới được.
#
# dp[i] = độ dài hành trình dài nhất bắt đầu từ núi i = 1 + max(dp[j]) với j
# lượn tới được từ i. Ta tính dp theo thứ tự chiều cao TĂNG DẦN. Mỗi núi i, sau khi
# tính xong dp[i], "đẩy" giá trị này lên hai "cha" của nó là SL[i] và SR[i] (núi
# gần nhất cao hơn THỰC SỰ bên trái / phải). Các cực đại của đoạn lượn tới được của
# một núi chính là những núi nhận nó làm SL hoặc SR, nên childmax[i] tích lũy đúng
# max(dp[j]) trên toàn bộ đoạn lượn tới được của i. O(n log n) do sort, phần còn lại O(n).

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

    # SL[i]: vị trí gần nhất bên trái có chiều cao > h[i] (lớn hơn thực sự), hoặc -1
    # SR[i]: vị trí gần nhất bên phải có chiều cao > h[i] (lớn hơn thực sự), hoặc n
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
    order = sorted(range(n), key=h.__getitem__)  # tăng dần theo chiều cao

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
