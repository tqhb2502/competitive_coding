# String Functions - CSES 2107
# https://cses.fi/problemset/task/2107
#
# In ra Z-function (dòng 1) và prefix function π (dòng 2) của xâu s.
# Cả hai đều là thuật toán tất định O(n), không dùng hashing.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]                # làm việc trực tiếp trên bytes (index -> int)
    n = len(s)

    # ----- Z-function (z[0] = 0 theo quy ước z(1)=0 của đề) -----
    z = [0] * n
    l = 0
    r = 0
    for i in range(1, n):
        zi = 0
        if i < r:
            d = r - i
            zli = z[i - l]
            zi = d if d < zli else zli
        # mở rộng bằng so sánh trực tiếp
        while i + zi < n and s[zi] == s[i + zi]:
            zi += 1
        z[i] = zi
        if i + zi > r:
            l = i
            r = i + zi

    # ----- prefix function (KMP) -----
    pi = [0] * n
    for i in range(1, n):
        j = pi[i - 1]
        si = s[i]
        while j > 0 and si != s[j]:
            j = pi[j - 1]
        if si == s[j]:
            j += 1
        pi[i] = j

    out = sys.stdout
    out.write(' '.join(map(str, z)))
    out.write('\n')
    out.write(' '.join(map(str, pi)))
    out.write('\n')


main()
