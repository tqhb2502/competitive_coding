# Gray Code - CSES 2205
# https://cses.fi/problemset/task/2205
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    total = 1 << n
    out = []
    for i in range(total):
        g = i ^ (i >> 1)
        out.append(format(g, '0{}b'.format(n)))
    sys.stdout.write("\n".join(out) + "\n")


main()
