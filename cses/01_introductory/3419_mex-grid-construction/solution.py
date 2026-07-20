# Mex Grid Construction - https://cses.fi/problemset/task/3419
# Value at cell (i, j) (0-indexed) equals i XOR j.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    out = []
    for i in range(n):
        out.append(" ".join(str(i ^ j) for j in range(n)))
    sys.stdout.write("\n".join(out))
    if n > 0:
        sys.stdout.write("\n")


main()
