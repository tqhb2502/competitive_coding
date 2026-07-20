# Reading Books - https://cses.fi/problemset/task/1631
# Đáp án = max(2*max(t_i), sum(t_i))
# Nếu quyển lớn nhất quá lớn (2*M > S) thì bị nghẽn bởi 2*M, ngược lại là S.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    total = 0
    mx = 0
    for i in range(1, n + 1):
        t = int(data[i])
        total += t
        if t > mx:
            mx = t
    ans = max(2 * mx, total)
    sys.stdout.write(str(ans) + "\n")


main()
