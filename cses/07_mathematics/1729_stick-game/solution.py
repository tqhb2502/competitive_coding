# Stick Game - CSES 1729
# https://cses.fi/problemset/task/1729
#
# Game theory (subtraction game). For each heap size i (1..n) print 'W' if the
# player to move wins, else 'L'.
#   win[0] = False (losing: cannot move, previous player took the last stick).
#   win[i] = True  iff exists p in P with p <= i and win[i-p] = False.
#
# Forward-marking implementation: scan i = 0..n; when win[i] is still unset it is
# a losing state, so mark all i+p (p in P) as winning. Because we scan in
# increasing order, every j < i has already propagated its marks before we read i.
# Complexity: O(n + (#losing)*k), memory O(n + maxp).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    moves = tuple(int(data[2 + i]) for i in range(k))
    maxp = max(moves)

    # win[i] == 1 -> winning state; 0 -> losing (default).
    # Extend by maxp so win[i + p] is always a valid index (no bound checks).
    win = bytearray(n + maxp + 1)
    res = bytearray(n)  # output chars for positions 1..n

    W = 87  # ord('W')
    L = 76  # ord('L')

    mv = moves
    w = win
    r = res
    for i in range(n + 1):
        if w[i]:
            # i >= 1 here because win[0] starts at 0 (losing)
            r[i - 1] = W
        else:
            # i is a losing state -> every i+p is a winning state
            for p in mv:
                w[i + p] = 1
            if i:
                r[i - 1] = L

    sys.stdout.write(r.decode())


main()
