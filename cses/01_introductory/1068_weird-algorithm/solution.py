# Weird Algorithm - https://cses.fi/problemset/task/1068
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    res = []
    while n != 1:
        res.append(n)
        if n % 2 == 0:
            n //= 2
        else:
            n = 3 * n + 1
    res.append(1)
    sys.stdout.write(" ".join(map(str, res)) + "\n")


if __name__ == "__main__":
    main()
