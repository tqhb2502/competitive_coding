# Finding Borders - CSES 1732
# https://cses.fi/problemset/task/1732
#
# A border is a prefix that is also a suffix (but not the whole string).
# Use the KMP prefix function (pi). The longest border of the whole string
# is pi[n-1]; following the chain b -> pi[b-1] -> ... enumerates every border
# length in decreasing order. Reverse for increasing order.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("\n")
        return
    s = data[0]                    # bytes; s[i] is an int in Python 3
    n = len(s)

    # prefix function (KMP failure function), O(n) amortized
    pi = [0] * n
    k = 0
    for i in range(1, n):
        c = s[i]
        while k > 0 and s[k] != c:
            k = pi[k - 1]
        if s[k] == c:
            k += 1
        pi[i] = k

    # collect all border lengths by following the pi-chain from the end
    res = []
    b = pi[n - 1]
    while b > 0:
        res.append(b)
        b = pi[b - 1]
    res.reverse()

    sys.stdout.write(" ".join(map(str, res)) + "\n")


main()
