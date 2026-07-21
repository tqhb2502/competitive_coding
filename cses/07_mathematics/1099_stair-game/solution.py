# Stair Game - CSES 1099
# https://cses.fi/problemset/task/1099
# Staircase Nim: first player wins iff XOR of balls on 1-indexed even
# positions (stairs at odd distance from the sink, stair 1) is non-zero.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    t = int(data[pos]); pos += 1
    out = []
    for _ in range(t):
        n = int(data[pos]); pos += 1
        # 1-indexed even stairs = 0-based odd indices within this test's array.
        x = 0
        # data[pos .. pos+n-1] are the ball counts; we XOR entries at
        # 0-based indices 1, 3, 5, ... (i.e. offset+1, offset+3, ...).
        for j in range(pos + 1, pos + n, 2):
            x ^= int(data[j])
        pos += n
        out.append("first" if x != 0 else "second")
    sys.stdout.write("\n".join(out) + "\n")


main()
