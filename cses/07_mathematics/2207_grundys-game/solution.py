# Grundy's Game - CSES 2207
# https://cses.fi/problemset/task/2207
#
# Sprague-Grundy. Split a heap k into a + (k-a) with a != k-a; the two heaps are
# independent, so the Grundy value of the resulting state is g(a) XOR g(k-a).
#   g(k) = mex { g(a) XOR g(k-a) : 1 <= a < k-a }
# g(n) == 0  =>  the player to move loses  =>  "second"; otherwise "first".
#
# The P-positions (g(n)=0) of this game form a FINITE set; the last P-position is
# n = 1222 (a verified result: no more zeros exist far beyond 10^6). Hence we only
# need Grundy values up to a threshold B = 1300 > 1222; every n > B is a sure win
# for the player to move => "first".

import sys


def main():
    B = 1300  # safely larger than the last P-position (1222)
    g = [0] * (B + 1)  # g[1] = g[2] = 0 (cannot split into two heaps of different size)
    for k in range(3, B + 1):
        m = (k - 1) // 2  # a runs 1..m to guarantee a < k-a
        seen = set()
        add = seen.add
        for a in range(1, m + 1):
            add(g[a] ^ g[k - a])
        mex = 0
        while mex in seen:
            mex += 1
        g[k] = mex

    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(1, t + 1):
        n = int(data[i])
        if n <= B:
            out.append("second" if g[n] == 0 else "first")
        else:
            out.append("first")  # no P-position beyond 1222
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
