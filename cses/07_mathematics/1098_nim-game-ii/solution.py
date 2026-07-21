# Nim Game II - https://cses.fi/problemset/task/1098
# Subtraction game with move set {1,2,3}: Grundy(x) = x % 4.
# XOR all heap Grundy values; nonzero -> "first" wins, zero -> "second".

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1
    out = []
    for _ in range(t):
        n = int(data[idx]); idx += 1
        g = 0
        end = idx + n
        while idx < end:
            g ^= int(data[idx]) & 3  # x % 4 == x & 3
            idx += 1
        out.append("first" if g != 0 else "second")
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
