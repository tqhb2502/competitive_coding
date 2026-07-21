# Candy Lottery - CSES 1727
# https://cses.fi/problemset/task/1727
# E[max] = sum_{m=1}^{k} m * ((m/k)^n - ((m-1)/k)^n)
# n children, each uniform integer in [1, k]; expected value of the maximum.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])

    expected = 0.0
    prev = 0.0  # P(max <= m-1) = ((m-1)/k)^n
    for m in range(1, k + 1):
        cur = (m / k) ** n  # P(max <= m)
        expected += m * (cur - prev)
        prev = cur

    sys.stdout.write("%.6f\n" % expected)


if __name__ == "__main__":
    main()
