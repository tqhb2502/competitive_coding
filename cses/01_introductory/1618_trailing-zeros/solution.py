# Trailing Zeros - CSES 1618
# https://cses.fi/problemset/task/1618
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    res = 0
    p = 5
    while p <= n:
        res += n // p
        p *= 5
    sys.stdout.write(str(res) + "\n")


if __name__ == "__main__":
    main()
