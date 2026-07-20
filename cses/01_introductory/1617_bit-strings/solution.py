# Bit Strings - CSES task 1617
# https://cses.fi/problemset/task/1617
# Number of bit strings of length n = 2^n mod (10^9 + 7).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    MOD = 10 ** 9 + 7
    sys.stdout.write(str(pow(2, n, MOD)))


if __name__ == "__main__":
    main()
