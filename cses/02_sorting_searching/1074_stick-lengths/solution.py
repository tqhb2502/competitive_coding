# Stick Lengths - CSES 1074
# https://cses.fi/problemset/task/1074
# Ý tưởng: chi phí sum(|p_i - t|) đạt cực tiểu tại median của dãy.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    p = list(map(int, data[1:1 + n]))
    p.sort()
    median = p[n // 2]
    total = 0
    for x in p:
        total += x - median if x >= median else median - x
    sys.stdout.write(str(total) + "\n")


if __name__ == "__main__":
    main()
