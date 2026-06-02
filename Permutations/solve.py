import sys


def solve():
    input = sys.stdin.readline
    n = int(input())

    if (n % 2 == 0 and (n - 1) - 2 > 1) or (n % 2 == 1 and n - 2 > 1):
        for v in range(1, n + 1, 2):
            print(v, end=" ")
        for v in range(2, n + 1, 2):
            print(v, end=" ")
    else:
        print("NO SOLUTION")


if __name__ == "__main__":
    solve()
