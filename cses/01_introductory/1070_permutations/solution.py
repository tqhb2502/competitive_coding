# Permutations - CSES 1070
# https://cses.fi/problemset/task/1070
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    if n == 1:
        sys.stdout.write("1\n")
        return
    if n == 2 or n == 3:
        sys.stdout.write("NO SOLUTION\n")
        return

    # n >= 4: even numbers first, then odd numbers.
    result = list(range(2, n + 1, 2)) + list(range(1, n + 1, 2))
    sys.stdout.write(" ".join(map(str, result)))
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
