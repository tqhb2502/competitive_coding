# String Matching - https://cses.fi/problemset/task/1753
# Đếm số lần pattern xuất hiện trong text bằng thuật toán KMP (deterministic, O(n + m)).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if len(data) < 2:
        sys.stdout.write("0\n")
        return
    t = data[0]  # text (bytes)
    p = data[1]  # pattern (bytes)

    n = len(t)
    m = len(p)
    if m > n:
        sys.stdout.write("0\n")
        return

    # Bước 1: tính failure function (prefix function) của pattern.
    pi = [0] * m
    k = 0
    for i in range(1, m):
        c = p[i]
        while k and p[k] != c:
            k = pi[k - 1]
        if p[k] == c:
            k += 1
        pi[i] = k

    # Bước 2: quét text, đếm số lần khớp đầy đủ (kể cả chồng lấp).
    ans = 0
    k = 0
    for i in range(n):
        c = t[i]
        while k and p[k] != c:
            k = pi[k - 1]
        if p[k] == c:
            k += 1
            if k == m:
                ans += 1
                k = pi[k - 1]

    sys.stdout.write(str(ans) + "\n")


main()
