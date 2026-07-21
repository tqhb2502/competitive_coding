# Finding Periods - CSES 1733
# https://cses.fi/problemset/task/1733
#
# A period length p is valid iff s has a border of length b = n - p.
# The set of all border lengths of s is the chain
#   pi[n-1], pi[pi[n-1]-1], ...  down to 0,
# where pi is the KMP prefix (failure) function. Walking that chain from the
# largest border to the smallest yields periods p = n - b in increasing order;
# finally p = n (empty border) is always a period.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]  # bytes; indexing yields ints (fast)
    n = len(s)

    # KMP prefix function; local bindings for speed on n up to 1e6.
    pi = [0] * n
    k = 0
    for i in range(1, n):
        c = s[i]
        while k > 0 and s[k] != c:
            k = pi[k - 1]
        if s[k] == c:
            k += 1
        pi[i] = k

    # Walk the border chain -> periods in increasing order.
    res = []
    b = pi[n - 1]
    while b > 0:
        res.append(n - b)
        b = pi[b - 1]
    res.append(n)  # the full string is always a period

    sys.stdout.write(" ".join(map(str, res)))
    sys.stdout.write("\n")


main()
